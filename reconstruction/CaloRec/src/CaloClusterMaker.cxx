
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
  Gaugi::Algorithm( name )
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
  delete m_showerShapes;
}


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
  

  //m_showerShapes = new ShowerShapes( "ShowerShapes" );
  
  //if ( m_showerShapes->initialize().isFailure() )
  //  MSG_FATAL("It's not possible to initialize the ShowerShapes tool");


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
  if ( m_showerShapes->finalize().isFailure() )
    MSG_FATAL("It's not possible to finalize the ShowerShapes tool");

  return StatusCode::SUCCESS;
}



StatusCode CaloClusterMaker::post_execute( EventContext &ctx ) const
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


  auto particles_vec = getAllParticles(ctx);
  forceTruthMatch = particles->size()>0 ? m_forceTruthMatch : false;

  auto clusters_vec = getAllClusters(ctx);

  if ( forceTruthMatch ){

    for( auto& caloCluster : clusters_vec )
    {
      for( auto& particle : particles_vec )
      {
        fillCluster<xAOD::TruthParticle>( ctx, particle );
        //calculate( particle );
    
        MSG_INFO( "DeltaR between particle and cluster is "<< dR(caloCluster, particle) );
        if( !particle->caloCluster() && dR( caloCluster, particle ) < m_dR ){
          fillCluster<xAOD::CaloCluster>( ctx, caloCluster );
          // Calculate all shower shapes
          //m_showerShapes->executeTool( caloCluster )
          particle->setCaloCluster( caloCluster );
          clusters->push_back( caloCluster );
        }
        particles->push_back( particle );
      }
    }

  }else{
    for( auto& caloCluster : clusters_vec ){
      fillCluster<xAOD::CaloCluster>( ctx, caloCluster );
      //m_showerShapes->executeTool( caloCluster )
      clusters->push_back( caloCluster );
    }

    for( auto& particle : particles_vec ){
      fillCluster<xAOD::TruthParticle>( ctx, particle );
      //m_showerShapes->executeTool( particle )
      particles->push_back( particle );
    }
  }



  MSG_INFO( "We found " << clusters->size() << " clusters (RoIs) inside of this event." );
  MSG_INFO( "We found " << particles->size() << " particles (seeds) inside of this event." );

  for (auto& particle : **particles)
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

  for ( auto& seed : (**event.ptr()).front()->allSeeds() )
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
    if( collection->sampling() != CaloSample::EM2 ) continue;


    // Get all cells from the second calorimeter layer
    for (const auto &pair : **collection.ptr() )
    {
      const auto* cell=pair.second;

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
    for ( auto& pair : **collection.ptr() )
    {
      const auto *cell = pair.second;
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
}







StatusCode CaloClusterMaker::fillHistograms(EventContext &ctx) const
{
  auto store = getStoreGateSvc();
  
  SG::ReadHandle<xAOD::CaloClusterContainer> clusters( m_clusterKey, ctx );
  SG::ReadHandle<xAOD::TruthParticleContainer> particles( m_truthKey, ctx );


  if( !clusters.isValid() ){
    MSG_ERROR("It's not possible to get the CaloClusterContainer inside of this Context using this key: " << m_clusterKey);
    return StatusCode::FAILURE;
  }

  for( const auto& clus : **clusters.ptr() ){
    store->hist1(m_histPath+"/cl_et")->Fill( clus->et() / 1.e3);
    store->hist1(m_histPath+"/cl_eta")->Fill( clus->eta() );
    store->hist1(m_histPath+"/cl_phi")->Fill( clus->phi() );
    store->hist1(m_histPath+"/cl_reta")->Fill( clus->reta() );
    store->hist1(m_histPath+"/cl_rphi")->Fill( clus->rphi() );
    store->hist1(m_histPath+"/cl_rhad")->Fill( clus->rhad() );
    store->hist1(m_histPath+"/cl_eratio")->Fill( clus->eratio() );
  }

  return StatusCode::SUCCESS;
}



float CaloClusterMaker::dR( const xAOD::CaloCluster *clus, const xAOD::TruthParticle *p ) const
{
  float deta = fabs(clus->eta() - p->eta());
  float dphi = fabs(clus->phi()-p->phi())  < TMath::Pi() ? fabs(clus->phi()-p->phi()) : 2*TMath::Pi() - fabs(clus->phi()-p->phi()) ;
  return sqrt( deta*deta + dphi*dphi);
}





