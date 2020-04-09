
#include "CaloCellCollection.h"
#include "CaloClusterMaker.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "TH1F.h"

using namespace SG;
using namespace CaloSampling;
using namespace Gaugi;


CaloClusterMaker::CaloClusterMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  // Key inputs
  declareProperty( "TruthCellsKey"  , m_truthCellsKey="TruthCells"      );
  declareProperty( "CellsKey"       , m_cellsKey="Cells"                );
  declareProperty( "EventKey"       , m_eventKey="EventInfo"            );
  // Key outputs
  declareProperty( "ClusterKey"     , m_clusterKey="Clusters"           );
  declareProperty( "TruthClusterKey", m_truthClusterKey="TruthClusters" );
  declareProperty( "TruthKey"       , m_truthKey="Particles"            );
  // Algorithm configuration
  declareProperty( "EnergyThreshold", m_energyThreshold=3*GeV           );
  declareProperty( "EtaWindow"      , m_etaWindow=0.4                   );
  declareProperty( "PhiWindow"      , m_phiWindow=0.4                   );
  declareProperty( "DeltaR"         , m_dR=0.15                         );            
  declareProperty( "ForceTruthMatch", m_forceTruthMatch=false           );
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
  declareProperty( "HistogramPath"  , m_histPath="Clusters"             );
}



CaloClusterMaker::~CaloClusterMaker()
{;}


StatusCode CaloClusterMaker::initialize()
{
  auto store = getStoreGateSvc();
  
  store->mkdir( m_histPath );  
  
  store->add( new TH1F("mc_et", "Count;E_{T};", 100, 0, 100) );
  store->add( new TH1F("mc_eta", "Count;#eta;", 100, -1.5, 1.5) );
  store->add( new TH1F("mc_phi", "Count;#phi;", 100, -3.2, 3.2) );
  store->add( new TH1F("mc_eratio", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store->add( new TH1F("mc_reta", "Count;R_{#eta};", 100, 0.5, 1.1) );
  store->add( new TH1F("mc_rphi", "Count;R_{#phi};", 100, 0.5, 1.1) );
  store->add( new TH1F("mc_rhad", "Count;R_{had};", 100, 0.1, 1.1) );
  store->add( new TH1F("cl_et", "Count;E_{T};", 100, 0, 100) );
  store->add( new TH1F("cl_eta", "Count;#eta;", 100, -1.5, 1.5) );
  store->add( new TH1F("cl_phi", "Count;#phi;", 100, -3.2, 3.2) );
  store->add( new TH1F("cl_eratio", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store->add( new TH1F("cl_reta", "Count;R_{#eta};", 100, 0.5, 1.1) );
  store->add( new TH1F("cl_rphi", "Count;R_{#phi};", 100, 0.5, 1.1) );
  store->add( new TH1F("cl_rhad", "Count;R_{had};", 100, 0.1, 1.1) );
  store->add( new TH1F("res_eta"    , "Count;res_{#eta};",100, -1, 1 ) );
  store->add( new TH1F("res_phi"    , "Count;res_{#phi};",100, -3.2, 3.2 ) );
  store->add( new TH1F("res_eratio" , "Count;res_{E_{ratio}};",100, -1, 1 ) );
  

  m_showerShapes = new ShowerShapes( "ShowerShapes" );
  
  //if ( m_showerShapes->initialize().isFailure() )
  //  MSG_FATAL("It's not possible to initialize the ShowerShapes tool");

  MSG_INFO("CaloClusterMaker was completed initialized.");
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


StatusCode CaloClusterMaker::finalize()
{
  //if ( m_showerShapes->finalize().isFailure() )
  //  MSG_FATAL("It's not possible to finalize the ShowerShapes tool");
  //delete m_showerShapes;
  return StatusCode::SUCCESS;
}



StatusCode CaloClusterMaker::post_execute( EventContext &ctx ) const
{
  bool forceTruthMatch = m_forceTruthMatch;


  SG::WriteHandle<xAOD::TruthParticleContainer> particles(m_truthKey, ctx);
  SG::WriteHandle<xAOD::CaloClusterContainer> clusters(m_clusterKey, ctx);
  SG::WriteHandle<xAOD::CaloClusterContainer> truth_clusters(m_truthClusterKey, ctx);

  // Reco clusters
  clusters.record( std::unique_ptr<xAOD::CaloClusterContainer>(new xAOD::CaloClusterContainer()) );
  // Truth clusters
  truth_clusters.record( std::unique_ptr<xAOD::CaloClusterContainer>(new xAOD::CaloClusterContainer()) );
  // Truth particles
  particles.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()) );
  
  // Event info
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);

  if( !event.isValid() ){
    MSG_WARNING( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
    forceTruthMatch = false;
  }

  auto particles_vec = getAllParticles(ctx);
  forceTruthMatch = particles->size()>0 ? m_forceTruthMatch : false;
  auto clusters_vec = getAllClusters(ctx);

  if ( forceTruthMatch ){

    for( auto& caloCluster : clusters_vec )
    {
      for( auto& particlePair : particles_vec )
      {
        if( particlePair.second ){
          fillCluster( ctx, particlePair.second , m_truthCellsKey );
          m_showerShapes->executeTool( particlePair.second );
    
          //MSG_INFO( "DeltaR between particle and cluster is "<< dR(caloCluster, particle) );
          if( dR( particlePair.second->eta(), caloCluster->eta(),
                  particlePair.second->phi(), caloCluster->phi()) < m_dR )
          {
            fillCluster( ctx, caloCluster, m_cellsKey );
            // Calculate all shower shapes
            m_showerShapes->executeTool( caloCluster );
            clusters->push_back( caloCluster );
          }
          particlePair.first->setCaloCluster( particlePair.second );
          particles->push_back( particlePair.first );
          truth_clusters->push_back( particlePair.second );
        }
      }
    }
  }else{
    for( auto& caloCluster : clusters_vec ){
      fillCluster( ctx, caloCluster, m_cellsKey );
      m_showerShapes->executeTool( caloCluster );
      clusters->push_back( caloCluster );
    }
    for( auto& particlePair : particles_vec ){
      fillCluster( ctx, particlePair.second, m_truthCellsKey );
      m_showerShapes->executeTool( particlePair.second  );
      particlePair.first->setCaloCluster( particlePair.second );
      particles->push_back( particlePair.first );
      truth_clusters->push_back( particlePair.second );
    }
  }

  MSG_INFO( "We found " << clusters->size() << " clusters (RoIs) inside of this event." );
  MSG_INFO( "We found " << particles->size() << " particles (seeds) inside of this event." );
  MSG_INFO( "We found " << truth_clusters->size() << " clusters (truth) associated to the particles inside of this event." );

  return StatusCode::SUCCESS;
}




std::vector< std::pair<xAOD::TruthParticle*,xAOD::CaloCluster*> > CaloClusterMaker::getAllParticles( EventContext &ctx ) const
{
  std::vector< std::pair<xAOD::TruthParticle*,xAOD::CaloCluster*> > particles;
  
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);
  SG::ReadHandle<xAOD::CaloCellContainer> container( m_truthCellsKey, ctx );

  if( !event.isValid() ){
    MSG_WARNING( "It's not possible to read the xAOD::EventInfoContainer from this Context using this key: " << m_eventKey );
    return particles;
  }
  
  if( !container.isValid() )
  {
    MSG_WARNING("It's not possible to read the xAOD::CaloCellContainer from this Contaxt using this key " << m_cellsKey );
    return particles;
  }

  for ( auto& seed : (**event.ptr()).front()->allSeeds() )
  {
    xAOD::TruthParticle *p = new xAOD::TruthParticle();
    p->setEt( seed.et );
    p->setEta( seed.eta );
    p->setPhi( seed.phi );
    p->setPdgid( seed.pdgid );

    float maxDeltaR = 999;
    const xAOD::CaloCell *closest_cell=nullptr;
    // Searching for the closest cell to the event origin in EM2
    for (const auto cell : **container.ptr() ){
      // Get only cells collection from EM2 layer
      if( cell->sampling() != CaloSample::EM2 ) continue;
      float deltaR = dR( p->eta(), cell->eta(), p->phi(), cell->phi() ) ;
      if( deltaR < maxDeltaR ){
        closest_cell=cell;
        maxDeltaR=deltaR;
      }
    }
    auto *clus  = new xAOD::CaloCluster( closest_cell->energy(), closest_cell->eta(), closest_cell->phi(), m_etaWindow/2., m_phiWindow/2. ) ;
    particles.push_back( std::make_pair(p,clus) );
  }

  return particles;
}



std::vector< xAOD::CaloCluster* > CaloClusterMaker::getAllClusters( EventContext &ctx ) const
{
  std::vector<xAOD::CaloCluster*> vec_cluster;

  SG::ReadHandle<xAOD::CaloCellContainer> container( m_cellsKey, ctx );

  if( !container.isValid() )
  {
    MSG_WARNING("It's not possible to read the xAOD::CaloCellContainer from this Contaxt using this key " << m_cellsKey );
    return vec_cluster;
  }


  // Get all cells from the second calorimeter layer
  for (const auto cell : **container.ptr() ){

    // Get only cells collection from EM2 layer
    if( cell->sampling() != CaloSample::EM2 ) continue;
    // Must be higher than energy cut to be considere an roi
    if (cell->energy() < m_energyThreshold ) continue;
 
    float eta = cell->eta(); 
    float phi = cell->phi(); 
    float emaxs2 =  cell->energy();
    
    bool newCluster = true;
    // Check cluster overlap. Get the higher energy cluster energy in case of overlap
    for( unsigned int i=0; i < vec_cluster.size(); ++i ){
      if ( abs(eta - vec_cluster[i]->eta()) < m_etaWindow/2. 
          && abs( phi - vec_cluster[i]->phi()) < m_phiWindow/2. )
      { 
        newCluster=false;
        if (emaxs2 > vec_cluster[i]->emaxs2())
        { // Overwrite the cluster position and energy
          vec_cluster[i]->setEmaxs2(emaxs2); vec_cluster[i]->setEta(eta); vec_cluster[i]->setPhi(phi);
          break;
        }
      }
    }
    if(newCluster){
      vec_cluster.push_back( new xAOD::CaloCluster( emaxs2, eta, phi, m_etaWindow/2., m_phiWindow/2. ) );
    }
  }

  return vec_cluster;
}




void CaloClusterMaker::fillCluster( EventContext &ctx, xAOD::CaloCluster *clus, std::string key) const
{
  SG::ReadHandle<xAOD::CaloCellContainer> container(key, ctx);
  if( !container.isValid() ){
    MSG_WARNING( "It's not possible to read the xAOD::CaloCellContainer from this Context using this key: " << key );
    return;
  }

  for ( const auto cell : **container.ptr() ){
    // Check if the current cell is in the eta window
    if( (cell->eta() < clus->eta()+m_etaWindow/2.) && (cell->eta() > clus->eta()-m_etaWindow/2.) )
    {
      // Check if the current cell is in the phi window
      if( (cell->phi() < clus->phi()+m_phiWindow/2.) && (cell->phi() > clus->phi()-m_phiWindow/2.) )
      {
        // Add the cell to the cluster
        clus->push_back(cell);
      }
    }
  }// Loop over all cells
}







StatusCode CaloClusterMaker::fillHistograms(EventContext &ctx) const
{
  MSG_INFO( "Fill all histograms" );
  auto store = getStoreGateSvc();
  
  SG::ReadHandle<xAOD::CaloClusterContainer> clusters( m_clusterKey, ctx );
  SG::ReadHandle<xAOD::TruthParticleContainer> particles( m_truthKey, ctx );


  if( !clusters.isValid() ){
    MSG_ERROR("It's not possible to get the CaloClusterContainer inside of this Context using this key: " << m_clusterKey);
    return StatusCode::FAILURE;
  }


  if( !particles.isValid() ){
    MSG_ERROR("It's not possible to get the TruthParticleContainer inside of this Context using this key: " << m_truthKey);
    return StatusCode::FAILURE;
  }

  store->cd(m_histPath);

  for( const auto& clus : **clusters.ptr() ){
    
    store->hist1("cl_et")->Fill( clus->et() / 1.e3);
    store->hist1("cl_eta")->Fill( clus->eta() );
    store->hist1("cl_phi")->Fill( clus->phi() );
    store->hist1("cl_reta")->Fill( clus->reta() );
    store->hist1("cl_rphi")->Fill( clus->rphi() );
    store->hist1("cl_rhad")->Fill( clus->rhad() );
    store->hist1("cl_eratio")->Fill( clus->eratio() );
  }


  for( const auto& particle : **particles.ptr() ){
    
    if ( !particle->caloCluster() ) continue;

    const auto* clus = particle->caloCluster() ;
    store->hist1("mc_et")->Fill( clus->et() / 1.e3);
    store->hist1("mc_eta")->Fill( clus->eta() );
    store->hist1("mc_phi")->Fill( clus->phi() );
    store->hist1("mc_reta")->Fill( clus->reta() );
    store->hist1("mc_rphi")->Fill( clus->rphi() );
    store->hist1("mc_rhad")->Fill( clus->rhad() );
    store->hist1("mc_eratio")->Fill( clus->eratio() );
  }


  return StatusCode::SUCCESS;
}



float CaloClusterMaker::dR( float eta1, float phi1, float eta2, float phi2 ) const
{
  float deta = fabs(eta1-eta2);
  float dphi = fabs(phi1-phi2)  < TMath::Pi() ? fabs(phi1-phi2) : 2*TMath::Pi() - fabs(phi1-phi2) ;
  return sqrt( deta*deta + dphi*dphi);
}





