
#include "CaloClusterMaker.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

using namespace std;
using namespace SG;
using namespace CaloSampling;



CaloClusterMaker::CaloClusterMaker( std::string name ) : 
  Gaugi::Algorithm( name ),
{
  declareProperty( "EnergyThreshold", m_energyThreshold=3*GeV );
  declareProperty( "CollectionKeys" , m_collectionKeys={}     );
  declareProperty( "ClusterKey"     , m_clusterKey="Clusters" );
  declareProperty( "EventKey"       , m_eventKey="Event"      );
  declareProperty( "TruthKey"       , m_truthKey="Particles"  );
  declareProperty( "EtaWindow"      , m_etaWindow=0.4         );
  declareProperty( "PhiWindow"      , m_phiWindow=0.4         );
  declareProperty( "DeltaR"         , m_dR=0.15               );            
  declareProperty( "ForceTruthMatch", m_forceTruthMatch=true  );
  declareProperty( "OutputLevel"    , m_outputLevel=MSG::INFO );
  declareProperty( "HistogramPath"  , m_histPath="Clusters"   );
}



CaloClusterMaker::~CaloClusterMaker()
{
  delete m_showershapes;
}


StatusCode CaloClusterMaker::initialize()
{
  auto store = getStoreGateSvc();
  
  store->mkdir( m_histPath );  
  
  store->add( new TF1F("mc_et", "Count;E_{T};", 100, 0, 100) );
  store->add( new TF1F("mc_eta", "Count;#eta;", 100, -1.5, 1.5) );
  store->add( new TF1F("mc_phi", "Count;#phi;", 100, -3.2, 3.2) );
  store->add( new TF1F("mc_eratio", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store->add( new TF1F("mc_reta", "Count;R_{#eta};", 100, 0.5, 1.1) );
  store->add( new TF1F("mc_rphi", "Count;R_{#phi};", 100, 0.5, 1.1) );
  store->add( new TF1F("mc_rhad", "Count;R_{had};", 100, 0.1, 1.1) );
  
  
  store->add( new TF1F("cl_et", "Count;E_{T};", 100, 0, 100) );
  store->add( new TF1F("cl_eta", "Count;#eta;", 100, -1.5, 1.5) );
  store->add( new TF1F("cl_phi", "Count;#phi;", 100, -3.2, 3.2) );
  store->add( new TF1F("cl_eratio", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store->add( new TF1F("cl_reta", "Count;R_{#eta};", 100, 0.5, 1.1) );
  store->add( new TF1F("cl_rphi", "Count;R_{#phi};", 100, 0.5, 1.1) );
  store->add( new TF1F("cl_rhad", "Count;R_{had};", 100, 0.1, 1.1) );
  
  
  
  store->add( new TF1F("res_eta"    , "Count;res_{#eta};",100, -1, 1 ) );
  store->add( new TF1F("res_phi"    , "Count;res_{#phi};",100, -3.2, 3.2 ) );
  store->add( new TF1F("res_eratio" , "Count;res_{E_{ratio}};",100, -1, 1 ) );
  

  m_showerShapes = new ShowerShapes( "ShowerShapes" );
  
  if ( m_showerShapes->initialize().isFailure() )
    MSG_FATAL("It's not possible to initialize the ShowerShapes tool");


  return StatusCode::SUCCESS;
}


StatusCode CaloClusterMaker::pre_execute( EventContext *ctx ) const
{
  return StatusCode::SUCCESS;
}

StatusCode CaloClusterMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloClusterMaker::finalize()
{
  if ( m_showerShapes->finalize().isFailure() )
    MSG_FATAL("It's not possible to finalize the ShowerShapes tool");

  return StatusCode::SUCCESS;
}



StatusCode CaloClusterMaker::post_execute( EventContext *ctx ) const
{
  bool forceTruthMatch = m_forceTruthMatch;


  SG::WriteHandle<xAOD::CaloClusterContainer> clusters(m_clusterKey, ctx);
  SG::WriteHandle<xAOD::TruthParticleContainer> particles(m_truthKey, ctx);
  

  clusters.record( std::unique_ptr<xAOD::CaloClusterContainer>(new xAOD::CaloClusterContainer()) );
  particles.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()) );

  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);

  if( !event.isValid() ){
    MSG_WARNING( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
    forceTruthMatch = false;
  }


  auto particles = getAllParticles(ctx);
  bool forceTruthMatch = particles.size()>0 ? m_forceTruthMatch : false;

  auto clusters = getAllClusters(ctx);

  if ( forceTruthMatch ){

    for( auto& caloCluster : clusters )
    {
      for( auto& particle : particles )
      {
        fillCluster<xAOD::TruthPatcile>( particle );
        calculate( particle );
    
        MSG_INFO( "DeltaR between particle and cluster is "<< dR(caloCluster, particle) );
        if( !particle->caloCluster() && dR( caloCluster, particle ) < m_dR ){
          fillCluster( caloCluster );
          calculate( caloCluster );
          // Calculate all shower shapes
          m_showerShapes->executeTool( caloCluster )
          particle->setCaloCluster( caloCluster );
          clusters->push_back( caloCluster );
        }
        particles->push_back( particle );
      }
    }

  }else{
    for( auto& caloCluster : clusters ){
      fillCluster<xAOD::CaloCluster>( caloCluster );
      m_showerShapes->executeTool( caloCluster )
      calculate( caloCluster );
      clusters->push_back( caloCluster );
    }

    for( auto& particle : particles ){
      fillParticle( particle );
      m_showerShapes->executeTool( particle )
      particles->push_back( particle );
    }
  }



  MSG_INFO( "We found " << clusters.size() << " clusters (RoIs) inside of this event." );
  MSG_INFO( "We found " << particles.size() << " particles (seeds) inside of this event." );

  for (auto& particle : *particles)
  {
    bool matched = particle->caloCluster()?true:false;
    MSG_INFO( "Particle in (eta="<<particle->eta() << ",phi="<< particle->phi()<< ") with " << particle->et()<< ". With cluster? " << (matched?"Yes":"No") );
  }



  return StatusCode::SUCCESS;
}




std::vector< xAOD::TruthParticle* > CaloClusterMaker::getAllParticles( EventContext &ctx) const
{
  std::vector< xAOD::TruthParticle* > particles;
  
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);

  if( !event.isValid() ){
    MSG_WARNING( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
    return particles;
  }

  for ( auto& seed : event->front()->allSeeds() )
  {
    xAOD::TruthParticle *particle = new xAOD::TruthParticle();
    particle->setEta( seed.eta );
    particle->setPhi( seed.phi );
    particle->setPdgid( seed.pdgid );
    particles.push_back( particle );
  }

  return particles;
}



std::vector< xAOD::CaloCluster* > CaloClusterMaker::getAllClusters( EventContext &ctx) const
{
  std::vector<xAOD::CaloCluster*> vec_cluster;

  for ( auto key : m_collectionKeys )
  {

    SG::ReadHandle<xAOD::CaloCellCollection> collection(key, ctx);

    if( !collection.isValid() ){
      MSG_WARNING( "It's not possible to read the xAOD::CaloCellCollection from this Context using this key: " << key );
      continue;
    }

    // Get only cells collection from EM2 layer
    if( collection->sampling() != CaloSample::EM2 ) continue


    // Get all cells from the second calorimeter layer
    for (const auto &cell : *collection )
    {
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

  }// Loop over all collections stored into the Context

  return vec_cluster;
}



template<class T>
void CaloClusterMaker::fillCluster( EventContext &ctx, T* clus) const
{
  for ( auto key : m_collectionKeys )
  {
    SG::ReadHandle<xAOD::CaloCellCollection> collection(key, ctx);
    if( !collection.isValid() ){
      MSG_WARNING( "It's not possible to read the xAOD::CaloCellCollection from this Context using this key: " << key );
      continue;
    }
    // Attach all cells into your cluster
    for ( auto& cell : *collection )
    {
      // Check if the current cell is in the eta window
      if( (cell->eta() < clus->eta()+m_etaWindow/2.) && (cell->eta() > clus->eta()-m_etaWindow/2.) )
      {
        // Check if the current cell is in the phi window
        if( (cell->phi() < clus->phi()+m_phiWindow/2.) && (cell->phi() > clus->phi()-m_phiWindow/2.) )
        {
          // Add the cell to the cluster
          cluster->push_back(cell);
        }
      }
    }// Loop over all cells
  }
}







StatusCode CaloClusterMaker::fillHistograms(EventContext *ctx) const
{
  auto store = getStoreGateSvc();
  
  SG::Readandle<xAOD::CaloClusterContainer> clusters( m_clusterKey, ctx );
  SG::Readandle<xAOD::TruthParticleContainer> particles( m_truthKey, ctx );


  if( !cluster.isValid() ){
    MSG_ERROR("It's not possible to get the CaloClusterContainer inside of this Context using this key: " << m_clusterKey);
    return StatusCode::FAILURE;
  }

  for( const auto& clus : *clusters ){
    store->hist1(m_basePath+"/cl_et")->Fill( clus->et() / 1.e3);
    store->hist1(m_basePath+"/cl_eta")->Fill( clus->eta() );
    store->hist1(m_basePath+"/cl_phi")->Fill( clus->phi() );
    store->hist1(m_basePath+"/cl_reta")->Fill( clus->reta() );
    store->hist1(m_basePath+"/cl_rphi")->Fill( clus->rphi() );
    store->hist1(m_basePath+"/cl_rhad")->Fill( clus->rhad() );
    store->hist1(m_basePath+"/cl_eratio")->Fill( clus->eratio() );
  }

  return StatusCode::SUCCESS;
}



float CaloClusterMaker::dR( const xAOD::CaloCluster *clus, const xAOD::TruthParticle *p ) const
{
  float deta = fabs(clus->eta() - p->eta());
  float dphi = fabs( (clus->phi()-p->phi())  < TMath::Pi() ? fabs(clus->phi()-p->phi()) : 2*TMath::Pi() - fabs(clus->phi()-p->phi()) ;
  return sqrt( deta*deta + dphi*dphi);
}





