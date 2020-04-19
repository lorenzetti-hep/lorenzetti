#include "CaloCluster/CaloClusterContainer.h"
#include "CaloCell/CaloCellContainer.h"
#include "CaloCellMerge.h"
#include "TVector3.h"
#include <cstdlib>

using namespace Gaugi;
using namespace SG;
using namespace CaloSampling;




CaloCellMerge::CaloCellMerge( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "CollectionKeys"   , m_collectionKeys={}           );
  declareProperty( "CellsKey"         , m_cellsKey="Cells"            );
  declareProperty( "TruthCellsKey"    , m_truthCellsKey="TruthCells"  );
  declareProperty( "OutputLevel"      , m_outputLevel=1               );

}

CaloCellMerge::~CaloCellMerge()
{;}


StatusCode CaloCellMerge::initialize()
{
  setMsgLevel( m_outputLevel );
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::bookHistograms( StoreGate &/*store*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

  
StatusCode CaloCellMerge::execute( EventContext &/*ctx*/ , const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::post_execute( EventContext &ctx ) const
{

  MSG_DEBUG( "Starting collection merge algorithm..." );

  MSG_DEBUG( "Creating reco cells containers with key " << m_cellsKey);
  SG::WriteHandle<xAOD::CaloCellContainer> recoContainer( m_cellsKey , ctx );
  recoContainer.record( std::unique_ptr<xAOD::CaloCellContainer>(new xAOD::CaloCellContainer()) );
  
  MSG_DEBUG( "Creating truth cells containers with key " << m_truthCellsKey);
  SG::WriteHandle<xAOD::CaloCellContainer> truthContainer( m_truthCellsKey , ctx );
  truthContainer.record( std::unique_ptr<xAOD::CaloCellContainer>(new xAOD::CaloCellContainer()) );

  for ( auto key : m_collectionKeys ){

    MSG_DEBUG( "Reading all cells from collection with key " << key );
    SG::ReadHandle<xAOD::CaloCellCollection> collection( key, ctx );
    
    if( !collection.isValid() ){
      MSG_WARNING( "It's not possible to read the xAOD::CaloCellCollection from this Context using this key: " << key );
      continue;
    }

    MSG_DEBUG( "Creating new cells and attach the object into the container" );
    for (const auto &pair : **collection.ptr() )
    {
      // Raw Cell with all geant/bunch/pulse information
      const xAOD::RawCell* raw=pair.second;
     
      // Create the truth cell 
      auto truth_cell = new xAOD::CaloCell();
      truth_cell->setEta( raw->eta() );
      truth_cell->setPhi( raw->phi() );
      truth_cell->setDeltaEta( raw->deltaEta() );
      truth_cell->setDeltaPhi( raw->deltaPhi() );
      truth_cell->setSampling( raw->sampling() );
      truth_cell->setEnergy( raw->truthRawEnergy() );
      truth_cell->setEt( truth_cell->energy() / std::cosh( truth_cell->eta() ) );
      truth_cell->setParent( raw );
      truthContainer->push_back( truth_cell );

      // Create the Reco cell
      auto cell = new xAOD::CaloCell();
      truth_cell->setEta( raw->eta() );
      cell->setEta( raw->eta() );
      cell->setPhi( raw->phi() );
      cell->setDeltaEta( raw->deltaEta() );
      cell->setDeltaPhi( raw->deltaPhi() );
      cell->setSampling( raw->sampling() );
      cell->setEnergy( raw->energy() );
      cell->setEt( cell->energy() / std::cosh( cell->eta() ) );
      cell->setParent( raw );
      recoContainer->push_back( cell );

    }// Loop over all RawCells
  }// Loop over all collections

  MSG_DEBUG( "All collections were merged into two CaloCellContainer" );
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::fillHistograms( EventContext &/*ctx*/ , StoreGate &/*store*/ ) const
{
  return StatusCode::SUCCESS;
}



