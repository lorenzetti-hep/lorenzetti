/**
 * @class RootStreamNtupleMaker
 * @brief This class is responsible for creating an ntuple from ROOT files.
 *
 * The RootStreamNtupleMaker class reads data from ROOT files and creates an ntuple
 * with the specified branches. It initializes the necessary variables, books the histograms,
 * and fills the ntuple with the data from the ROOT file. It also provides functions for
 * initializing, finalizing, and executing the ntuple creation process.
 */
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
#include "RootStreamNtupleMaker.h"
#include "GaugiKernel/EDM.h"


using namespace SG;
using namespace Gaugi;



RootStreamNtupleMaker::RootStreamNtupleMaker( std::string name ) : 
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
  declareProperty( "OutputNtupleName"   , m_outputNtupleName="events"     );
}

//!=====================================================================

RootStreamNtupleMaker::~RootStreamNtupleMaker()
{}

//!=====================================================================

StatusCode RootStreamNtupleMaker::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNtupleMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNtupleMaker::bookHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  TFile *file = new TFile(m_inputFile.c_str(), "read");
  store->decorate( "events", file );
  store->cd();

  float cl_eta                = 0;
  float cl_phi                = 0;
  float cl_e                  = 0;
  float cl_et                 = 0;
  float cl_deta               = 0;
  float cl_dphi               = 0;
  float cl_e0                 = 0;
  float cl_e1                 = 0;
  float cl_e2                 = 0;
  float cl_e3                 = 0;
  float cl_ehad1              = 0;
  float cl_ehad2              = 0;
  float cl_ehad3              = 0;
  float cl_etot               = 0;
  float cl_e233               = 0;
  float cl_e237               = 0;
  float cl_e277               = 0;
  float cl_emaxs1             = 0;
  float cl_emaxs2             = 0;
  float cl_e2tsts1            = 0;
  float cl_reta               = 0;
  float cl_rphi               = 0;
  float cl_rhad               = 0;
  float cl_rhad1              = 0;
  float cl_eratio             = 0;
  float cl_f0                 = 0;
  float cl_f1                 = 0;
  float cl_f2                 = 0;
  float cl_f3                 = 0;
  float cl_weta2              = 0;
  float cl_secondR            = 0;
  float cl_lambdaCenter       = 0;
  float cl_secondLambda       = 0;
  float cl_fracMax            = 0;
  float cl_lateralMom         = 0;
  float cl_longitudinalMom    = 0;
  std::vector<float>*cl_rings = nullptr;
  float el_eta                = -1;
  float el_et                 = -1;
  float el_phi                = -1;
  bool el_tight               = false;
  bool el_medium              = false;
  bool el_loose               = false;
  bool el_vloose              = false;

 
  int eventNumber = -1;
  int runNumber   = -1;
  float avgmu     = -1;


  TTree *tree = new TTree(m_outputNtupleName.c_str(), "");

  tree->Branch("EventNumber"            , &eventNumber);
  tree->Branch("RunNumber"              , &runNumber);  
  tree->Branch("avgmu"                  , &avgmu);  
  tree->Branch("cl_eta"                 , &cl_eta);
  tree->Branch("cl_phi"                 , &cl_phi);
  tree->Branch("cl_e"                   , &cl_e);
  tree->Branch("cl_et"                  , &cl_et);
  tree->Branch("cl_deta"                , &cl_deta);
  tree->Branch("cl_dphi"                , &cl_dphi);
  tree->Branch("cl_e0"                  , &cl_e0);
  tree->Branch("cl_e1"                  , &cl_e1);
  tree->Branch("cl_e2"                  , &cl_e2);
  tree->Branch("cl_e3"                  , &cl_e3);
  tree->Branch("cl_ehad1"               , &cl_ehad1);
  tree->Branch("cl_ehad2"               , &cl_ehad2);
  tree->Branch("cl_ehad3"               , &cl_ehad3);
  tree->Branch("cl_etot"                , &cl_etot);
  tree->Branch("cl_e233"                , &cl_e233);
  tree->Branch("cl_e237"                , &cl_e237);
  tree->Branch("cl_e277"                , &cl_e277);
  tree->Branch("cl_emaxs1"              , &cl_emaxs1);
  tree->Branch("cl_emaxs2"              , &cl_emaxs2);
  tree->Branch("cl_e2tsts1"             , &cl_e2tsts1);
  tree->Branch("cl_reta"                , &cl_reta);
  tree->Branch("cl_rphi"                , &cl_rphi);
  tree->Branch("cl_rhad"                , &cl_rhad);
  tree->Branch("cl_rhad1"               , &cl_rhad1);
  tree->Branch("cl_eratio"              , &cl_eratio);
  tree->Branch("cl_f0"                  , &cl_f0);
  tree->Branch("cl_f1"                  , &cl_f1);
  tree->Branch("cl_f2"                  , &cl_f2);
  tree->Branch("cl_f3"                  , &cl_f3);
  tree->Branch("cl_weta2"               , &cl_weta2);
  tree->Branch("cl_rings"               , &cl_rings);
  tree->Branch("cl_secondR"             , &cl_secondR);
  tree->Branch("cl_lambdaCenter"        , &cl_lambdaCenter);
  tree->Branch("cl_secondLambda"        , &cl_secondLambda);
  tree->Branch("cl_fracMax"             , &cl_fracMax);
  tree->Branch("cl_lateralMom"          , &cl_lateralMom);
  tree->Branch("cl_longitudinalMom"     , &cl_longitudinalMom);
  tree->Branch("el_eta"                 , &el_eta);
  tree->Branch("el_et"                  , &el_et);
  tree->Branch("el_phi"                 , &el_phi);
  tree->Branch("el_tight"               , &el_tight);
  tree->Branch("el_medium"              , &el_medium);
  tree->Branch("el_loose"               , &el_loose);
  tree->Branch("el_vloose"              , &el_vloose);

  store->add(tree);
  return StatusCode::SUCCESS; 
}

//!=====================================================================

StatusCode RootStreamNtupleMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNtupleMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNtupleMaker::execute( EventContext &ctx, int evt ) const
{
  return StatusCode::SUCCESS;
}

//!===================================    ==================================

StatusCode RootStreamNtupleMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode RootStreamNtupleMaker::fillHistograms( EventContext &ctx ) const
{

  SG::ReadHandle<xAOD::EventInfoContainer> event_container( m_eventKey, ctx );
  if( !event_container.isValid() )
  {
    MSG_FATAL("It's not possible to read the xAOD::EventInfoContainer from this Context using this key " << m_eventKey );
  }

  SG::ReadHandle<xAOD::CaloRingsContainer> ringer_container( m_ringerKey, ctx );
  if( !ringer_container.isValid() )
  {
    MSG_FATAL("It's not possible to read the xAOD::CaloRingsContainer from this Context using this key " << m_ringerKey );
  }

  SG::ReadHandle<xAOD::ElectronContainer> electron_container( m_electronKey, ctx );
  if( !electron_container.isValid() )
  {
    MSG_FATAL("It's not possible to read the xAOD::ElectronContainer from this Context using this key " << m_electronKey );
  }

  auto store = ctx.getStoreGateSvc();
  TTree *tree = store->tree(m_outputNtupleName.c_str());

  MSG_INFO( "Link all branches..." );

  int   runNumber          = 0;
  int   eventNumber        = 0;
  float avgmu              = 0;  
  float cl_eta             = 0;
  float cl_phi             = 0;
  float cl_e               = 0;
  float cl_et              = 0;
  float cl_deta            = 0;
  float cl_dphi            = 0;
  float cl_e0              = 0;
  float cl_e1              = 0;
  float cl_e2              = 0;
  float cl_e3              = 0;
  float cl_ehad1           = 0;
  float cl_ehad2           = 0;
  float cl_ehad3           = 0;
  float cl_etot            = 0;
  float cl_e233            = 0;
  float cl_e237            = 0;
  float cl_e277            = 0;
  float cl_emaxs1          = 0;
  float cl_emaxs2          = 0;
  float cl_e2tsts1         = 0;
  float cl_reta            = 0;
  float cl_rphi            = 0;
  float cl_rhad            = 0;
  float cl_rhad1           = 0;
  float cl_eratio          = 0;
  float cl_f0              = 0;
  float cl_f1              = 0;
  float cl_f2              = 0;
  float cl_f3              = 0;
  float cl_weta2           = 0;
  float cl_secondR         = 0;
  float cl_lambdaCenter    = 0;
  float cl_secondLambda    = 0;
  float cl_fracMax         = 0;
  float cl_lateralMom      = 0;
  float cl_longitudinalMom = 0;
  float el_eta             = -1;
  float el_et              = -1;
  float el_phi             = -1;
  bool el_tight            = false;
  bool el_medium           = false;
  bool el_loose            = false;
  bool el_vloose           = false;
  std::vector<float> *cl_rings = nullptr;

  InitBranch( tree, "avgmu"                   , &avgmu             );
  InitBranch( tree, "event_number"            , &eventNumber       );
  InitBranch( tree, "run_number"              , &runNumber         );
  InitBranch( tree, "cl_eta"                  , &cl_eta            );
  InitBranch( tree, "cl_phi"                  , &cl_phi            );
  InitBranch( tree, "cl_e"                    , &cl_e              );
  InitBranch( tree, "cl_et"                   , &cl_et             );
  InitBranch( tree, "cl_deta"                 , &cl_deta           );
  InitBranch( tree, "cl_dphi"                 , &cl_dphi           );
  InitBranch( tree, "cl_e0"                   , &cl_e0             );
  InitBranch( tree, "cl_e1"                   , &cl_e1             );
  InitBranch( tree, "cl_e2"                   , &cl_e2             );
  InitBranch( tree, "cl_e3"                   , &cl_e3             );
  InitBranch( tree, "cl_ehad1"                , &cl_ehad1          );
  InitBranch( tree, "cl_ehad2"                , &cl_ehad2          );
  InitBranch( tree, "cl_ehad3"                , &cl_ehad3          );
  InitBranch( tree, "cl_etot"                 , &cl_etot           );
  InitBranch( tree, "cl_e233"                 , &cl_e233           );
  InitBranch( tree, "cl_e237"                 , &cl_e237           );
  InitBranch( tree, "cl_e277"                 , &cl_e277           );
  InitBranch( tree, "cl_emaxs1"               , &cl_emaxs1         );
  InitBranch( tree, "cl_emaxs2"               , &cl_emaxs2         );
  InitBranch( tree, "cl_e2tsts1"              , &cl_e2tsts1        );
  InitBranch( tree, "cl_reta"                 , &cl_reta           );
  InitBranch( tree, "cl_rphi"                 , &cl_rphi           );
  InitBranch( tree, "cl_rhad"                 , &cl_rhad           );
  InitBranch( tree, "cl_rhad1"                , &cl_rhad1          );
  InitBranch( tree, "cl_eratio"               , &cl_eratio         );
  InitBranch( tree, "cl_f0"                   , &cl_f0             );
  InitBranch( tree, "cl_f1"                   , &cl_f1             );
  InitBranch( tree, "cl_f2"                   , &cl_f2             );
  InitBranch( tree, "cl_f3"                   , &cl_f3             );
  InitBranch( tree, "cl_weta2"                , &cl_weta2          );
  InitBranch( tree, "cl_secondR"              , &cl_secondR        );
  InitBranch( tree, "cl_lambdaCenter"         , &cl_lambdaCenter   );
  InitBranch( tree, "cl_secondLambda"         , &cl_secondLambda   );
  InitBranch( tree, "cl_fracMax"              , &cl_fracMax        );
  InitBranch( tree, "cl_lateralMom"           , &cl_lateralMom     );
  InitBranch( tree, "cl_longitudinalMom"      , &cl_longitudinalMom);
  InitBranch( tree, "rings"                   , &cl_rings          );
  InitBranch( tree, "el_eta"                  , &el_eta            );
  InitBranch( tree, "el_et"                   , &el_et             );
  InitBranch( tree, "el_phi"                  , &el_phi            );
  InitBranch( tree, "el_tight"                , &el_tight          );
  InitBranch( tree, "el_medium"               , &el_medium         );
  InitBranch( tree, "el_loose"                , &el_loose          );
  InitBranch( tree, "el_vloose"               , &el_vloose         );    


  auto evt = (**event_container.ptr()).front();


  for (auto el : **electron_container.ptr() )
  
  {
    auto clus = el->caloCluster();


    const xAOD::CaloRings* cl_ringer = nullptr;
    for (auto ring : **ringer_container.ptr() )
    {
      if (ring->caloCluster() == clus)
      {
        cl_ringer = ring;
        break;
      } 
    }

    if(!cl_ringer)
    {
      MSG_WARNING("No rings found for cluster");
      continue;
    }


    avgmu              = evt->averageInteractionsPerCrossing();
    runNumber          = evt->runNumber();
    eventNumber        = evt->eventNumber();
    cl_eta             = clus->eta();
    cl_phi             = clus->phi();
    cl_e               = clus->e();
    MSG_INFO("Cluster energy: " << cl_e);
    cl_et              = clus->et();
    cl_deta            = clus->deltaEta();
    cl_dphi            = clus->deltaPhi();
    cl_e0              = clus->e0();
    cl_e1              = clus->e1();
    cl_e2              = clus->e2();
    cl_e3              = clus->e3();
    cl_ehad1           = clus->ehad1();
    cl_ehad2           = clus->ehad2();
    cl_ehad3           = clus->ehad3();
    cl_etot            = clus->etot();
    cl_e233            = clus->e233();
    cl_e237            = clus->e237();
    cl_e277            = clus->e277();
    cl_emaxs1          = clus->emaxs1();
    cl_emaxs2          = clus->emaxs2();
    cl_e2tsts1         = clus->e2tsts1();
    cl_reta            = clus->reta();
    cl_rphi            = clus->rphi();
    cl_rhad            = clus->rhad();
    cl_rhad1           = clus->rhad1();
    cl_eratio          = clus->eratio();
    cl_f0              = clus->f0();
    cl_f1              = clus->f1();
    cl_f2              = clus->f2();
    cl_f3              = clus->f3();
    cl_weta2           = clus->weta2();
    cl_secondR         = clus->secondR();
    cl_lambdaCenter    = clus->lambdaCenter();
    cl_secondLambda    = clus->secondLambda();
    cl_fracMax         = clus->fracMax();
    cl_lateralMom      = clus->lateralMom();
    cl_longitudinalMom = clus->longitudinalMom();
    for (auto ring : cl_ringer->rings()) cl_rings->push_back(ring);
    el_tight           = el->isTight();
    el_medium          = el->isMedium();
    el_loose           = el->isLoose();
    el_vloose          = el->isVeryLoose(); 
    el_et              = el->et();
    el_eta             = el->eta();
    el_phi             = el->phi();
    tree->Fill();
         
  } //each cluster should be 1 entry on ntuple file
   

  return StatusCode::SUCCESS;
}

//!=====================================================================

template <class T>
void RootStreamNtupleMaker::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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

