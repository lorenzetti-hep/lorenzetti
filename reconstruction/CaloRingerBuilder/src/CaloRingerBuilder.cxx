

#include "CaloRingerBuilder.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "TH1F.h"
#include "TH2F.h"
#include <numeric>

using namespace SG;
using namespace CaloSampling;
using namespace Gaugi;


CaloRingerBuilder::CaloRingerBuilder( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "RingerKey"      , m_ringerKey="Rings"     );
  declareProperty( "ClusterKey"     , m_clusterKey="Clusters" );
  declareProperty( "DeltaEtaRings"  , m_detaRings={}          );
  declareProperty( "DeltaPhiRings"  , m_dphiRings={}          );
  declareProperty( "NRings"         , m_nRings={}             );
  declareProperty( "LayerRings"     , m_layerRings={}         );
  declareProperty( "OutputLevel"    , m_outputLevel=1         );
  declareProperty( "HistogramPath"  , m_histPath=""           );
}


CaloRingerBuilder::~CaloRingerBuilder()
{;}


StatusCode CaloRingerBuilder::initialize()
{
  setMsgLevel(m_outputLevel);
  m_maxRingsAccumulated = std::accumulate(m_nRings.begin(), m_nRings.end(), 0);
  m_maxRingSets = m_nRings.size(); 

  return StatusCode::SUCCESS;
}


StatusCode CaloRingerBuilder::finalize()
{
  /*
  for (int r=0; r < m_maxRingsAccumulated; ++r){
    std::stringstream ss; ss << "rings_" << r;
    float energy = store->hist1( ss.str() )->GetMean();
    store->hist2( "ring_profile" )->SetBinContent( r+1, energy );
  }*/
  
  return StatusCode::SUCCESS;
}


StatusCode CaloRingerBuilder::bookHistograms( StoreGate &store ) const
{
  store.mkdir( m_histPath );
  store.add( new TH2F( "rings", "Et Vs #ring; #ring; E_{T} [GeV]; Count", m_maxRingsAccumulated, 0, m_maxRingsAccumulated, 150, 0, 150 ));
  return StatusCode::SUCCESS;
}


StatusCode CaloRingerBuilder::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloRingerBuilder::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloRingerBuilder::post_execute( EventContext &ctx ) const
{

  SG::WriteHandle<xAOD::CaloRingsContainer> ringer(m_ringerKey, ctx);
  ringer.record( std::unique_ptr<xAOD::CaloRingsContainer>(new xAOD::CaloRingsContainer()) );

  SG::ReadHandle<xAOD::CaloClusterContainer> clusters(m_clusterKey, ctx);
  
  std::vector< xAOD::RingSet > ringsets;

  MSG_DEBUG( "Creating all RingSets...");
  for ( int rs=0 ; rs < m_maxRingSets; ++rs )
  {  
    ringsets.push_back( xAOD::RingSet( (CaloSample)m_layerRings[rs], m_nRings[rs], m_detaRings[rs], m_dphiRings[rs] ) );

  }

  // Loop over all CaloClusters
  for( auto* clus : **clusters.ptr())
  {
    // Clear all RingSets
    for ( auto &rs : ringsets ) rs.clear();

    MSG_DEBUG( "Creating the CaloRings for this cluster..." );
    // Create the CaloRings object
    auto rings = new xAOD::CaloRings();
    for ( auto& rs : ringsets ){

      auto *hotCell = maxCell( clus, rs.sampling() );
     
      // Fill all rings using the hottest cell as center
      if( hotCell ){
        for ( auto* cell : clus->allCells() )
        {
          rs.add( cell, hotCell->eta(), hotCell->phi() );
        }
      }
    }

    std::vector<float> ref_rings;
    ref_rings.reserve( m_maxRingsAccumulated );

    for ( auto& rs : ringsets )
      ref_rings.insert(ref_rings.end(), rs.pattern().begin(), rs.pattern().end());

    MSG_DEBUG( "Setting all ring informations and attach into the EventContext." );
    rings->setRings( ref_rings );
    rings->setCaloCluster( clus );
    ringer->push_back( rings );
  
  }

  return StatusCode::SUCCESS;
}


const xAOD::CaloCell * CaloRingerBuilder::maxCell( const xAOD::CaloCluster *clus, CaloSample sampling) const
{
  const xAOD::CaloCell *maxCell=nullptr;
  for ( auto *cell : clus->allCells() ){

    if( cell->sampling() != sampling ) continue;

    if(!maxCell)  maxCell=cell;

    if (cell->energy() > maxCell->energy() )
        maxCell = cell;
  }// Loop over all cells inside of this cluster
  return maxCell;
}


StatusCode CaloRingerBuilder::fillHistograms( EventContext &ctx , StoreGate &store ) const
{
  SG::ReadHandle<xAOD::CaloRingsContainer> ringer( m_ringerKey, ctx );

  if( !ringer.isValid() ){
    MSG_ERROR( "It's not possible to read CaloRingsContainer from this Context using this key "<< m_ringerKey );
    return StatusCode::FAILURE;
  }

  store.cd(m_histPath);
  for (auto rings : **ringer.ptr() ){
    auto ringerShape = rings->rings();

    for (int r=0; r < m_maxRingsAccumulated; ++r){
      store.hist1("rings")->Fill( r, ringerShape.at(r)/1.e3 );
    }
  }

  return StatusCode::SUCCESS;
}



