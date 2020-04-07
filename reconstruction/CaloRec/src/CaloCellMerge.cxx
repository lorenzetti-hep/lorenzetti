#include "CaloCluster/CaloClusterContainer.h"
#include "G4Kernel/EventReader.h"
#include "CaloCellMerge.h"
#include "TVector3.h"
#include <cstdlib>

using namespace Gaugi;
using namespace SG;
using namespace CaloSampling;




CaloCellMerge::CaloCellMerge( std::string name ) : 
  Algorithm( name ),
  IMsgService(name),
{
  declareProperty( "CollectionKeys"   , m_collectionKeys={}           );
  declareProperty( "CellsKey"         , m_cellsKey="Cells"            );
  declareProperty( "TruthCellsKey"    , m_truthCellsKey="TruthCells"  );
  declareProperty( "OutputLevel"      , m_outputLevel=1               );

}


StatusCode CaloCellMerge::initialize()
{
  setMsgLevel( (MSG::Level)m_outputLevel );
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::pre_execute( EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

  
StatusCode CaloCellMerge::execute( EventContext &ctx , const G4Step *step ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::post_execute( EventContext &ctx ) const
{

  SG::WriteHandle<xAOD::CaloCellContainer> recoContainer( m_cellsKey , ctx );
  SG::WriteHandle<xAOD::CaloCellContainer> truthContainer( m_truthCellsKey , ctx );

  for ( auto key : m_collectionKeys ){

    SG::ReadHandle<xAOD::CaloCellCollection> collection( key, ctx );
    
    if( !collection.isValid() ){
      MSG_WARNING( "It's not possible to read the xAOD::CaloCellCollection from this Context using this key: " << key );
      continue;
    }

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
      truth_cell->setEnergy( raw->truthEnergy() );
      truth_cell->setEt( cell->energy() / std::cosh( cell->eta() ) );
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
      cell->setEnergy( raw->estimatedEnergy() );
      cell->setEt( cell->energy() / std::cosh( cell->eta() ) );
      cell->setParent( raw );
      recoContainer->push_back( cell );

    }// Loop over all RawCells
  }// Loop over all collections

  return StatusCode::SUCCESS;
}




StatusCode CaloCellMerge::fillHistograms( EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}



