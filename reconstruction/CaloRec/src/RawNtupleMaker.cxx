
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
  declareProperty( "NtupleName"     , m_ntupleName="raw"                );
}



RawNtupleMaker::~RawNtupleMaker()
{;}


StatusCode RawNtupleMaker::initialize()
{
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}


StatusCode RawNtupleMaker::bookHistograms( StoreGate &store ) const
{
  // Create all local variables since this must be a const method
  int eventNumber         = -1;
  float avgmu             = -1;
  std::vector<cell_t> cells;
 
  store.cd();
  TTree *tree = new TTree(m_ntupleName.c_str(), "");
  
  tree->Branch(  "EventNumber"        , &eventNumber        );
  tree->Branch(  "avgmu"              , &avgmu              );
  tree->Branch(  "cells",               &cells         );
  
 

  store.add( tree );
  
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





StatusCode RawNtupleMaker::fillHistograms( EventContext &ctx , StoreGate &store ) const
{
  store.cd();
  TTree *tree = store.tree(m_ntupleName);

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
  std::vector<cell_t> *cells        = nullptr;

  InitBranch( tree,  "EventNumber"        , &eventNumber        );
  InitBranch( tree,  "avgmu"              , &avgmu              );
  InitBranch( tree,  "cells"              , &cells);
  
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



  avgmu = (**event.ptr()).front()->avgmu();
  eventNumber = (**event.ptr()).front()->eventNumber();


  //auto seeds = (**event.ptr()).front()->allSeeds();
  
  
  for ( const auto cell : **container.ptr() ){

    auto raw = cell->parent();
    cell_t cellD{cell->eta(), cell->phi(), cell->deltaEta(), cell->deltaPhi(), raw->bcid_start(), raw->bcid_end(), raw->bc_nsamples(), raw->bc_duration(), raw->pulse(), raw->rawEnergySamples(), cell->sampling()};
    // Make something here...
    
    cells->push_back(cellD); 

  }// Loop over all cells

  tree->Fill();
}

