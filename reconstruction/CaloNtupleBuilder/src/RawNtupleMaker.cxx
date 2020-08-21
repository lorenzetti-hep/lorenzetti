#include "G4Kernel/CaloPhiRange.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloRings/CaloRingsContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "TruthParticle/TruthParticleContainer.h"
#include "TTree.h"
#include "RawNtupleMaker.h"
#include "TMath.h"

using namespace SG;
using namespace CaloSampling;
using namespace Gaugi;



RawNtupleMaker::RawNtupleMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "EventKey"       , m_eventKey="EventInfo"            );
  declareProperty( "CellsKey"       , m_cellsKey="Cells"                );
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
  declareProperty( "NtupleName"     , m_ntupleName="raw_events"         );
  declareProperty( "EtaWindow"      , m_etaWindow=0.4                   );
  declareProperty( "PhiWindow"      , m_phiWindow=0.4                   );
}



RawNtupleMaker::~RawNtupleMaker()
{;}


StatusCode RawNtupleMaker::initialize()
{
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  // Create all local variables since this must be a const method
  int eventNumber         = -1;
  float avgmu             = -1;
  float seed_eta          = -1;
  float seed_phi          = -1;
  std::vector<raw_cell_t> cells;
 
  store->cd();
  TTree *tree = new TTree( m_ntupleName.c_str(), "");
  
  tree->Branch(  "EventNumber"        , &eventNumber        );
  tree->Branch(  "avgmu"              , &avgmu              );
  tree->Branch(  "seed_eta"           , &seed_eta           );
  tree->Branch(  "seed_phi"           , &seed_phi           );
  tree->Branch(  "cells",               &cells              );

  store->add( tree );
  
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::post_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::fillHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  store->cd();
  TTree *tree = store->tree(m_ntupleName);

  Fill( ctx, tree );

  return StatusCode::SUCCESS;
}



template <class T>
void RawNtupleMaker::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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


void RawNtupleMaker::Fill( EventContext &ctx , TTree *tree  ) const
{
  // Create all local variables since this must be a const method
  int   eventNumber    ;
  float avgmu          ;
  float seed_eta       ;
  float seed_phi       ;

  std::vector<raw_cell_t> *cells = nullptr;

  InitBranch( tree,  "EventNumber"        , &eventNumber        );
  InitBranch( tree,  "avgmu"              , &avgmu              );
  InitBranch( tree,  "seed_eta"           , &seed_eta           );
  InitBranch( tree,  "seed_phi"           , &seed_phi           );
  InitBranch( tree,  "cells"              , &cells              );
  
  MSG_DEBUG( "Link all branches..." );


  // Event info
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);

  if( !event.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
  }

  SG::ReadHandle<xAOD::CaloCellContainer> container(m_cellsKey, ctx);

  if( !container.isValid() ){
    MSG_WARNING( "It's not possible to read the xAOD::CaloCellContainer from this Context using this key: " << m_cellsKey );
    return;
  }


  auto seeds = (**event.ptr()).front()->allSeeds() ;

  for ( auto& seed : seeds ){
  
    cells->clear();
    avgmu = (**event.ptr()).front()->avgmu();
    eventNumber = (**event.ptr()).front()->eventNumber();

    seed_eta = seed.eta;
    seed_phi = seed.phi;


    for ( int layer=1; layer<=6; ++layer )
    {
      float min_deltaR=999;
      const xAOD::CaloCell* closest_cell=nullptr;

      for ( const auto cell : **container.ptr() )
      {
        if ( (int)cell->layer() ==  layer )
        { 
          float deltaR = dR( cell->eta(), cell->phi(), seed.eta, seed.phi );
          if ( deltaR < min_deltaR)
          {
            min_deltaR=deltaR;  
            closest_cell = cell;
          }
        }          
      }

      if (closest_cell )
      {
        auto raw = closest_cell->parent();
        
        std::vector< std::vector<float> > pulsePerBunch;
        for (int bc=raw->bcid_start() ; bc<=raw->bcid_end() ; ++bc)
          pulsePerBunch.push_back(raw->pulse(bc));


        raw_cell_t obj{ closest_cell->eta(), 
                        closest_cell->phi(), 
                        closest_cell->deltaEta(), 
                        closest_cell->deltaPhi(), 
                        raw->bcid_start(), 
                        raw->bcid_end(), 
                        raw->bc_nsamples(), 
                        raw->bc_duration(), 
                        raw->pulse(), 
                        pulsePerBunch,
                        raw->rawEnergySamples(), 
                        closest_cell->layer()};

        // Make something here...
        cells->push_back(obj); 
      }

    }

    tree->Fill();

  }// Loop over all seeds


}


float RawNtupleMaker::dR( float eta1, float phi1, float eta2, float phi2 ) const
{
  float deta = fabs(eta1-eta2);
  float dphi = fabs(phi1-phi2)  < TMath::Pi() ? fabs(phi1-phi2) : 2*TMath::Pi() - fabs(phi1-phi2) ;
  return sqrt( deta*deta + dphi*dphi);
}



