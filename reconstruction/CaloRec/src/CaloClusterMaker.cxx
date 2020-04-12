
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

G4Mutex mutex = G4MUTEX_INITIALIZER;


CaloClusterMaker::CaloClusterMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  // Key inputs
  declareProperty( "CellsKey"       , m_cellsKey="Cells"                );
  declareProperty( "EventKey"       , m_eventKey="EventInfo"            );
  // Key outputs
  declareProperty( "ClusterKey"     , m_clusterKey="Clusters"           );
  declareProperty( "TruthKey"       , m_truthKey="Particles"            );
  // Algorithm configuration
  declareProperty( "EtaWindow"      , m_etaWindow=0.4                   );
  declareProperty( "PhiWindow"      , m_phiWindow=0.4                   );
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
  declareProperty( "HistogramPath"  , m_histPath="Clusters"             );
}



CaloClusterMaker::~CaloClusterMaker()
{;}


StatusCode CaloClusterMaker::initialize()
{
  m_showerShapes = new ShowerShapes( "ShowerShapes" );
  return StatusCode::SUCCESS;
}


StatusCode CaloClusterMaker::bookHistograms( StoreGate &store ) const
{
  store.mkdir( m_histPath );  
  store.add( new TH1F("cl_et", "Count;E_{T};", 100, 0, 100) );
  store.add( new TH1F("cl_eta", "Count;#eta;", 100, -1.5, 1.5) );
  store.add( new TH1F("cl_phi", "Count;#phi;", 100, -3.2, 3.2) );
  store.add( new TH1F("cl_eratio", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store.add( new TH1F("cl_eratio2", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store.add( new TH1F("cl_eratio3", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store.add( new TH1F("cl_eratio4", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store.add( new TH1F("cl_eratio5", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store.add( new TH1F("cl_eratio6", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store.add( new TH1F("cl_reta", "Count;R_{#eta};", 100, 0.5, 1.1) );
  store.add( new TH1F("cl_rphi", "Count;R_{#phi};", 100, 0.5, 1.1) );
  store.add( new TH1F("cl_rhad", "Count;R_{had};", 100, 0.1, 1.1) );
  store.add( new TH1F("cl_f1", "Count;f_{1};", 100, 0.0, 1.05) );
  store.add( new TH1F("cl_f3", "Count;f_{3};", 100, 0.0, 1.05) );
  store.add( new TH1F("cl_weta2", "Count;W_{#eta2};", 100, 0.0, 1.05) );
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
  return StatusCode::SUCCESS;
}


StatusCode CaloClusterMaker::post_execute( EventContext &ctx ) const
{

  SG::WriteHandle<xAOD::TruthParticleContainer> particles(m_truthKey, ctx);
  SG::WriteHandle<xAOD::CaloClusterContainer> clusters(m_clusterKey, ctx);

  clusters.record( std::unique_ptr<xAOD::CaloClusterContainer>(new xAOD::CaloClusterContainer()) );
  particles.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()) );
  
  // Event info
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);

  if( !event.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
  }

 
  MSG_INFO( "Associate all truth particles and clusters");
  // Truth and associated clusters using truth energy
  for( auto& it : getAllClusters(ctx, m_cellsKey) )
  {
    auto clus = it.second;
    if(clus){
      it.first->setCaloCluster(clus);
      clusters->push_back(clus);
      particles->push_back( it.first );
    }

  }

  MSG_INFO( "We found " << clusters->size() << " clusters (RoIs) inside of this event." );
  MSG_INFO( "We found " << particles->size() << " particles (seeds) inside of this event." );

  return StatusCode::SUCCESS;
}


std::vector< std::pair<xAOD::TruthParticle*, xAOD::CaloCluster* > > 
                                             CaloClusterMaker::getAllClusters( EventContext &ctx , std::string key) const
{
  std::vector< std::pair<xAOD::TruthParticle*, xAOD::CaloCluster *> > particles;
  
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);
  SG::ReadHandle<xAOD::CaloCellContainer> container( key, ctx );

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
    const xAOD::CaloCell *hotcell=nullptr;
    float emaxs2 = 0.0;
    // Searching for the closest cell to the event origin in EM2
    for (const auto cell : **container.ptr() ){
      if( cell->sampling() != CaloSample::EM2 ) continue;
      if( (cell->eta() <= seed.eta + m_etaWindow/2) && (cell->eta() > seed.eta - m_etaWindow/2) ){
        if( (cell->phi() <= seed.phi + m_phiWindow/2) && (cell->phi() > seed.phi - m_phiWindow/2) ){
          if( cell->energy() > emaxs2 ){
            hotcell=cell; emaxs2=cell->energy();
          }
        }
      }
    }

    xAOD::TruthParticle *particle = new xAOD::TruthParticle();
    particle->setEt( seed.et );
    particle->setEta( seed.eta );
    particle->setPhi( seed.phi );
    particle->setPdgid( seed.pdgid );
    xAOD::CaloCluster *clus=nullptr;

    if(hotcell){
      clus = new xAOD::CaloCluster( hotcell->energy(), hotcell->eta(), hotcell->phi(), m_etaWindow/2., m_phiWindow/2. );
      fillCluster( ctx, clus, m_cellsKey );
      m_showerShapes->executeTool( clus );
    }

    particles.push_back( std::make_pair( particle, clus ) );
  }

  return particles;
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


StatusCode CaloClusterMaker::fillHistograms(EventContext &ctx, StoreGate &store ) const
{

  MSG_INFO( "Fill all histograms" );
  
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

  MSG_INFO( "We found " << clusters->size() << " clusters (RoIs) inside of this event." );
  MSG_INFO( "We found " << particles->size() << " particles (seeds) inside of this event." );

  store.cd(m_histPath);

  MSG_INFO("============== Cluster Information ==============");

  for( const auto& particle : **particles.ptr() ){
  
    if ( !particle->caloCluster() ) continue;

    const auto* clus = particle->caloCluster() ;

    store.hist1("cl_et")->Fill( clus->et() / 1.e3);
    store.hist1("cl_eta")->Fill( clus->eta() );
    store.hist1("cl_phi")->Fill( clus->phi() );
    store.hist1("cl_reta")->Fill( clus->reta() );
    store.hist1("cl_rphi")->Fill( clus->rphi() );
    store.hist1("cl_rhad")->Fill( clus->rhad() );
    store.hist1("cl_eratio")->Fill( clus->eratio() );
    store.hist1("cl_eratio2")->Fill( clus->eratio2() );
    store.hist1("cl_eratio3")->Fill( clus->eratio3() );
    store.hist1("cl_eratio4")->Fill( clus->eratio4() );
    store.hist1("cl_eratio5")->Fill( clus->eratio5() );
    store.hist1("cl_eratio6")->Fill( clus->eratio6() );
    store.hist1("cl_f1")->Fill( clus->f1() );
    store.hist1("cl_f3")->Fill( clus->f3() );
    store.hist1("cl_weta2")->Fill( clus->weta2() );
    
    MSG_INFO( "Truth Particle information:" );
    MSG_INFO( "Et       : " << particle->et() );
    MSG_INFO( "Eta      : " << particle->eta() );
    MSG_INFO( "Phi      : " << particle->phi() );
    MSG_INFO( "Cluster information:" );
    MSG_INFO( "Et       :" << clus->et()    );
    MSG_INFO( "Eta      :" << clus->eta()   );
    MSG_INFO( "Phi      :" << clus->phi()   );
    MSG_INFO( "Reta     :" << clus->reta()  );
    MSG_INFO( "Rphi     :" << clus->rphi()  );
    MSG_INFO( "Rhad     :" << clus->rhad()  );
    MSG_INFO( "Eratio   :" << clus->eratio());
    MSG_INFO( "Eratio2  :" << clus->eratio2());
    MSG_INFO( "Eratio3  :" << clus->eratio3());
    MSG_INFO( "Eratio4  :" << clus->eratio4());
    MSG_INFO( "Eratio5  :" << clus->eratio5());
    MSG_INFO( "Eratio6  :" << clus->eratio6());
    MSG_INFO( "f1       :" << clus->f1());
    MSG_INFO( "f3       :" << clus->f3());
    MSG_INFO( "Weta2    :" << clus->weta2());
 
  }
  MSG_INFO("=================================================");
  return StatusCode::SUCCESS;
}



float CaloClusterMaker::dR( float eta1, float phi1, float eta2, float phi2 ) const
{
  float deta = fabs(eta1-eta2);
  float dphi = fabs(phi1-phi2)  < TMath::Pi() ? fabs(phi1-phi2) : 2*TMath::Pi() - fabs(phi1-phi2) ;
  return sqrt( deta*deta + dphi*dphi);
}





