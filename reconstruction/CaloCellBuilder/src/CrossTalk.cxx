#include "CrossTalk.h"

#include "CaloCell/CaloDetDescriptor.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCell/CaloDetDescriptorCollection.h"
#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"

#include "G4Kernel/CaloPhiRange.h"
#include "G4Kernel/constants.h"

#include "G4SystemOfUnits.hh"

#include <map>


using namespace Gaugi;


CrossTalk::CrossTalk( std::string name ) : 
  IMsgService(name),
  Algorithm()
  // AlgTool()
{
  declareProperty( "MinEnergy"        , m_minEnergy=1*GeV                              );
  declareProperty( "CollectionKeys"   , m_collectionKeys={}                            ); // input
  declareProperty( "XTCellsKey"       , m_xtcellsKey="XTCells"                         ); // input (test_mateus)
  declareProperty( "CellsKey"         , m_cellsKey="Cells"                             ); // input (test_mateus)
  declareProperty( "HistogramPath"    , m_histPath="/CrossTalkSimulator"                                  );
  declareProperty( "OutputLevel"      , m_outputLevel=1                                );
}

//!=====================================================================

// CrossTalk::~CrossTalk()
// {;}

//!=====================================================================

StatusCode CrossTalk::initialize()
{
// MSG_INFO("Initializing CrossTalk module...");
// m_Tree = new TTree("xtalk", "xtalk"); // create output tree
  CHECK_INIT();
  setMsgLevel(m_outputLevel);

  // initialize tools
  for ( auto tool : m_toolHandles )
  {
    if (tool->initialize().isFailure() )
    {
      MSG_FATAL( "It's not possible to iniatialize " << tool->name() << " tool." );
    }
  }
  return StatusCode::SUCCESS;
}
//!=====================================================================

void CrossTalk::push_back( Gaugi::AlgTool* tool )
{
  m_toolHandles.push_back(tool);
}

//!=====================================================================

StatusCode CrossTalk::pre_execute( SG::EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CrossTalk::execute( SG::EventContext &/*ctx*/ , const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CrossTalk::execute( SG::EventContext &ctx , int /*evt*/ ) const
{
  // if( pre_execute(ctx).isFailure())
  // {
  //   MSG_FATAL("Its not possible to pre_execute. Abort!");
  // }

  // if( post_execute(ctx).isFailure())
  // {
  //   MSG_FATAL("Its not possible to post_execute. Abort!");
  // }
  MSG_INFO("Executing CrossTalk module...");

  std::vector < float > samples_xtalk_ind    ;
  std::vector < float > samples_xtalk_cap    ;
  std::vector < float > samples_signal       ;
  std::vector < float > samples_signal_xtalk ;

  SG::ReadHandle<xAOD::CaloCellContainer> container   (m_cellsKey, ctx);

  MSG_INFO( "Creating reco XT cells containers with key " << m_xtcellsKey);
  SG::WriteHandle<xAOD::CaloCellContainer> xtContainer( m_xtcellsKey , ctx );
  xtContainer.record( std::unique_ptr<xAOD::CaloCellContainer>(new xAOD::CaloCellContainer()) );
  
  // SG::ReadHandle<xAOD::CaloCellContainer> xtcontainer (m_xtcellsKey, ctx);
  // std::map < unsigned long int, std::vector<float> > *xtPulse; // map < hash , XT_pulse (Ind+cap) >


  // loop over ordinary cell container
  for (const auto cell : **container.ptr() ){
    xAOD::CaloDetDescriptor *descriptor = const_cast<xAOD::CaloDetDescriptor *>(cell->descriptor());

    auto xtcell = new xAOD::CaloCell();

    // Step 1: check if we need to apply cx method for current cell. Only for cells higher than
    // min energy. Here, lets use the truth energy from the main bunch crossing.
    bool bCrossTalkConditions = ( !(descriptor->edep() < m_minEnergy) && !(descriptor->pulse().size() == 0) && !((descriptor->sampling() != 3) && (descriptor->sampling()  != 12)) );

    
    // if (descriptor->edep() < m_minEnergy) continue;
    // if (descriptor->pulse().size() == 0) continue;
    // if ((descriptor->sampling() != 3) && (descriptor->sampling()  != 12)) continue;

    // ------------------------------------------------------------------------------
    // If there IS xtalk conditions, apply XT model to cell 1st neighbors,
    // change the current cell pulse, then add cell to new XT cell container.
    // -------------------------------------------------------------------------------
    if (bCrossTalkConditions){

      MSG_DEBUG("Sampling/Detector "<< descriptor->sampling() <<"/"<< descriptor->detector() <<", hash "<< descriptor->hash() <<", nsamples " << descriptor->pulse().size() << ", truthEne "<< descriptor->edep() << ", ene " << descriptor->e() << ", eta/phi "<< descriptor->eta() << "/"<< descriptor->phi() );


      // Step 2: build a 3x3 window around the central cell.
      //    Since this is a cell candidate, lets take all cells around this cells using a 3x3 window.
      //    First lets retrieve the full container in memory (not const objects inside of the collection)

      std::vector<const xAOD::CaloCell*> cells_around;

      // loop over ordinary cell container
      for (auto neighborCell : **container.ptr() ){
        const xAOD::CaloDetDescriptor *neighborDescriptor = neighborCell->descriptor();

        if ( neighborDescriptor->pulse().size() == 0) continue; // protection: if there is no pulseShape, skip that cell. 
        if ( descriptor->sampling() != neighborDescriptor->sampling() ) continue;  // cells_around must belong to the same sampling of central_cell
        if ( descriptor == neighborDescriptor) continue; // central_cell must not belong to cells_around
        
        // build a 3x3 window around the central cell
        float diffEta = std::abs( cell->eta() - neighborCell->eta() );
        float diffPhi = std::abs( CaloPhiRange::fix( cell->phi() - neighborCell->phi() ) );
        if( diffEta <= 3*cell->deltaEta()/2 && diffPhi <= 3*cell->deltaPhi()/2 ){
          cells_around.push_back(neighborCell);
        }
      }

      // Step 3: Loop over cells_around to extract xtalk effect from the central_cell surroundings.
      std::vector<float> final_xt_pulse(5);

      for (auto cellXT : cells_around){
    
        auto pulseCellXT = cellXT->descriptor()->pulse();
        std::vector<float> neighbor_xt_pulse;

        float distorted_sample_ind=0, distorted_sample_cap=0;
        
        // case 1: diagonal from central cell
        if (cellXT->eta() != cell->eta() && cellXT->phi() != cell->phi()){
          
          for (unsigned samp_index=0; samp_index<5; ++samp_index){
            distorted_sample_ind = XTalkTF( pulseCellXT[samp_index],samp_index,true, true);
            distorted_sample_cap = 0; // there is no capacitive cross-talk effect in the cell diagonal

            samples_xtalk_ind.push_back(distorted_sample_ind); // histogram
            samples_xtalk_cap.push_back(distorted_sample_cap);  // histogram

            neighbor_xt_pulse.push_back(distorted_sample_ind + distorted_sample_cap);
          }
        }
        else {
          // case 2: is inside central cross position        
          for (int samp_index=0; samp_index<5; samp_index++){
            distorted_sample_ind = XTalkTF( pulseCellXT[samp_index],samp_index,true, true);
            distorted_sample_cap = XTalkTF( pulseCellXT[samp_index],samp_index,true, false);

            samples_xtalk_ind.push_back(distorted_sample_ind); // histogram
            samples_xtalk_cap.push_back(distorted_sample_cap); // histogram

            neighbor_xt_pulse.push_back(distorted_sample_ind + distorted_sample_cap);
          }
        }
        // sum all xtalk effects around center cell
        for (int i=0; i<5; i++){
          final_xt_pulse[i] += neighbor_xt_pulse[i];
        }
      } // end-for in cells_around


      // Step 4: add total pulse distortion from neighbor cells into the central cell of the 3x3 window.
      auto centralCellPulse = descriptor->pulse(); 

      auto pulseBefore = descriptor->pulse();
      auto energyBefore = descriptor->e();

      for (int i=0; i<5; i++){
        samples_signal.push_back(centralCellPulse[i]); // add to fillHistograms
        centralCellPulse[i] = centralCellPulse[i] + final_xt_pulse[i]; 
        samples_signal_xtalk.push_back(centralCellPulse[i]); // add to fillHistograms
      }

      // Step 5: change pulse value of central cell of the 3x3 window with adjacent xtalk effects.
      descriptor->setPulse(centralCellPulse);

      // Step 6: Call for Estimation Methods tool (or any other tool applied into cells, AFTER pulse generation.)
      for ( auto tool : m_toolHandles )
      {
        // digitalization
        if( tool->execute( ctx, descriptor ).isFailure() ){
          MSG_ERROR( "It's not possible to execute the tool with name " << tool->name() );
          return StatusCode::FAILURE;
        }
      }

      auto pulseAfter   = centralCellPulse;
      auto energyAfter  = descriptor->e();

      // fillHistograms( ctx, samples_xtalk_ind, samples_xtalk_cap, samples_signal, samples_signal_xtalk );
      MSG_DEBUG(" e: "<< energyBefore <<"  Pulse before: " << pulseBefore[0] << "   "<< pulseBefore[1] << "   "<< pulseBefore[2] << "   "<< pulseBefore[3] << "   "<< pulseBefore[4]);
      MSG_DEBUG(" e_xt: "<< energyAfter <<"  Pulse after: " << pulseAfter[0] << "   "<< pulseAfter[1] << "   "<< pulseAfter[2] << "   "<< pulseAfter[3] << "   "<< pulseAfter[4]);

      samples_xtalk_ind.clear();
      samples_xtalk_cap.clear();
      samples_signal.clear();
      samples_signal_xtalk.clear(); 
    }

    // ------------------------------------------------------------------------------
    // If there is NO xtalk conditions, add the cell normally into new XT Container.
    //  Look, here, the current descriptor hasn't been changed.
    // -------------------------------------------------------------------------------
    // Setup the caloCell to XT cell container (Copy from original CellContainer)    
    xtcell->setEta( descriptor->eta() );
    xtcell->setPhi( descriptor->phi() );
    xtcell->setDeltaEta( descriptor->deltaEta() );
    xtcell->setDeltaPhi( descriptor->deltaPhi() );
    xtcell->setE( descriptor->e() ); // Estimated energy from OF/COF
    xtcell->setEt( cell->e() / std::cosh( cell->eta() ) );
    xtcell->setDescriptor( descriptor );
    xtContainer->push_back( xtcell ); //add CaloCell to XTcontainer
    

  }

  MSG_DEBUG("collection.size: "<< container.ptr()->size() << ", xtCollection.size(): "<< xtContainer->size());

  return StatusCode::SUCCESS;
  
  // return post_execute(ctx);

}
//!=====================================================================

StatusCode CrossTalk::finalize()
{
  // MSG_INFO("Finalizing CrossTalk module...");
  for ( auto tool : m_toolHandles )
  {
    if (tool->finalize().isFailure() )
    {
      MSG_ERROR( "It's not possible to finalize " << tool->name() << " tool." );
    }
  }

  return StatusCode::SUCCESS;
}

//!=====================================================================

// StatusCode CrossTalk::execute( SG::EventContext &ctx, Gaugi::EDM *edm ) const // test_mateus
StatusCode CrossTalk::post_execute( SG::EventContext &ctx ) const
{
  // MSG_INFO("Executing CrossTalk module...");
  // It is just a cell from ROI. The execute loops over one cell per event.
  // auto *cellExt = static_cast<xAOD::CaloDetDescriptor*>(edm); // test_mateus
  
  // ******* DEBUG ***********
  // SG::ReadHandle<xAOD::CaloDetDescriptorCollection> xtCollection( m_xtcellsKey, ctx );

  // if( !xtCollection.isValid() ){
  //   MSG_FATAL("It's not possible to retrieve the CaloDetDescriptor XT Container using this key: " << m_xtcellsKey);
  // }
  // for (auto &pair : **xtCollection.ptr()){
  //   auto *cell = pair.second; // cell

  //   if ( (cell->edep() < m_minEnergy) || (cell->edep() <= 0) ){
  //     // return StatusCode::SUCCESS;
  //     continue;
  //   }
  //   if ( (cell->sampling() != 3) && (cell->sampling()  != 12) ){
  //     // return StatusCode::SUCCESS;
  //     continue;
  //   }
  //   if ( cell->pulse().size() == 0){ // protection: if there is no pulseShape, skip that cell. 
  //     // return StatusCode::SUCCESS;
  //     continue;
  //   }
  //   MSG_INFO("Cell sampling: "<< cell->sampling() << ", cell energy truth: "<< cell->edep());
  // }
  // return StatusCode::SUCCESS;
  // *****************************

  // Step 1: check if we need to apply cx methos. Only for cells higher than
  // min energy. At this point we dont have the estimated energy from pulse.
  // Here, lets use the truth energy from the main bunch crossing.

  // Since this is a cell candidate, lets take all cells around this cells using a 3x3 window.
  // First lets retrieve the full container in memory (not const objects inside of the collection)
  // SG::ReadHandle<xAOD::CaloDetDescriptorCollection> collection( m_collectionKey, ctx );
  // if( !collection.isValid() ){
  //   MSG_FATAL("It's not possible to retrieve the CaloDetDescriptorCollection using this key: " << m_collectionKey);
  // }
  
  // std::vector<xAOD::CaloDetDescriptor*> cells_around;

  // std::vector<double> BaseAmpXTc ,
  //                     BaseAmpXTl ,
  //                     BaseAmpXTr ,
  //                     etacentralCell ,
  //                     phicentralCell ,
  //                     etaCluster ,
  //                     phiCluster ,
  //                     RelativeAmpXTc ,
  //                     RelativeAmpXTl ,
  //                     RelativeAmpXTr ,
  //                     SampRelatClusXTc ,
  //                     SampRelatClusXTl ,
  //                     SampRelatClusXTr ,
  //                     SampClusNoise ,
  //                     vectorTau_0 ,
  //                     delayPerCell ,
  //                     CellSigSamples, 
  //                     AmpCoeff, 
  //                     TimeCoeff ;
  // // verify if cell belongs to sampling 2 of EMCal

  // if ( (cellExt->sampling() != 3) && (cellExt->sampling()  != 12) ){
  //   return StatusCode::SUCCESS;
  // }

  // if ( (cellExt->edep() < m_minEnergy) || (cellExt->edep() <= 0) ){
  //   return StatusCode::SUCCESS;
  // }

  // for (auto &pair : **collection.ptr()){
  //   auto *cell = pair.second; // cell

  //   if (cell == cellExt){ // central_cell must not belong to cells_around
  //     // return StatusCode::SUCCESS;
  //     continue;
  //   }
  //   if ( cell->sampling() != cellExt->sampling() ){ // cells_around must belong to the same sampling of central_cell
  //     // return StatusCode::SUCCESS;
  //     continue;
  //   }
  //   if ( cell->pulse().size() == 0){ // protection: if there is no pulseShape, skip that cell. 
  //     // return StatusCode::SUCCESS;
  //     continue;
  //   }
    
  //   // build a 3x3 window around the central cell
  //   float diffEta = std::abs( cellExt->eta() - cell->eta() );
  //   float diffPhi = std::abs( CaloPhiRange::fix( cellExt->phi() - cell->phi() ) );
  //   if( diffEta <= 3*cell->deltaEta()/2 && diffPhi <= 3*cell->deltaPhi()/2 ){
  //     cells_around.push_back(cell);
  //   }
  // }
      
  // std::vector<float> final_pulse(5);
  
  // // Loop over cells_around to extract xtalk effect from the central_cell surroundings.
  // for (auto cellXT : cells_around){
  
  //   auto pulseCellXT = cellXT->pulse();
  //   std::vector<float> distorted_pulse;
  //   float distorted_sample_ind, distorted_sample_cap=0;
    
  //   // case 1: diagonal
  //   if (cellXT->eta() != cellExt->eta() && cellXT->phi() != cellExt->phi()){
      
  //     for (unsigned samp_index=0; samp_index<5; ++samp_index){
  //       distorted_sample_ind = XTalkTF( pulseCellXT[samp_index],samp_index,true, true);
  //       samples_xtalk_ind.push_back(distorted_sample_ind);
  //       distorted_pulse.push_back(distorted_sample_ind);
  //     }

  //   }
  //   else {
  //     // is inside central cross position        
  //     for (int samp_index=0; samp_index<5; samp_index++){
  //       distorted_sample_ind = XTalkTF( pulseCellXT[samp_index],samp_index,true, true);
  //       distorted_sample_cap = XTalkTF( pulseCellXT[samp_index],samp_index,true, false);

  //       samples_xtalk_ind.push_back(distorted_sample_ind);
  //       samples_xtalk_cap.push_back(distorted_sample_cap);

  //       distorted_pulse.push_back(distorted_sample_ind + distorted_sample_cap);
  //     }

  //   }
  //   // sum all xtalk effects around center cell
  //   for (int i=0; i<5; i++){
  //     final_pulse[i] += distorted_pulse[i];
  //   }
  // } // end-for in cells_around

  // auto centralCellPulse = cellExt->pulse(); 

  // // add total pulse distortion of adjacent cell into the central cell of the 3x3 window.
  // // change pulse value of central cell of the 3x3 window with adjacent xtalk effects.
  // for (int i=0; i<5; i++){
  //   samples_signal.push_back(centralCellPulse[i]); // add to fillHistograms
  //   centralCellPulse[i] = centralCellPulse[i] + final_pulse[i]; 
  //   samples_signal_xtalk.push_back(centralCellPulse[i]); // add to fillHistograms
  // }

  // // add modified centralCell to copiedROI container
  // // auto copiedCentralCell = cellExt;
  // // copiedCentralCell->setPulse(centralCellPulse);
  // // copiedROI.push_back(copiedCell);

  // cellExt->setPulse(centralCellPulse);

  // // fillHistograms( ctx, samples_xtalk_ind, samples_xtalk_cap, samples_signal, samples_signal_xtalk );

  // samples_xtalk_ind.clear();
  // samples_xtalk_cap.clear();
  // samples_signal.clear();
  // samples_signal_xtalk.clear();
  

  // // 1) get pulses from sliding 3x3 window around central cell
  // // 2) pass them into XTalkTF
  // // 3) sum them to the central cell
  // // 4) set new pulse for central cell
  // // 5) go to next window...

  // // 6) book

  // //MSG_INFO("We find numbe rof celles "<<cells_around.size() );
  
  // // With cells_around and centralCell , we can apply the crosstalk logic.
  // // NOTE: at this point we can modify all objects (CaloDet) since 
  // // we not make this as const.

  return StatusCode::SUCCESS;
}

// void CrossTalk::XTalkTF(float &ind_part, float &cap_part, float sample, int samp_index, bool diagonal) {

//   float BaseAmpXTc = m_AmpXt_C*sample ;
//   float BaseAmpXTl = m_AmpXt_L*sample ;
//   float BaseAmpXTr = m_AmpXt_R*sample ;

//   float XTcSamples = BaseAmpXTc * XTalk       (25*(samp_index+1) , false ); //+ delayPerCell[cell] + m_tau_0, false ) ) ;
//   float XTlSamples = BaseAmpXTl * XTalk       (25*(samp_index+1) , false ); //+ delayPerCell[cell] + m_tau_0, false ) ) ;
//   float XTrSamples = BaseAmpXTr * CellFunction(25*(samp_index+1) , false ); //+ delayPerCell[cell] + m_tau_0, false ) ) ;

//   if (diagonal){
//     ind_part = XTlSamples;
//     // return XTlSamples;
//   }
//   else{
//     // return XTcSamples + XTlSamples;
//     cap_part = XTcSamples;
//     ind_part = XTlSamples;
//   }
  
//   // SampClusNoise.push_back( noise->Gaus(0, 2) ) ;

// }
float CrossTalk::XTalkTF(float sample, int samp_index, bool diagonal, bool inductive) const
{

  float BaseAmpXTc = m_AmpXt_C*sample ;
  float BaseAmpXTl = m_AmpXt_L*sample ;
  // float BaseAmpXTr = m_AmpXt_R*sample ;
  float XTcSamples = BaseAmpXTc * XTalk       (25*(samp_index+1) , false ); //+ delayPerCell[cell] + m_tau_0, false ) ) ;
  float XTlSamples = BaseAmpXTl * XTalk       (25*(samp_index+1) , false ); //+ delayPerCell[cell] + m_tau_0, false ) ) ;
  // float XTrSamples = BaseAmpXTr * CellFunction(25*(samp_index+1) , false ); //+ delayPerCell[cell] + m_tau_0, false ) ) ;

  if (diagonal && inductive){
    // ind_part = XTlSamples;
    return XTlSamples;
  }
  else{
    // return XTcSamples + XTlSamples;
    if (inductive){
      return XTlSamples;
      }
    else{
      return XTcSamples;
    }
  }
  
  // SampClusNoise.push_back( noise->Gaus(0, 2) ) ;

}

double CrossTalk::XTalk(double x, bool type) const
{// const {
  TF1* XT_cellTF = new TF1("XT_cellTF","((exp(-x/[0])*x*x)/(2 *[0]*[0]*([0] - [1])) - (exp(-(x/[0]))*x*[1])/([0]*pow([0] - [1],2)) + exp(-(x/[0]))*[1]*[1]/pow([0] - [1],3) + (exp(-(x/[1]))*[1]*[1])/pow(-[0] + [1],3) + (1/(2*[2]*[0] *pow(([0] - [1]),3)))*exp(-x* (1/[0] + 1/[1]))* (-2 *exp(x *(1/[0] + 1/[1]))*[0] *pow(([0] - [1]),3) - 2 *exp(x/[0])*[0]*pow([1],3) + exp(x/[1]) *(x*x *pow(([0] - [1]),2) + 2*x*[0]*([0]*[0] - 3*[0]*[1] + 2*[1]*[1]) + 2*[0]*[0]*([0]*[0] - 3*[0]*[1] + 3*[1]*[1]))) + ((1 - (exp((-x + [2])/[0])*(x - [2])*([0] - 2*[1]))/pow(([0] - [1]),2) - (exp((-x + [2])/[0])*(x - [2])*(x- [2]))/(2*[0]*([0] - [1])) + (exp((-x + [2])/[1])*[1]*[1]*[1])/pow(([0] - [1]),3) - (exp((-x + [2])/[0])*[0]*([0]*[0] - 3*[0]*[1] + 3*[1]*[1]))/pow(([0] - [1]),3))* 0.5*( 1+sign(1, x -[2]) ) )/[2])*[3]*[4]*[3]*[0]*[0]",0., m_tmax2);
  XT_cellTF->SetParameter(0, m_taud);
  XT_cellTF->SetParameter(1, m_taupa);
  XT_cellTF->SetParameter(2, m_td);
  XT_cellTF->SetParameter(3, m_Rf);
  XT_cellTF->SetParameter(4, m_C1);

  double xt_cell = 0 ;

  if (type){
    xt_cell = XT_cellTF->Derivative(x) ;
  }
  else {
    xt_cell = XT_cellTF->Eval(x) ;
  }

  delete XT_cellTF ;

  return xt_cell ;
}// end of function


double CrossTalk::CellFunction(double x, bool type) const
{
  TF1* CellM = new TF1("CellM","[5]*((exp(-x/[0])*x*x)/(2 *[0]*[0]*([0] - [1])) - (exp(-(x/[0]))*x*[1])/([0]*pow([0] - [1],2)) + exp(-(x/[0]))*[1]*[1]/pow([0] - [1],3) + (exp(-(x/[1]))*[1]*[1])/pow(-[0] + [1],3) + (1/(2*[2]*[0] *pow(([0] - [1]),3)))*exp(-x* (1/[0] + 1/[1]))* (-2 *exp(x *(1/[0] + 1/[1]))*[0] *pow(([0] - [1]),3) - 2 *exp(x/[0])*[0]*pow([1],3) + exp(x/[1]) *(x*x *pow(([0] - [1]),2) + 2*x*[0]*([0]*[0] - 3*[0]*[1] + 2*[1]*[1]) + 2*[0]*[0]*([0]*[0] - 3*[0]*[1] + 3*[1]*[1]))) + ((1 - (exp((-x + [2])/[0])*(x - [2])*([0] - 2*[1]))/pow(([0] - [1]),2) - (exp((-x + [2])/[0])*(x - [2])*(x- [2]))/(2*[0]*([0] - [1])) + (exp((-x + [2])/[1])*[1]*[1]*[1])/pow(([0] - [1]),3) - (exp((-x + [2])/[0])*[0]*([0]*[0] - 3*[0]*[1] + 3*[1]*[1]))/pow(([0] - [1]),3))* 0.5*( 1+sign(1,x -[2]) ) )/[2])*[3]*[4]*[3]*[0]*[0]",0., m_tmax2);

  CellM->SetParameter(0, m_taud);
  CellM->SetParameter(1, m_taupa);
  CellM->SetParameter(2, m_td);
  CellM->SetParameter(3, m_Rf);
  CellM->SetParameter(4, m_C1);

  double cell = 0 ;

  if (type){
      cell = CellM->Derivative(x) ;        
  }
  else {
      cell = CellM->Eval(x) ;
  }

  delete CellM ;

  return cell ;
}// end of function

// StatusCode CrossTalk::fillHistograms( SG::EventContext &ctx) const override{
// StatusCode CrossTalk::fillHistograms( SG::EventContext &ctx, std::vector<float> samples_ind, std::vector<float> samples_cap, std::vector<float> samples_signal, std::vector<float> samples_signal_xtalk ) {
//   auto store = ctx.getStoreGateSvc();
//   MSG_WARNING(" mateushufnagel store 1");
//   store->mkdir(m_histPath);
//   MSG_WARNING(" mateushufnagel store 2");
//   store->cd(m_histPath);
//   MSG_WARNING(" mateushufnagel store 3");

//   for (int samp=0; samp < 5; ++samp){
//     store->hist1("samples_xtalk_ind")->Fill( samp, samples_ind[samp] );
//     store->hist1("samples_xtalk_cap")->Fill( samp, samples_cap[samp] );
//     store->hist1("samples_signal")->Fill( samp, samples_signal[samp] );
//     store->hist1("samples_signal_xtalk")->Fill( samp, samples_signal_xtalk[samp] );
//   }
//   MSG_WARNING(" mateushufnagel fillhist");
//   return StatusCode::SUCCESS;
// }

StatusCode CrossTalk::fillHistograms( SG::EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CrossTalk::bookHistograms( SG::EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}
// void CrossTalk::BuildSampCluster(vector<double> BaseAmpXTc, vector<double> BaseAmpXTl,  vector<double> BaseAmpXTr, vector<double> delayPerCell, vector<double> &XTcSamples, vector<double> &XTlSamples, vector<double> &XTrSamples, vector<double> &CellSigSamples, vector<double> &SampClusNoise, uint m_Nsamples, double m_tau_0 ) const {
     
//   // build cluster wit cell signal samples and XT signals samples
//   // adding sampling delay per cell and a arbitrary tau_0 to validate
//   // XT mitigation methods.
//   CellSigSamples.clear() ;
//   XTcSamples.clear() ;
//   XTlSamples.clear() ;
//   XTrSamples.clear() ;
//   SampClusNoise.clear() ;

//   for (uint cell=0; cell < delayPerCell.size(); cell++ ){
//     for (uint s=0; s < m_Nsamples; s++ ){
//       CellSigSamples.push_back( CellFunction( 25*(s+1) + delayPerCell[cell] + m_tau_0, false ) ) ;
//       XTcSamples.push_back( BaseAmpXTc[cell] * XTalk( 25*(s+1) + delayPerCell[cell] + m_tau_0, false ) ) ;
//       XTlSamples.push_back( BaseAmpXTl[cell] * XTalk( 25*(s+1) + delayPerCell[cell] + m_tau_0, false ) ) ;
//       XTrSamples.push_back( BaseAmpXTr[cell] * CellFunction( 25*(s+1) + delayPerCell[cell] + m_tau_0, false ) ) ;
//       SampClusNoise.push_back( noise->Gaus(0, 2) ) ;
//     }
//   }
//   AmpClusterNoise( SampClusNoise ) ;

// }// end of function


