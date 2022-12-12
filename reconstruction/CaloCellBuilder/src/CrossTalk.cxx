#include "CrossTalk.h"
#include "CaloCell/CaloDetDescriptor.h"
#include "CaloCell/CaloDetDescriptorCollection.h"
#include "G4Kernel/CaloPhiRange.h"
#include "G4Kernel/constants.h"
#include "G4SystemOfUnits.hh"


using namespace Gaugi;


CrossTalk::CrossTalk( std::string name ) : 
  IMsgService(name),
  AlgTool()
{
  declareProperty( "MinEnergy"        , m_minEnergy=1*GeV          );
  declareProperty( "CollectionKey"    , m_collectionKey="CaloDetDescriptorCollection"  ); // input
  declareProperty( "HistogramPath"    , m_histPath=""             );
}

//!=====================================================================

CrossTalk::~CrossTalk()
{}

//!=====================================================================

StatusCode CrossTalk::initialize()
{
  // MSG_INFO("Initializing CrossTalk module...");
  // m_Tree = new TTree("xtalk", "xtalk"); // create output tree
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CrossTalk::finalize()
{
  // MSG_INFO("Finalizing CrossTalk module...");
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CrossTalk::execute( SG::EventContext &ctx, Gaugi::EDM *edm ) const
{
  // MSG_INFO("Executing CrossTalk module...");
  auto *cellExt = static_cast<xAOD::CaloDetDescriptor*>(edm); // central cell of ROI


  // Step 1: check if we need to apply cx methos. Only for cells higher than
  // min energy. At this point we dont have the estimated energy from pulse.
  // Here, lets use the truth energy from the main bunch crossing.

  // Since this is a cell candidate, lets take all cells around this cells using a 3x3 window.
  // First lets retrieve the full container in memory (not const objects inside of the collection)
  SG::ReadHandle<xAOD::CaloDetDescriptorCollection> collection( m_collectionKey, ctx );
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloDetDescriptorCollection using this key: " << m_collectionKey);
  }
  
  std::vector<xAOD::CaloDetDescriptor*> cells_around;

  std::vector<double> BaseAmpXTc ,
                      BaseAmpXTl ,
                      BaseAmpXTr ,
                      etacentralCell ,
                      phicentralCell ,
                      etaCluster ,
                      phiCluster ,
                      RelativeAmpXTc ,
                      RelativeAmpXTl ,
                      RelativeAmpXTr ,
                      SampRelatClusXTc ,
                      SampRelatClusXTl ,
                      SampRelatClusXTr ,
                      SampClusNoise ,
                      vectorTau_0 ,
                      delayPerCell ,
                      CellSigSamples, 
                      AmpCoeff, 
                      TimeCoeff ;
  // verify if cell belongs to sampling 2 of EMCal

  if ( (cellExt->sampling() != 3) && (cellExt->sampling()  != 12) ){
    return StatusCode::SUCCESS;
  }

  if ( (cellExt->edep() < m_minEnergy) || (cellExt->edep() <= 0) ){
    return StatusCode::SUCCESS;
  }

  for (auto &pair : **collection.ptr()){
    auto *cell = pair.second; // cell

    if (cell == cellExt){ // central_cell must not belong to cells_around
      return StatusCode::SUCCESS;
    }
    if ( cell->sampling() != cellExt->sampling() ){ // cells_around must belong to the same sampling of central_cell
      return StatusCode::SUCCESS;
    }
    if ( cell->pulse().size() == 0){ // protection: if there is no pulseShape, skip that cell. 
      return StatusCode::SUCCESS;
    }
    
    // build a 3x3 window around the central cell
    float diffEta = std::abs( cellExt->eta() - cell->eta() );
    float diffPhi = std::abs( CaloPhiRange::fix( cellExt->phi() - cell->phi() ) );
    if( diffEta <= 3*cell->deltaEta()/2 && diffPhi <= 3*cell->deltaPhi()/2 ){
      cells_around.push_back(cell);
    }
  }
      
  std::vector<float> final_pulse(5);
  
  // Loop over cells_around to extract xtalk effect from the central_cell surroundings.
  for (auto cellXT : cells_around){
  
    auto pulseCellXT = cellXT->pulse();
    std::vector<float> distorted_pulse;
    float distorted_sample_ind, distorted_sample_cap=0;
    
    // case 1: diagonal
    if (cellXT->eta() != cellExt->eta() && cellXT->phi() != cellExt->phi()){
      
      for (unsigned samp_index=0; samp_index<5; ++samp_index){
        distorted_sample_ind = XTalkTF( pulseCellXT[samp_index],samp_index,true, true);
        samples_xtalk_ind.push_back(distorted_sample_ind);
        distorted_pulse.push_back(distorted_sample_ind);
      }

    }
    else {
      // is inside central cross position        
      for (int samp_index=0; samp_index<5; samp_index++){
        distorted_sample_ind = XTalkTF( pulseCellXT[samp_index],samp_index,true, true);
        distorted_sample_cap = XTalkTF( pulseCellXT[samp_index],samp_index,true, false);

        samples_xtalk_ind.push_back(distorted_sample_ind);
        samples_xtalk_cap.push_back(distorted_sample_cap);

        distorted_pulse.push_back(distorted_sample_ind + distorted_sample_cap);
      }

    }
    // sum all xtalk effects around center cell
    for (int i=0; i<5; i++){
      final_pulse[i] += distorted_pulse[i];
    }
  } // end-for in cells_around

  auto centralCellPulse = cellExt->pulse(); 

  // add total pulse distortion of adjacent cell into the central cell of the 3x3 window.
  // change pulse value of central cell of the 3x3 window with adjacent xtalk effects.
  for (int i=0; i<5; i++){
    samples_signal.push_back(centralCellPulse[i]); // add to fillHistograms
    centralCellPulse[i] = centralCellPulse[i] + final_pulse[i]; 
    samples_signal_xtalk.push_back(centralCellPulse[i]); // add to fillHistograms
  }
  cellExt->setPulse(centralCellPulse);

  // fillHistograms( ctx, samples_xtalk_ind, samples_xtalk_cap, samples_signal, samples_signal_xtalk );

  samples_xtalk_ind.clear();
  samples_xtalk_cap.clear();
  samples_signal.clear();
  samples_signal_xtalk.clear();
  

  // 1) get pulses from sliding 3x3 window around central cell
  // 2) pass them into XTalkTF
  // 3) sum them to the central cell
  // 4) set new pulse for central cell
  // 5) go to next window...

  // 6) book

  //MSG_INFO("We find numbe rof celles "<<cells_around.size() );
  
  // With cells_around and centralCell , we can apply the crosstalk logic.
  // NOTE: at this point we can modify all objects (CaloDet) since 
  // we not make this as const.

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
float CrossTalk::XTalkTF(float sample, int samp_index, bool diagonal, bool inductive) const{

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

double CrossTalk::XTalk(double x, bool type) const{// const {
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


double CrossTalk::CellFunction(double x, bool type) const{// const {
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
StatusCode CrossTalk::fillHistograms( SG::EventContext &ctx, std::vector<float> samples_ind, std::vector<float> samples_cap, std::vector<float> samples_signal, std::vector<float> samples_signal_xtalk ) const{
  auto store = ctx.getStoreGateSvc();
  MSG_WARNING(" mateushufnagel store 1");
  store->mkdir(m_histPath);
  MSG_WARNING(" mateushufnagel store 2");
  store->cd(m_histPath);
  MSG_WARNING(" mateushufnagel store 3");

  for (int samp=0; samp < 5; ++samp){
    store->hist1("samples_xtalk_ind")->Fill( samp, samples_ind[samp] );
    store->hist1("samples_xtalk_cap")->Fill( samp, samples_cap[samp] );
    store->hist1("samples_signal")->Fill( samp, samples_signal[samp] );
    store->hist1("samples_signal_xtalk")->Fill( samp, samples_signal_xtalk[samp] );
  }
  MSG_WARNING(" mateushufnagel fillhist");
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


