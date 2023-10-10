#include "CrossTalkMaker.h"

#include "CaloCell/CaloDetDescriptor.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCell/CaloDetDescriptorCollection.h"
#include "G4Kernel/CaloPhiRange.h"
#include "G4Kernel/constants.h"

#include "G4SystemOfUnits.hh"

#include <map>


using namespace Gaugi;


CrossTalkMaker::CrossTalkMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{

  declareProperty( "InputCollectionKey"     , m_collectionKey="Cells"               ); // input
  declareProperty( "OutputCollectionKey"    , m_xtcollectionKey="XTCells"           ); // output
  declareProperty( "MinEnergy"              , m_minEnergy=1*GeV                     );
  declareProperty( "HistogramPath"          , m_histPath="/CrossTalkMakerSimulator"      );
  declareProperty( "OutputLevel"            , m_outputLevel=1                       );
  declareProperty( "XTAmpCapacitive"        , m_AmpXt_C=4.2                         );
  declareProperty( "XTAmpInductive"         , m_AmpXt_L=2.3                         );
  declareProperty( "XTAmpResistive"         , m_AmpXt_R=1.0                         );
}\

//!=====================================================================

StatusCode CrossTalkMaker::initialize()
{
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

void CrossTalkMaker::push_back( Gaugi::AlgTool* tool )
{
  m_toolHandles.push_back(tool);
}

//!=====================================================================

StatusCode CrossTalkMaker::bookHistograms( SG::EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CrossTalkMaker::pre_execute( SG::EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CrossTalkMaker::execute( SG::EventContext &/*ctx*/ , const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CrossTalkMaker::execute( SG::EventContext &ctx , int /*evt*/ ) const
{
  MSG_INFO("Executing CrossTalkMaker module...");

  std::vector < float > samples_xtalk_ind    ;
  std::vector < float > samples_xtalk_cap    ;
  std::vector < float > samples_signal       ;
  std::vector < float > samples_signal_xtalk ;

  SG::ReadHandle<xAOD::CaloDetDescriptorCollection> collection( m_collectionKey, ctx );



  MSG_INFO( "Creating reco XT cells containers with key " << m_xtcollectionKey);
  SG::WriteHandle<xAOD::CaloDetDescriptorCollection> xtCollection( m_xtcollectionKey , ctx );
  xtCollection.record( std::unique_ptr<xAOD::CaloDetDescriptorCollection>(new xAOD::CaloDetDescriptorCollection()) );
  


  // loop over ordinary cell container
  for (const auto &pair : **collection.ptr() )
  {
    auto descriptor = pair.second;
    xAOD::CaloDetDescriptor *xtdescriptor = descriptor->copy();

    // Step 1: check if we need to apply cx method for current cell. Only for cells higher than
    // min energy. Here, lets use the truth energy from the main bunch crossing.
    bool bCrossTalkMakerConditions = ( !(xtdescriptor->edep() < m_minEnergy) && !(xtdescriptor->pulse().size() == 0) && !((xtdescriptor->sampling() != 3) && (xtdescriptor->sampling()  != 12)) );

    
    // if (descriptor->edep() < m_minEnergy) continue;
    // if (descriptor->pulse().size() == 0) continue;
    // if ((descriptor->sampling() != 3) && (descriptor->sampling()  != 12)) continue;

    // ------------------------------------------------------------------------------
    // If there IS xtalk conditions, apply XT model to cell 1st neighbors,
    // change the current cell pulse, then add cell to new XT cell container.
    // ------------------------------------------------------------------------------
    if (bCrossTalkMakerConditions)
    {

      MSG_DEBUG("Sampling/Detector "<< xtdescriptor->sampling() <<"/"<< xtdescriptor->detector() <<", hash "<< xtdescriptor->hash() 
                << ", nsamples " << xtdescriptor->pulse().size() << ", truthEne "<< xtdescriptor->edep() << ", ene " << xtdescriptor->e() 
                << ", eta/phi "<< xtdescriptor->eta() << "/"<< xtdescriptor->phi() );

      // Step 2: build a 3x3 window around the central cell.
      //    Since this is a cell candidate, lets take all cells around this cells using a 3x3 window.
      //    First lets retrieve the full container in memory (not const objects inside of the collection)
      std::vector<const xAOD::CaloDetDescriptor*> cells_around;

      // loop over ordinary cell container
      for (auto &pairAround : **collection.ptr() ){
        auto neighborDescriptor = pairAround.second;
        if ( neighborDescriptor->pulse().size() == 0) continue; // protection: if there is no pulseShape, skip that cell. 
        if ( xtdescriptor->sampling() != neighborDescriptor->sampling() ) continue;  // cells_around must belong to the same sampling of central_cell
        if ( xtdescriptor == neighborDescriptor) continue; // central_cell must not belong to cells_around
        // build a 3x3 window around the central cell
        float diffEta = std::abs( xtdescriptor->eta() - neighborDescriptor->eta() );
        float diffPhi = std::abs( CaloPhiRange::fix( xtdescriptor->phi() - neighborDescriptor->phi() ) );
        if( diffEta <= 3*xtdescriptor->deltaEta()/2 && diffPhi <= 3*xtdescriptor->deltaPhi()/2 ){
          cells_around.push_back(neighborDescriptor);
        }
      }

      // Step 3: Loop over cells_around to extract xtalk effect from the central_cell surroundings.
      std::vector<float> final_xt_pulse(5);

      for (auto cell : cells_around){
    
        auto pulseCellXT = cell->pulse();
        std::vector<float> neighbor_xt_pulse;
        float distorted_sample_ind=0, distorted_sample_cap=0;
        
        // case 1: diagonal from central cell
        if (cell->eta() != descriptor->eta() && cell->phi() != descriptor->phi())
        {
          for (unsigned samp_index=0; samp_index<5; ++samp_index)
          {
            distorted_sample_ind = XTalkTF( pulseCellXT[samp_index],samp_index,true, true);
            distorted_sample_cap = 0; // there is no capacitive cross-talk effect in the cell diagonal
            samples_xtalk_ind.push_back(distorted_sample_ind); // histogram
            samples_xtalk_cap.push_back(distorted_sample_cap);  // histogram
            neighbor_xt_pulse.push_back(distorted_sample_ind + distorted_sample_cap);
          }
        }else {
          // case 2: is inside central cross position        
          for (int samp_index=0; samp_index<5; samp_index++)
          {
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

      }// end-for in cells_around


      // Step 4: add total pulse distortion from neighbor cells into the central cell of the 3x3 window.
      auto centralCellPulse = descriptor->pulse(); 
      auto pulseBefore      = descriptor->pulse();
      auto energyBefore     = descriptor->e();

      for (int i=0; i<5; i++)
      {
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
    if ( !xtCollection->insert( descriptor->hash(), descriptor ) ){
        MSG_FATAL( "It is not possible to include cell hash ("<< descriptor->hash() << ") into the collection. hash already exist.");
    }

  }


  return StatusCode::SUCCESS;
  
  // return post_execute(ctx);

}


//!=====================================================================


StatusCode CrossTalkMaker::fillHistograms( SG::EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


//!=====================================================================


StatusCode CrossTalkMaker::finalize()
{
  // MSG_INFO("Finalizing CrossTalkMaker module...");
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

StatusCode CrossTalkMaker::post_execute( SG::EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


float CrossTalkMaker::XTalkTF(float sample, int samp_index, bool diagonal, bool inductive) const
{

  float BaseAmpXTc = m_AmpXt_C/100*sample ;
  float BaseAmpXTl = m_AmpXt_L/100*sample ;
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

double CrossTalkMaker::XTalk(double x, bool type) const
{
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


double CrossTalkMaker::CellFunction(double x, bool type) const
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





