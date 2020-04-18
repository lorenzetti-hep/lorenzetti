
#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloRings/CaloRingsContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "TTree.h"
#include "CaloNtupleMaker.h"
#include "TMath.h"

using namespace SG;
using namespace CaloSampling;
using namespace Gaugi;





CaloNtupleMaker::CaloNtupleMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "EventKey"       , m_eventKey="EventInfo"            );
  declareProperty( "ClusterKey"     , m_clusterKey="Clusters"           );
  declareProperty( "TruthClusterKey", m_truthClusterKey="TruthClusters" );
  declareProperty( "RingerKey"      , m_ringerKey="TruthRinger"         );
  declareProperty( "TruthRingerKey" , m_truthRingerKey="TruthRinger"    );
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
  declareProperty( "DeltaR"         , m_deltaR=0.15                     );
}



CaloNtupleMaker::~CaloNtupleMaker()
{;}


StatusCode CaloNtupleMaker::initialize()
{
  return StatusCode::SUCCESS;
}


StatusCode CaloNtupleMaker::bookHistograms( StoreGate &store ) const
{
  // Create all local variables since this must be a const method
  float seed_eta       = -1;
  float seed_phi       = -1;
  float seed_et        = -1;
  bool  mc_cl_match    = false;
  float mc_cl_eta      = -1;
  float mc_cl_phi      = -1;
  float mc_cl_et       = -1;
  float mc_cl_e1       = -1;
  float mc_cl_e2       = -1;
  float mc_cl_e3       = -1;
  float mc_cl_ehad1    = -1;
  float mc_cl_ehad2    = -1;
  float mc_cl_ehad3    = -1;
  float mc_cl_etot     = -1;
  float mc_cl_reta     = -1;
  float mc_cl_rphi     = -1;
  float mc_cl_rhad     = -1;
  float mc_cl_eratio   = -1;
  float mc_cl_f0       = -1;
  float mc_cl_f1       = -1;
  float mc_cl_f2       = -1;
  float mc_cl_f3       = -1;
  float mc_cl_weta2    = -1;
  float mc_cl_e233     = -1;
  float mc_cl_e237     = -1;
  float mc_cl_e277     = -1;
  float mc_cl_emaxs1   = -1;
  float mc_cl_e2tsts1  = -1;
  bool  mc_cl_ringer_match=false;
  std::vector<float> mc_cl_rings;
  bool  cl_match       = false;
  float cl_eta         = -1;
  float cl_phi         = -1;
  float cl_et          = -1;
  float cl_e1          = -1;
  float cl_e2          = -1;
  float cl_e3          = -1;
  float cl_ehad1       = -1;
  float cl_ehad2       = -1;
  float cl_ehad3       = -1;
  float cl_etot        = -1;
  float cl_reta        = -1;
  float cl_rphi        = -1;
  float cl_rhad        = -1;
  float cl_eratio      = -1;
  float cl_f0          = -1;
  float cl_f1          = -1;
  float cl_f2          = -1;
  float cl_f3          = -1;
  float cl_weta2       = -1;
  float cl_e233        = -1;
  float cl_e237        = -1;
  float cl_e277        = -1;
  float cl_emaxs1      = -1;
  float cl_e2tsts1     = -1;
  bool  cl_ringer_match=false;
  std::vector<float> cl_rings;

  store.cd();
  TTree *tree = new TTree("collection", "");
  
  tree->Branch(  "seed_eta"      , &seed_eta       );
  tree->Branch(  "seed_phi"      , &seed_phi       );
  tree->Branch(  "seed_et"       , &seed_et        );

  tree->Branch(  "mc_cl_match"   , &mc_cl_match    );
  tree->Branch(  "mc_cl_eta"     , &mc_cl_eta      );
  tree->Branch(  "mc_cl_phi"     , &mc_cl_phi      );
  tree->Branch(  "mc_cl_et"      , &mc_cl_et       );
  tree->Branch(  "mc_cl_e1"      , &mc_cl_e1       );
  tree->Branch(  "mc_cl_e2"      , &mc_cl_e2       );
  tree->Branch(  "mc_cl_e3"      , &mc_cl_e3       );
  tree->Branch(  "mc_cl_ehad1"   , &mc_cl_ehad1    );
  tree->Branch(  "mc_cl_ehad2"   , &mc_cl_ehad2    );
  tree->Branch(  "mc_cl_ehad3"   , &mc_cl_ehad3    );
  tree->Branch(  "mc_cl_etot"    , &mc_cl_etot     );
  tree->Branch(  "mc_cl_reta"    , &mc_cl_reta     );
  tree->Branch(  "mc_cl_rphi"    , &mc_cl_rphi     );
  tree->Branch(  "mc_cl_rhad"    , &mc_cl_rhad     );
  tree->Branch(  "mc_cl_eratio"  , &mc_cl_eratio   );
  tree->Branch(  "mc_cl_f0"      , &mc_cl_f0       );
  tree->Branch(  "mc_cl_f1"      , &mc_cl_f1       );
  tree->Branch(  "mc_cl_f2"      , &mc_cl_f2       );
  tree->Branch(  "mc_cl_f3"      , &mc_cl_f3       );
  tree->Branch(  "mc_cl_weta2"   , &mc_cl_weta2    );
  tree->Branch(  "mc_cl_e233"    , &mc_cl_e233     );
  tree->Branch(  "mc_cl_e237"    , &mc_cl_e237     );
  tree->Branch(  "mc_cl_e277"    , &mc_cl_e277     );
  tree->Branch(  "mc_cl_emaxs1"  , &mc_cl_emaxs1   );
  tree->Branch(  "mc_cl_e2tsts1" , &mc_cl_e2tsts1  );
  tree->Branch(  "mc_cl_ringer_match", &mc_cl_ringer_match  );
  tree->Branch(  "mc_cl_rings"   , &mc_cl_rings    );

  tree->Branch(  "cl_match"      , &cl_match       );
  tree->Branch(  "cl_eta"        , &cl_eta         );
  tree->Branch(  "cl_phi"        , &cl_phi         );
  tree->Branch(  "cl_et"         , &cl_et          );
  tree->Branch(  "cl_e1"         , &cl_e1          );
  tree->Branch(  "cl_e2"         , &cl_e2          );
  tree->Branch(  "cl_e3"         , &cl_e3          );
  tree->Branch(  "cl_ehad1"      , &cl_ehad1       );
  tree->Branch(  "cl_ehad2"      , &cl_ehad2       );
  tree->Branch(  "cl_ehad3"      , &cl_ehad3       );
  tree->Branch(  "cl_etot"       , &cl_etot        );
  tree->Branch(  "cl_reta"       , &cl_reta        );
  tree->Branch(  "cl_rphi"       , &cl_rphi        );
  tree->Branch(  "cl_rhad"       , &cl_rhad        );
  tree->Branch(  "cl_eratio"     , &cl_eratio      );
  tree->Branch(  "cl_f0"         , &cl_f0          );
  tree->Branch(  "cl_f1"         , &cl_f1          );
  tree->Branch(  "cl_f2"         , &cl_f2          );
  tree->Branch(  "cl_f3"         , &cl_f3          );
  tree->Branch(  "cl_weta2"      , &cl_weta2       );
  tree->Branch(  "cl_e233"       , &cl_e233        );
  tree->Branch(  "cl_e237"       , &cl_e237        );
  tree->Branch(  "cl_e277"       , &cl_e277        );
  tree->Branch(  "cl_emaxs1"     , &cl_emaxs1      );
  tree->Branch(  "cl_e2tsts1"    , &cl_e2tsts1     );
  tree->Branch(  "cl_ringer_match", &cl_ringer_match  );
  tree->Branch(  "cl_rings"       , &cl_rings    );


  store.add( tree );
  
  return StatusCode::SUCCESS;
}


StatusCode CaloNtupleMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloNtupleMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloNtupleMaker::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode CaloNtupleMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}





StatusCode CaloNtupleMaker::fillHistograms( EventContext &ctx , StoreGate &store ) const
{
  // Event info
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);

  if( !event.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
  }

  store.cd();
  TTree *tree = store.tree("collection");
 
  for ( auto& seed : (**event.ptr()).front()->allSeeds() ){
    MSG_INFO( "Fill this seed into the collection tree." );
    Fill( ctx, tree, seed );
  }
  
  return StatusCode::SUCCESS;
}


template <class T>
void CaloNtupleMaker::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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


void CaloNtupleMaker::Fill( EventContext &ctx , TTree *tree, xAOD::seed_t seed ) const
{
  // Create all local variables since this must be a const method
  float seed_eta       = 0;
  float seed_phi       = 0;
  float seed_et        = 0;
  bool  mc_cl_match    = false;
  float mc_cl_eta      = 0;
  float mc_cl_phi      = 0;
  float mc_cl_et       = 0;
  float mc_cl_e1       = 0;
  float mc_cl_e2       = 0;
  float mc_cl_e3       = 0;
  float mc_cl_ehad1    = 0;
  float mc_cl_ehad2    = 0;
  float mc_cl_ehad3    = 0;
  float mc_cl_etot     = 0;
  float mc_cl_reta     = 0;
  float mc_cl_rphi     = 0;
  float mc_cl_rhad     = 0;
  float mc_cl_eratio   = 0;
  float mc_cl_f0       = 0;
  float mc_cl_f1       = 0;
  float mc_cl_f2       = 0;
  float mc_cl_f3       = 0;
  float mc_cl_weta2    = 0;
  float mc_cl_e233     = 0;
  float mc_cl_e237     = 0;
  float mc_cl_e277     = 0;
  float mc_cl_emaxs1   = 0;
  float mc_cl_e2tsts1  = 0;
  bool  mc_cl_ringer_match = false;
  std::vector<float> *mc_cl_rings;
  bool  cl_match       = false;
  float cl_eta         = 0;
  float cl_phi         = 0;
  float cl_et          = 0;
  float cl_e1          = 0;
  float cl_e2          = 0;
  float cl_e3          = 0;
  float cl_ehad1       = 0;
  float cl_ehad2       = 0;
  float cl_ehad3       = 0;
  float cl_etot        = 0;
  float cl_reta        = 0;
  float cl_rphi        = 0;
  float cl_rhad        = 0;
  float cl_eratio      = 0;
  float cl_f0          = 0;
  float cl_f1          = 0;
  float cl_f2          = 0;
  float cl_f3          = 0;
  float cl_weta2       = 0;
  float cl_e233        = 0;
  float cl_e237        = 0;
  float cl_e277        = 0;
  float cl_emaxs1      = 0;
  float cl_e2tsts1     = 0;
  bool  cl_ringer_match=false;
  std::vector<float> *cl_rings;


  InitBranch( tree,  "seed_eta"      , &seed_eta       );
  InitBranch( tree,  "seed_phi"      , &seed_phi       );
  InitBranch( tree,  "seed_et"       , &seed_et        );
  InitBranch( tree,  "mc_cl_match"   , &mc_cl_match    );
  InitBranch( tree,  "mc_cl_eta"     , &mc_cl_eta      );
  InitBranch( tree,  "mc_cl_phi"     , &mc_cl_phi      );
  InitBranch( tree,  "mc_cl_et"      , &mc_cl_et       );
  InitBranch( tree,  "mc_cl_e1"      , &mc_cl_e1       );
  InitBranch( tree,  "mc_cl_e2"      , &mc_cl_e2       );
  InitBranch( tree,  "mc_cl_e3"      , &mc_cl_e3       );
  InitBranch( tree,  "mc_cl_ehad1"   , &mc_cl_ehad1    );
  InitBranch( tree,  "mc_cl_ehad2"   , &mc_cl_ehad2    );
  InitBranch( tree,  "mc_cl_ehad3"   , &mc_cl_ehad3    );
  InitBranch( tree,  "mc_cl_etot"    , &mc_cl_etot     );
  InitBranch( tree,  "mc_cl_reta"    , &mc_cl_reta     );
  InitBranch( tree,  "mc_cl_rphi"    , &mc_cl_rphi     );
  InitBranch( tree,  "mc_cl_rhad"    , &mc_cl_rhad     );
  InitBranch( tree,  "mc_cl_eratio"  , &mc_cl_eratio   );
  InitBranch( tree,  "mc_cl_f0"      , &mc_cl_f0       );
  InitBranch( tree,  "mc_cl_f1"      , &mc_cl_f1       );
  InitBranch( tree,  "mc_cl_f2"      , &mc_cl_f2       );
  InitBranch( tree,  "mc_cl_f3"      , &mc_cl_f3       );
  InitBranch( tree,  "mc_cl_weta2"   , &mc_cl_weta2    );
  InitBranch( tree,  "mc_cl_e233"    , &mc_cl_e233     );
  InitBranch( tree,  "mc_cl_e237"    , &mc_cl_e237     );
  InitBranch( tree,  "mc_cl_e277"    , &mc_cl_e277     );
  InitBranch( tree,  "mc_cl_emaxs1"  , &mc_cl_emaxs1   );
  InitBranch( tree,  "mc_cl_e2tsts1" , &mc_cl_e2tsts1  );
  InitBranch( tree,  "mc_cl_ringer_match", &mc_cl_ringer_match);
  InitBranch( tree,  "mc_cl_rings"       , &mc_cl_rings       );
  InitBranch( tree,  "cl_match"      , &cl_match       );
  InitBranch( tree,  "cl_eta"        , &cl_eta         );
  InitBranch( tree,  "cl_phi"        , &cl_phi         );
  InitBranch( tree,  "cl_et"         , &cl_et          );
  InitBranch( tree,  "cl_e1"         , &cl_e1          );
  InitBranch( tree,  "cl_e2"         , &cl_e2          );
  InitBranch( tree,  "cl_e3"         , &cl_e3          );
  InitBranch( tree,  "cl_ehad1"      , &cl_ehad1       );
  InitBranch( tree,  "cl_ehad2"      , &cl_ehad2       );
  InitBranch( tree,  "cl_ehad3"      , &cl_ehad3       );
  InitBranch( tree,  "cl_etot"       , &cl_etot        );
  InitBranch( tree,  "cl_reta"       , &cl_reta        );
  InitBranch( tree,  "cl_rphi"       , &cl_rphi        );
  InitBranch( tree,  "cl_rhad"       , &cl_rhad        );
  InitBranch( tree,  "cl_eratio"     , &cl_eratio      );
  InitBranch( tree,  "cl_f0"         , &cl_f0          );
  InitBranch( tree,  "cl_f1"         , &cl_f1          );
  InitBranch( tree,  "cl_f2"         , &cl_f2          );
  InitBranch( tree,  "cl_f3"         , &cl_f3          );
  InitBranch( tree,  "cl_weta2"      , &cl_weta2       );
  InitBranch( tree,  "cl_e233"       , &cl_e233        );
  InitBranch( tree,  "cl_e237"       , &cl_e237        );
  InitBranch( tree,  "cl_e277"       , &cl_e277        );
  InitBranch( tree,  "cl_emaxs1"     , &cl_emaxs1      );
  InitBranch( tree,  "cl_e2tsts1"    , &cl_e2tsts1     );
  InitBranch( tree,  "cl_ringer_match", &cl_ringer_match);
  InitBranch( tree,  "cl_rings"       , &cl_rings)     ;

  MSG_INFO( "Link all branches..." );

  seed_eta = seed.eta;
  seed_phi = seed.phi;
  seed_et  = seed.et * 1.e3; // in MeV

  { // Fill all truth values
    const xAOD::CaloCluster *clus=nullptr;
    if( match( ctx, m_truthClusterKey, seed, clus ) )
    {
      mc_cl_match   =  true;
      mc_cl_eta     =  clus->eta()    ;
      mc_cl_phi     =  clus->phi()    ;
      mc_cl_et      =  clus->et()     ;
      mc_cl_e1      =  clus->e1()     ;
      mc_cl_e2      =  clus->e2()     ;
      mc_cl_e3      =  clus->e3()     ;
      mc_cl_ehad1   =  clus->ehad1()  ;
      mc_cl_ehad2   =  clus->ehad2()  ;
      mc_cl_ehad3   =  clus->ehad3()  ;
      mc_cl_etot    =  clus->etot()   ;
      mc_cl_reta    =  clus->reta()   ;
      mc_cl_rphi    =  clus->rphi()   ;
      mc_cl_rhad    =  clus->rhad()   ;
      mc_cl_eratio  =  clus->eratio() ;
      mc_cl_f0      =  clus->f0()     ;
      mc_cl_f1      =  clus->f1()     ;
      mc_cl_f2      =  clus->f2()     ;
      mc_cl_f3      =  clus->f3()     ;
      mc_cl_weta2   =  clus->weta2()  ;
      mc_cl_e233    =  clus->e233()   ;
      mc_cl_e237    =  clus->e237()   ;
      mc_cl_e277    =  clus->e277()   ;
      mc_cl_emaxs1  =  clus->emaxs1() ;
      mc_cl_e2tsts1 =  clus->e2tsts1();


      //fillTruthCells(clus);
      const xAOD::CaloRings *ringer=nullptr;
      if( match( ctx, m_truthRingerKey, clus, ringer ) )
      {
        MSG_INFO( "Ringer mathc!" );
        mc_cl_ringer_match = true;
        auto vec = ringer->rings();
        mc_cl_rings->assign( vec.begin(), vec.end() );
      }
    }
  }


  { // Fill all reco values
    const xAOD::CaloCluster *clus=nullptr;
    if( match( ctx, m_clusterKey, seed, clus ) )
    {
      cl_match   =  true;
      cl_eta     =  clus->eta()    ;
      cl_phi     =  clus->phi()    ;
      cl_et      =  clus->et()     ;
      cl_e1      =  clus->e1()     ;
      cl_e2      =  clus->e2()     ;
      cl_e3      =  clus->e3()     ;
      cl_ehad1   =  clus->ehad1()  ;
      cl_ehad2   =  clus->ehad2()  ;
      cl_ehad3   =  clus->ehad3()  ;
      cl_etot    =  clus->etot()   ;
      cl_reta    =  clus->reta()   ;
      cl_rphi    =  clus->rphi()   ;
      cl_rhad    =  clus->rhad()   ;
      cl_eratio  =  clus->eratio() ;
      cl_f0      =  clus->f0()     ;
      cl_f1      =  clus->f1()     ;
      cl_f2      =  clus->f2()     ;
      cl_f3      =  clus->f3()     ;
      cl_weta2   =  clus->weta2()  ;
      cl_e233    =  clus->e233()   ;
      cl_e237    =  clus->e237()   ;
      cl_e277    =  clus->e277()   ;
      cl_emaxs1  =  clus->emaxs1() ;
      cl_e2tsts1 =  clus->e2tsts1();
      
      //fillTruthCells(clus);
      const xAOD::CaloRings *ringer=nullptr;
      if( match( ctx, m_ringerKey, clus, ringer ) )
      {
        cl_ringer_match = true;
        auto vec = ringer->rings();
        cl_rings->assign( vec.begin(), vec.end() );
      }
    }
  }

  tree->Fill();
}












float CaloNtupleMaker::dR( float eta1, float phi1, float eta2, float phi2 ) const
{
  float deta = fabs(eta1-eta2);
  float dphi = fabs(phi1-phi2)  < TMath::Pi() ? fabs(phi1-phi2) : 2*TMath::Pi() - fabs(phi1-phi2) ;
  return sqrt( deta*deta + dphi*dphi);
}


bool CaloNtupleMaker::match( EventContext &ctx , std::string key, xAOD::seed_t seed, const xAOD::CaloCluster *&cluster )  const
{
  SG::ReadHandle<xAOD::CaloClusterContainer> container( key, ctx );
  
  if( !container.isValid() )
  {
    MSG_WARNING("It's not possible to read the xAOD::CaloClusterContainer from this Contaxt using this key " << key );
    return false;
  }

  for (const auto clus : **container.ptr() ){
    if( dR( seed.eta, seed.phi, clus->eta(), clus->phi() ) < m_deltaR ){
      cluster=clus;
      return true;
    }
  }
  return false;
}


bool CaloNtupleMaker::match( EventContext &ctx , std::string key, const xAOD::CaloCluster *cluster, const xAOD::CaloRings *&ringer ) const
{
  SG::ReadHandle<xAOD::CaloRingsContainer> container( key, ctx );
  
  if( !container.isValid() )
  {
    MSG_WARNING("It's not possible to read the xAOD::CaloRingsContainer from this Contaxt using this key " << key );
    return false;
  }

  MSG_INFO( "CLUSTER == " << cluster );
  for (const auto rings : **container.ptr() ){ 
    MSG_INFO( "Ringer == " << rings->caloCluster() );
    if ( rings->caloCluster() == cluster ){
      MSG_INFO( "Match!" );
      ringer= rings;
      return true;
    }
  }
  return false;
}









