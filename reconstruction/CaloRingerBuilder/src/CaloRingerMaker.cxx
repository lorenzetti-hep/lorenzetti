

#include "CaloRingerMaker.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "G4Kernel/CaloPhiRange.h"
#include "TH1F.h"
#include "TH2F.h"
#include <numeric>

using namespace SG;
using namespace Gaugi;


CaloRingerMaker::CaloRingerMaker( std::string name ) : 
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

//!=====================================================================

CaloRingerMaker::~CaloRingerMaker()
{;}

//!=====================================================================

StatusCode CaloRingerMaker::initialize()
{
  CHECK_INIT();

  setMsgLevel(m_outputLevel);
  m_maxRingsAccumulated = std::accumulate(m_nRings.begin(), m_nRings.end(), 0);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloRingerMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloRingerMaker::bookHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  store->mkdir( m_histPath );
  store->add( new TH2F( "rings", "Et Vs #ring; #ring; E_{T} [GeV]; Count", m_maxRingsAccumulated, 0, m_maxRingsAccumulated, 150, 0, 150 ));
  return StatusCode::SUCCESS;
}

//!=====================================================================


StatusCode CaloRingerMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================


StatusCode CaloRingerMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloRingerMaker::execute( EventContext &ctx, int /*evt*/ ) const
{
  return post_execute(ctx);
}

//!=====================================================================

StatusCode CaloRingerMaker::post_execute( EventContext &ctx ) const
{

  
  SG::WriteHandle<xAOD::CaloRingsContainer> ringer(m_ringerKey, ctx);
  ringer.record( std::unique_ptr<xAOD::CaloRingsContainer>(new xAOD::CaloRingsContainer()) );

  SG::ReadHandle<xAOD::CaloClusterContainer> clusters(m_clusterKey, ctx);
  
  std::vector< RingSet > vec_rs;

  MSG_DEBUG( "Creating all RingSets...");
  for ( int rs=0 ; rs < (int)m_nRings.size(); ++rs )
  {  
    std::vector<CaloSampling> samplings;
    for(auto samp : m_layerRings[rs])  {
      samplings.push_back((CaloSampling)samp);
    }
    vec_rs.push_back( RingSet( samplings, m_nRings[rs], m_detaRings[rs], m_dphiRings[rs] ) );
  }

  // Loop over all CaloClusters
  for( auto* clus : **clusters.ptr())
  {
    MSG_DEBUG( "Creating the CaloRings for this cluster..." );
    // Create the CaloRings object
    auto rings = new xAOD::CaloRings();

    for ( auto &rs : vec_rs ){

      // zeroize
      rs.clear();

      auto *hotCell = maxCell( clus, rs );
     
      // Fill all rings using the hottest cell as center
      for ( auto* cell : clus->cells() )
      {
        if (hotCell){
          rs.push_back( cell, hotCell->eta(), hotCell->phi() );
        }else{
          rs.push_back( cell, clus->eta(), clus->phi() );
        }
      }
      
    }

    std::vector<float> ref_rings;
    ref_rings.reserve( m_maxRingsAccumulated );

    for ( auto& rs : vec_rs )
      ref_rings.insert(ref_rings.end(), rs.rings().begin(), rs.rings().end());

    MSG_DEBUG( "Setting all ring informations and attach into the EventContext." );
    rings->setRings( ref_rings );
    rings->setCaloCluster( clus );
    ringer->push_back( rings );

  }
  
  return StatusCode::SUCCESS;
}

//!=====================================================================

const xAOD::CaloCell * CaloRingerMaker::maxCell( const xAOD::CaloCluster *clus, RingSet &rs ) const
{
  const xAOD::CaloCell *maxCell=nullptr;

  for ( auto *cell : clus->cells() ){

    if( !rs.isValid(cell) ) continue;

    if(!maxCell)  maxCell=cell;

    if (cell->e() > maxCell->e() )
        maxCell = cell;
  }// Loop over all cells inside of this cluster
  return maxCell;
}

//!=====================================================================

StatusCode CaloRingerMaker::fillHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  SG::ReadHandle<xAOD::CaloRingsContainer> ringer( m_ringerKey, ctx );

  if( !ringer.isValid() ){
    MSG_ERROR( "It's not possible to read CaloRingsContainer from this Context using this key "<< m_ringerKey );
    return StatusCode::FAILURE;
  }

  store->cd(m_histPath);
  for (auto rings : **ringer.ptr() ){
    auto ringerShape = rings->rings();

    for (int r=0; r < m_maxRingsAccumulated; ++r){
      store->hist1("rings")->Fill( r, ringerShape.at(r)/1.e3 );
    }
  }

  return StatusCode::SUCCESS;
}

//!=====================================================================
//!=====================================================================
//!=====================================================================
//!=====================================================================


RingSet::RingSet( std::vector<CaloSampling> &samplings, unsigned nrings, float deta, float dphi ):
  m_rings(nrings,0), 
  m_deta(deta), 
  m_dphi(dphi),
  m_samplings(samplings)
{;}

//!=====================================================================

void RingSet::push_back( const xAOD::CaloCell *cell , float eta_center, float phi_center )
{
  // This cell does not allow to this RingSet
  if( isValid(cell) ){
    float deta = std::abs( eta_center - cell->eta() ) / m_deta;
    float dphi = std::abs( CaloPhiRange::diff(phi_center , cell->phi()) ) / m_dphi;
    float deltaGreater = std::max(deta, dphi);
    int i = static_cast<unsigned int>( std::round(deltaGreater) );
    if( i < (int)m_rings.size() ){
      m_rings[i] += cell->e()/ std::cosh(std::abs(eta_center));
    }
  }
}

//!=====================================================================

size_t RingSet::size() const
{
  return m_rings.size();
}

//!=====================================================================

const std::vector<float>& RingSet::rings() const
{
  return m_rings;
}

//!=====================================================================

bool RingSet::isValid(const xAOD::CaloCell* cell ) const
{
  for( auto samp : m_samplings){
    if(cell->descriptor()->sampling() == samp)
      return true;
  }
  return false;
}

//!=====================================================================

void RingSet::clear()
{
  for (std::vector<float>::iterator it = m_rings.begin(); it < m_rings.end(); it++)
  {
    *it=0.0;
  }
}

