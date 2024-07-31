#include "CaloCell/CaloCellContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloRings/CaloRingsContainer.h"
#include "Particle/ElectronContainer.h"
#include "CaloCell/CaloCellConverter.h"
#include "CaloCell/CaloDetDescriptorConverter.h"
#include "EventInfo/EventInfoConverter.h"
#include "TruthParticle/TruthParticleConverter.h"
#include "CaloCluster/CaloClusterConverter.h"
#include "CaloRings/CaloRingsConverter.h"
#include "Particle/ElectronConverter.h"
#include "RootStreamNTUPLEMaker.h"
#include "GaugiKernel/EDM.h"


using namespace SG;
using namespace Gaugi;



RootStreamNTUPLEMaker::RootStreamNTUPLEMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "InputFile"          , m_inputFile=""                  );
  declareProperty( "InputEventKey"      , m_eventKey="EventInfo"          );
  declareProperty( "InputTruthKey"      , m_truthKey="Particles"          );
  declareProperty( "InputCellsKey"      , m_cellsKey="Cells"              );
  declareProperty( "InputClusterKey"    , m_clusterKey="Clusters"         );
  declareProperty( "InputRingerKey"     , m_ringerKey="Rings"             );
  declareProperty( "InputElectronKey"   , m_electronKey="Electrons"       );
  declareProperty( "OutputLevel"        , m_outputLevel=1                 );
  declareProperty( "NtupleName"         , m_ntupleName="physics"          );
  declareProperty( "OutputNtupleName"   , m_outputNtupleName="events"     );
  declareProperty( "SecondLambdaCuts"   , m_secondLambdaCuts={}           );
  declareProperty( "LateralMomCuts"     , m_lateralMomCuts={}             );
  declareProperty( "LongMomCuts"        , m_longMomCuts={}                );
  declareProperty( "FracMaxCuts"        , m_fracMaxCuts={}                );
  declareProperty( "SecondRCuts"        , m_secondRCuts={}                );
  declareProperty( "LambdaCenterCuts"   , m_lambdaCenterCuts={}           );


}

//!=====================================================================

RootStreamNTUPLEMaker::~RootStreamNTUPLEMaker()
{}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::bookHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  TFile *file = new TFile(m_inputFile.c_str(), "read");
  store->decorate( "events", file );

  
  store->cd();
  float eta = 0;
  float phi = 0;
  float e   = 0;
  float et  = 0;
  float deta         = 0;
  float dphi         = 0;
  float e0           = 0;
  float e1           = 0;
  float e2           = 0;
  float e3           = 0;
  float ehad1        = 0;
  float ehad2        = 0;
  float ehad3        = 0;
  float etot         = 0;
  float e233         = 0;
  float e237         = 0;
  float e277         = 0;
  float emaxs1       = 0;
  float emaxs2       = 0;
  float e2tsts1      = 0;
  float reta         = 0;
  float rphi         = 0;
  float rhad         = 0;
  float rhad1        = 0;
  float eratio       = 0;
  float f0           = 0;
  float f1           = 0;
  float f2           = 0;
  float f3           = 0;
  float weta2        = 0;
  std::vector<float>*rings        = nullptr;
  bool el_tight      = false;
  bool el_medium     = false;
  bool el_loose      = false;
  bool el_vloose     = false;
  float secondR      = 0;
  float lambdaCenter = 0;
  float secondLambda = 0;
  float fracMax      = 0;
  float lateralMom   = 0;
  float longitudinalMom = 0;
  bool el_fwdTight = false;
  bool el_fwdMedium = false;
  bool el_fwdLoose = false;
  float avgmu        = 0;
  int eventNumber = -9999;
  float el_eta = -1;
  float el_et = -1;
  float el_phi = -1;


  TTree *outputTree = new TTree(m_outputNtupleName.c_str(), "");
  outputTree->Branch( "cluster_eta",  &eta);
  outputTree->Branch( "cluster_phi",  &phi);
  outputTree->Branch( "cluster_e",    &e);
  outputTree->Branch( "cluster_et",   &et);
  outputTree->Branch("cluster_deta"   , &deta);
  outputTree->Branch("cluster_dphi"   , &dphi);
  outputTree->Branch("cluster_e0"     , &e0);
  outputTree->Branch("cluster_e1"     , &e1);
  outputTree->Branch("cluster_e2"     , &e2);
  outputTree->Branch("cluster_e3"     , &e3);
  outputTree->Branch("cluster_ehad1"  , &ehad1);
  outputTree->Branch("cluster_ehad2"  , &ehad2);
  outputTree->Branch("cluster_ehad3"  , &ehad3);
  outputTree->Branch("cluster_etot"   , &etot);
  outputTree->Branch("cluster_e233"   , &e233);
  outputTree->Branch("cluster_e237"   , &e237);
  outputTree->Branch("cluster_e277"   , &e277);
  outputTree->Branch("cluster_emaxs1" , &emaxs1);
  outputTree->Branch("cluster_emaxs2" , &emaxs2);
  outputTree->Branch("cluster_e2tsts1", &e2tsts1);
  outputTree->Branch("cluster_reta"   , &reta);
  outputTree->Branch("cluster_rphi"   , &rphi);
  outputTree->Branch("cluster_rhad"   , &rhad);
  outputTree->Branch("cluster_rhad1"  , &rhad1);
  outputTree->Branch("cluster_eratio" , &eratio);
  outputTree->Branch("cluster_f0"     , &f0);
  outputTree->Branch("cluster_f1"     , &f1);
  outputTree->Branch("cluster_f2"     , &f2);
  outputTree->Branch("cluster_f3"     , &f3);
  outputTree->Branch("cluster_weta2"  , &weta2);
  outputTree->Branch("rings"          , &rings);
  outputTree->Branch("el_tight"       , &el_tight);
  outputTree->Branch("el_medium"      , &el_medium);
  outputTree->Branch("el_loose"       , &el_loose);
  outputTree->Branch("el_vloose"      , &el_vloose);
  outputTree->Branch("cluster_secondR", &secondR);
  outputTree->Branch("cluster_lambdaCenter", &lambdaCenter);
  outputTree->Branch("cluster_secondLambda", &secondLambda);
  outputTree->Branch("cluster_fracMax", &fracMax);
  outputTree->Branch("cluster_lateralMom", &lateralMom);
  outputTree->Branch("cluster_longitudinalMom", &longitudinalMom);
  outputTree->Branch("el_fwdTight",             &el_fwdTight);
  outputTree->Branch("el_fwdMedium",             &el_fwdMedium);
  outputTree->Branch("el_fwdLoose",             &el_fwdLoose);
  outputTree->Branch("eventNumber"     , &eventNumber);
  outputTree->Branch("avgmu"          , &avgmu);
  outputTree->Branch("el_eta", &el_eta);
  outputTree->Branch("el_et", &el_et);
  outputTree->Branch("el_phi", &el_phi);

  store->add(outputTree);
  return StatusCode::SUCCESS; 
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::execute( EventContext &ctx, int evt ) const
{
  MSG_INFO("Called initialize");
  return deserialize( evt, ctx );
}

//!===================================    ==================================

StatusCode RootStreamNTUPLEMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::fillHistograms( EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNTUPLEMaker::deserialize( int evt, EventContext &ctx ) const
{
  std::vector<xAOD::CaloDetDescriptor_t > *collection_descriptor = nullptr;
  std::vector<xAOD::CaloCell_t          > *collection_cells      = nullptr;
  std::vector<xAOD::EventInfo_t         > *collection_event      = nullptr;
  std::vector<xAOD::TruthParticle_t     > *collection_truth      = nullptr;
  std::vector<xAOD::CaloCluster_t       > *collection_cluster    = nullptr;
  std::vector<xAOD::CaloRings_t         > *collection_rings      = nullptr;
  std::vector<xAOD::Electron_t          > *collection_electrons  = nullptr;

  MSG_INFO( "Link all branches..." );

  auto store = ctx.getStoreGateSvc();
  TFile *file = (TFile*)store->decorator("events");
  TTree *tree = (TTree*)file->Get(m_ntupleName.c_str());

  InitBranch( tree, ("EventInfoContainer_"         + m_eventKey).c_str() ,  &collection_event     );
  InitBranch( tree, ("TruthParticleContainer_"     + m_truthKey).c_str() ,  &collection_truth     );
  InitBranch( tree, ("CaloCellContainer_"          + m_cellsKey).c_str() ,  &collection_cells     );
  InitBranch( tree, ("CaloDetDescriptorContainer_" + m_cellsKey).c_str() ,  &collection_descriptor);
  InitBranch( tree, ("CaloClusterContainer_"       + m_clusterKey).c_str(), &collection_cluster);
  InitBranch( tree, ("CaloRingsContainer_"         + m_ringerKey).c_str(),  &collection_rings);
  InitBranch( tree, ("ElectronContainer_"          + m_electronKey).c_str(), &collection_electrons);

  tree->GetEntry( evt );

  store->cd();
  TTree *outputTree = store->tree(m_outputNtupleName.c_str());

  float eta = 0;
  float phi = 0;
  float e = 0;
  float et = 0;
  float deta         = 0;
  float dphi         = 0;
  float e0           = 0;
  float e1           = 0;
  float e2           = 0;
  float e3           = 0;
  float ehad1        = 0;
  float ehad2        = 0;
  float ehad3        = 0;
  float etot         = 0;
  float e233         = 0;
  float e237         = 0;
  float e277         = 0;
  float emaxs1       = 0;
  float emaxs2       = 0;
  float e2tsts1      = 0;
  float reta         = 0;
  float rphi         = 0;
  float rhad         = 0;
  float rhad1        = 0;
  float eratio       = 0;
  float f0           = 0;
  float f1           = 0;
  float f2           = 0;
  float f3           = 0;
  float weta2        = 0;
  std::vector<float> *rings = nullptr;
  bool el_tight      = false;
  bool el_medium     = false;
  bool el_loose      = false;
  bool el_vloose     = false;
  float secondR      = 0;
  float lambdaCenter = 0;
  float secondLambda = 0;
  float fracMax      = 0;
  float lateralMom   = 0;
  float longitudinalMom = 0;
  bool  el_fwdTight = false;
  bool  el_fwdMedium = false;
  bool  el_fwdLoose = false;
  float el_eta = -1;
  float el_et = -1;
  float el_phi = -1;
  float avgmu        = 0;
  int eventNumber    = 0;
  
  InitBranch( outputTree, "cluster_eta" , &eta);
  InitBranch( outputTree, "cluster_phi" , &phi);
  InitBranch( outputTree, "cluster_e"   , &e);
  InitBranch( outputTree, "cluster_et"  , &et);
  InitBranch( outputTree,"cluster_deta"   , &deta);
  InitBranch( outputTree,"cluster_dphi"   , &dphi);
  InitBranch( outputTree,"cluster_e0"     , &e0);
  InitBranch( outputTree,"cluster_e1"     , &e1);
  InitBranch( outputTree,"cluster_e2"     , &e2);
  InitBranch( outputTree,"cluster_e3"     , &e3);
  InitBranch( outputTree,"cluster_ehad1"  , &ehad1);
  InitBranch( outputTree,"cluster_ehad2"  , &ehad2);
  InitBranch( outputTree,"cluster_ehad3"  , &ehad3);
  InitBranch( outputTree,"cluster_etot"   , &etot);
  InitBranch( outputTree,"cluster_e233"   , &e233);
  InitBranch( outputTree,"cluster_e237"   , &e237);
  InitBranch( outputTree,"cluster_e277"   , &e277);
  InitBranch( outputTree,"cluster_emaxs1" , &emaxs1);
  InitBranch( outputTree,"cluster_emaxs2" , &emaxs2);
  InitBranch( outputTree,"cluster_e2tsts1", &e2tsts1);
  InitBranch( outputTree,"cluster_reta"   , &reta);
  InitBranch( outputTree,"cluster_rphi"   , &rphi);
  InitBranch( outputTree,"cluster_rhad"   , &rhad);
  InitBranch( outputTree,"cluster_rhad1"  , &rhad1);
  InitBranch( outputTree,"cluster_eratio" , &eratio);
  InitBranch( outputTree,"cluster_f0"     , &f0);
  InitBranch( outputTree,"cluster_f1"     , &f1);
  InitBranch( outputTree,"cluster_f2"     , &f2);
  InitBranch( outputTree,"cluster_f3"     , &f3);
  InitBranch( outputTree,"cluster_weta2"  , &weta2);
  InitBranch( outputTree,"el_tight"       , &el_tight);
  InitBranch( outputTree,"el_medium"      , &el_medium);
  InitBranch( outputTree,"el_loose"       , &el_loose);
  InitBranch( outputTree,"el_vloose"      , &el_vloose);

  InitBranch( outputTree,"cluster_secondR"  , &secondR);
  InitBranch( outputTree,"cluster_lambdaCenter"  , &lambdaCenter);
  InitBranch( outputTree,"cluster_secondLambda"  , &secondLambda);
  InitBranch( outputTree,"cluster_fracMax"  , &fracMax);
  InitBranch( outputTree,"cluster_lateralMom"  , &lateralMom);
  InitBranch( outputTree,"cluster_longitudinalMom"  , &longitudinalMom);
  InitBranch( outputTree,"el_fwdTight", &el_fwdTight);
  InitBranch( outputTree,"el_fwdMedium", &el_fwdMedium);
  InitBranch( outputTree,"el_fwdLoose", &el_fwdLoose);
  InitBranch( outputTree, "el_eta", &el_eta);
  InitBranch( outputTree, "el_et", &el_et);
  InitBranch( outputTree, "el_phi", &el_phi);
  InitBranch(outputTree, "event_number", &evt);
  

  { //main loop (from electron to rings)
  
    InitBranch(outputTree, "rings", &rings);
    SG::WriteHandle<xAOD::CaloRingsContainer> ringerContainer(m_ringerKey, ctx);
    ringerContainer.record( std::unique_ptr<xAOD::CaloRingsContainer>(new xAOD::CaloRingsContainer()));

    SG::WriteHandle<xAOD::CaloClusterContainer> clusterContainer(m_clusterKey, ctx);
    clusterContainer.record( std::unique_ptr<xAOD::CaloClusterContainer>(new xAOD::CaloClusterContainer()));

    SG::WriteHandle<xAOD::ElectronContainer> electronContainer(m_electronKey, ctx);
    electronContainer.record( std::unique_ptr<xAOD::ElectronContainer>(new xAOD::ElectronContainer()));

    int cluster_link = 0;
    xAOD::CaloCluster_t cluster_t;
    for (auto &electron_t: *collection_electrons){
      for (auto &ringer_t : *collection_rings ){
        if (ringer_t.cluster_link == cluster_link && electron_t.cluster_link == cluster_link){
          MSG_DEBUG("Match between cluster and ringer");
          
          xAOD::CaloRings *ringer = nullptr;
          xAOD::CaloRingsConverter cnv;
          cnv.convert(ringer_t, ringer); // alloc memory
          for (auto ring : ringer->rings()) rings->push_back(ring);
          
          cluster_t = collection_cluster->at(cluster_link);
          xAOD::CaloCluster *cluster = nullptr;
          xAOD::CaloClusterConverter clusterCnv;
          if (!clusterCnv.convert(cluster_t, cluster)){
            MSG_INFO("Error converting cluster");
            return StatusCode::FAILURE;
          };

          xAOD::Electron *electron = nullptr;
          xAOD::ElectronConverter electronCnv;
          electronCnv.convert(electron_t, electron);

          eta     = cluster->eta();
          phi     = cluster->phi();
          e       = cluster->e();
          MSG_INFO("Cluster energy: " << e);
          et      = cluster->et();
          deta    = cluster->deltaEta();
          dphi    = cluster->deltaPhi();
          e0      = cluster->e0();
          e1      = cluster->e1();
          e2      = cluster->e2();
          e3      = cluster->e3();
          ehad1   = cluster->ehad1();
          ehad2   = cluster->ehad2();
          ehad3   = cluster->ehad3();
          etot    = cluster->etot();
          e233    = cluster->e233();
          e237    = cluster->e237();
          e277    = cluster->e277();
          emaxs1  = cluster->emaxs1();
          emaxs2  = cluster->emaxs2();
          e2tsts1 = cluster->e2tsts1();
          reta    = cluster->reta();
          rphi    = cluster->rphi();
          rhad    = cluster->rhad();
          rhad1   = cluster->rhad1();
          eratio  = cluster->eratio();
          f0      = cluster->f0();
          f1      = cluster->f1();
          f2      = cluster->f2();
          f3      = cluster->f3();
          weta2   = cluster->weta2();

          secondR = cluster->secondR();
          lambdaCenter = cluster->lambdaCenter();
          secondLambda = cluster->secondLambda();
          fracMax      = cluster->fracMax();
          lateralMom   = cluster->lateralMom();
          longitudinalMom = cluster->longitudinalMom();
          el_fwdTight = computeForwardDecision(cluster,"tight");
          el_fwdMedium = computeForwardDecision(cluster,"medium");
          el_fwdLoose = computeForwardDecision(cluster,"loose");
          
          el_tight = electron->getDecisions().at(0);
          el_medium = electron->getDecisions().at(1);
          el_loose = electron->getDecisions().at(2);
          el_vloose = electron->getDecisions().at(3);

          el_et = electron->et();
          el_eta = electron->eta();
          el_phi = electron->phi();

          // outputTree->Fill();
          break;
        }
      } //each cluster should be 1 entry on ntuple file
    cluster_link++;
    }
  } 

  { // deserialize EventInfo
    InitBranch(outputTree, "eventNumber", &eventNumber);
    InitBranch(outputTree, "avgmu", &avgmu);

    SG::WriteHandle<xAOD::EventInfoContainer> container(m_eventKey, ctx);
    container.record( std::unique_ptr<xAOD::EventInfoContainer>(new xAOD::EventInfoContainer()));
    xAOD::EventInfo  *event=nullptr;
    xAOD::EventInfoConverter cnv;
    cnv.convert(collection_event->at(0), event);
    std::cout<<"EventNumber = " << event->eventNumber() << ", AvgMu = " << event->avgmu()<<std::endl;
    eventNumber = event->eventNumber();
    avgmu = event->avgmu();    

    // if (collection_event == nullptr){
    //   std::cout<<"null pointer"<<std::endl;
    // }
    // else if (!collection_event->empty()){
    //   for (auto &event_t: *collection_event){
    //     std::cout<<event_t.eventNumber<<std::endl;
    //   }
    // }
    // else{
    //   std::cout<<"empty vector"<<std::endl;
    // }

    outputTree->Fill();  //each cluster should be 1 entry on ntulpe fill, only fill ntuple tree once, after filling all branches
  }


  eta          = 0;
  phi          = 0;
  e            = 0;
  et           = 0;
  deta         = 0;
  dphi         = 0;
  e0           = 0;
  e1           = 0;
  e2           = 0;
  e3           = 0;
  ehad1        = 0;
  ehad2        = 0;
  ehad3        = 0;
  etot         = 0;
  e233         = 0;
  e237         = 0;
  e277         = 0;
  emaxs1       = 0;
  emaxs2       = 0;
  e2tsts1      = 0;
  reta         = 0;
  rphi         = 0;
  rhad         = 0;
  rhad1        = 0;
  eratio       = 0;
  f0           = 0;
  f1           = 0;
  f2           = 0;
  f3           = 0;
  weta2        = 0;
  secondR      = 0;
  lambdaCenter = 0;
  secondLambda = 0;
  fracMax      = 0;
  lateralMom   = 0;
  longitudinalMom = 0;
  el_tight     = false;
  el_medium    = false;
  el_loose     = false;
  el_vloose    = false;
  el_fwdTight  = false;
  el_fwdMedium = false;
  el_fwdLoose  = false;
  avgmu        = 0;
  eventNumber  = 0;
  
  delete rings        ;

  delete collection_descriptor;
  delete collection_cells     ;
  delete collection_event     ;
  delete collection_truth     ;
  delete collection_cluster   ;
  delete collection_electrons ;

  return StatusCode::SUCCESS;
 
}

//!=====================================================================

template <class T>
void RootStreamNTUPLEMaker::InitBranch(TTree* fChain, std::string branch_name, T* param) const
{
  std::string bname = branch_name;
  if (fChain->GetAlias(bname.c_str()))
     bname = std::string(fChain->GetAlias(bname.c_str()));

  if (!fChain->FindBranch(bname.c_str()) ) {
    MSG_WARNING( "unknown branch " << bname );
    return;
  }
  fChain->SetBranchStatus(bname.c_str(), 1.);
  fChain->SetBranchAddress(bname.c_str(), param);
}

bool RootStreamNTUPLEMaker::computeForwardDecision(xAOD::CaloCluster* cluster, std::string workingpoint) const {
  if (workingpoint == "loose"){
    if (cluster->secondLambda() > m_secondLambdaCuts[0]) return false;
    if (cluster->lateralMom() > m_lateralMomCuts[0]) return false;
    if (cluster->longitudinalMom() > m_longMomCuts[0]) return false;
    if (cluster->fracMax() < m_fracMaxCuts[0]) return false;
    if (cluster->secondR() > m_secondRCuts[0]) return false;
    if (cluster->lambdaCenter() > m_lambdaCenterCuts[0]) return false;
  return true;
  }

  if (workingpoint == "medium"){
    if (cluster->secondLambda() > m_secondLambdaCuts[1]) return false;
    if (cluster->lateralMom() > m_lateralMomCuts[1]) return false;
    if (cluster->longitudinalMom() > m_longMomCuts[1]) return false;
    if (cluster->fracMax() < m_fracMaxCuts[1]) return false;
    if (cluster->secondR() > m_secondRCuts[1]) return false;
    if (cluster->lambdaCenter() > m_lambdaCenterCuts[1]) return false;
    return true;
  }

  if (workingpoint == "tight"){
    if (cluster->secondLambda() > m_secondLambdaCuts[2]) return false;
    if (cluster->lateralMom() > m_lateralMomCuts[2]) return false;
    if (cluster->longitudinalMom() > m_longMomCuts[2]) return false;
    if (cluster->fracMax() < m_fracMaxCuts[2]) return false;
    if (cluster->secondR() > m_secondRCuts[2]) return false;
    if (cluster->lambdaCenter() > m_lambdaCenterCuts[2]) return false;
    return true;
  }

}