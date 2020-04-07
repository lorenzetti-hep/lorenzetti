
#include "CaloCellCollection.h"
#include "CaloClusterMaker.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "TH1F.h"

using namespace std;
using namespace SG;
using namespace CaloSampling;



CaloClusterMaker::CaloClusterMaker( std::string name ) : 
  Gaugi::Algorithm( name ),
  IMsgService(name)
{
  declareProperty( "RingerKey"      , m_ringerKey="Rings"     );
  declareProperty( "ClusterKey"     , m_clusterKey="Clusters" );
  declareProperty( "TruthKey"       , m_truthKey="Particles"  );
  declareProperty( "HistogramPath"  , m_histPath="Rings"      );
  declareProperty( "OutputLevel"    , m_outputLevel=MSG::INFO );
}



CaloClusterMaker::~CaloClusterMaker()
{;}


StatusCode CaloClusterMaker::initialize()
{
  auto store = getStoreGateSvc();
  
  
  
  
  return StatusCode::SUCCESS;
}


StatusCode CaloClusterMaker::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode CaloClusterMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}



StatusCode CaloClusterMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}




StatusCode CaloClusterMaker::post_execute( EventContext &ctx ) const
{

  SG::WriteHandle<xAOD::CaloRingsContainer> ringer(m_ringerKey, ctx);
  ringer.record( std::unique_ptr<xAOD::CaloRingsContainer>(new xAOD::CaloRingsContainer()) );



  SG::ReadHandle<xAOD::CaloClusterContainer> clusters(m_clusterKey, ctx);

  
  std::vector< xAOD::RingSet > m_ringSets;

  for ( unsigned rs=0 ; rs < m_maxRingSets; ++rs )
  {  
    ringsets.push_back( xAOD::RingSet( m_layerRings[rs], m_nRings[rs], m_detaRings[rs], m_dphiRings[rs] );

  }




  // Loop over all CaloClusters
  for( auto* clus : **cluster.ptr())
  {

    // Clear all RingSets
    for ( auto &rs : ringsets ) rs.clear();

    // Create the CaloRings object
    auto rings = new xAOD::CaloRings();


    for ( auto& rs : ringsets ){

      auto *maxCell = maxCell( clus, rs.sampling() );

      // Fill all rings using the hottest cell as center
      for ( auto* cell : clus->allCells() )
      {
        rs.add( cell, clus->eta(), maxCell->eta(), maxCell->phi() );
      }

    }

    std::vector<float> ref_rings;
    ref_rings.reserve( m_maxRingsAccumulated );

    for ( auto& rs : ringsets )
      ref_rings.insert(ref_rings.end(), rs.pattern().begin(), rs.pattern().end());

    rings->setRings( ref_rings );
    rings->setCaloCluster( clus );

    ringer->push_back( ring );


  }



  return StatusCode::SUCCESS;
}



const xAOD::CaloCell * CaloRingsBuilder::maxCell( const xAOD::CaloCluster *clus, CaloSample sampling) const
{
  const xAOD::CaloCell maxCell=nullptr;
  for ( auto *cell : clus->allCells() ){

    if( cell->sampling() != sampling ) continue;

    if(!maxCell)  maxCell=cell;

    if (cell->energy() > maxCell->energy() )
        maxCell = cell;
  }// Loop over all cells inside of this cluster
  return maxCell;
}






