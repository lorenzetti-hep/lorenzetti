
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
  
  store->add( new TH1F("mc_cl_et", "Count;E_{T};", 100, 0, 100) );
  store->add( new TH1F("mc_cl_eta", "Count;#eta;", 100, -1.5, 1.5) );
  store->add( new TH1F("mc_cl_phi", "Count;#phi;", 100, -3.2, 3.2) );
  store->add( new TH1F("mc_cl_eratio", "Count;E_{ratio};", 100, 0.0, 1.05) );
  store->add( new TH1F("mc_cl_reta", "Count;R_{#eta};", 100, 0.5, 1.1) );
  store->add( new TH1F("mc_cl_rphi", "Count;R_{#phi};", 100, 0.5, 1.1) );
  store->add( new TH1F("mc_cl_rhad", "Count;R_{had};", 100, 0.1, 1.1) );
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

  

  // Truth and associated clusters using truth energy
  for( auto& it : getAllClusters(ctx,m_truthCellsKey,true) )
  {
    particles->push_back( it.first );
    truth_clusters->push_back( it.second );
  }

  // Only clustes associated to the truth using the estimated energy
  for( auto& it : getAllClusters(ctx,m_cellsKey,false) )
  {
    clusters->push_back( it.second );
  }



  MSG_INFO( "We found " << clusters->size() << " clusters (RoIs) inside of this event." );
  MSG_INFO( "We found " << particles->size() << " particles (seeds) inside of this event." );
  MSG_INFO( "We found " << truth_clusters->size() << " clusters (truth) associated to the particles inside of this event." );

  return StatusCode::SUCCESS;
}




std::vector< std::pair<xAOD::TruthParticle*,xAOD::CaloCluster*> > CaloClusterMaker::getAllClusters( EventContext &ctx , std::string key, bool createTruth) const
{
  std::vector< std::pair<xAOD::TruthParticle*,xAOD::CaloCluster*> > particles;
  
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
    float emaxs2 = 0.0;
    const xAOD::CaloCell *hotcell=nullptr;
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

    // Create the cluster using the hotcell position
    auto *clus  = new xAOD::CaloCluster( hotcell->energy(), hotcell->eta(), hotcell->phi(), m_etaWindow/2., m_phiWindow/2. );
    fillCluster( ctx, clus, key );
    m_showerShapes->executeTool( clus );

    if ( createTruth ){
      xAOD::TruthParticle *particle = new xAOD::TruthParticle();
      particle->setEt( seed.et );
      particle->setEta( seed.eta );
      particle->setPhi( seed.phi );
      particle->setPdgid( seed.pdgid );
      particle->setCaloCluster(clus);
      particles.push_back( std::make_pair(particle,clus) );
    }else{
      particles.push_back( std::make_pair(nullptr,clus) );
    }
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

  MSG_INFO("============== Cluster Information ==============");

  for( const auto& particle : **particles.ptr() ){
    
    if ( !particle->caloCluster() ) continue;

    const auto* clus = particle->caloCluster() ;
    store->hist1("mc_cl_et")->Fill( clus->et() / 1.e3);
    store->hist1("mc_cl_eta")->Fill( clus->eta() );
    store->hist1("mc_cl_phi")->Fill( clus->phi() );
    store->hist1("mc_cl_reta")->Fill( clus->reta() );
    store->hist1("mc_cl_rphi")->Fill( clus->rphi() );
    store->hist1("mc_cl_rhad")->Fill( clus->rhad() );
    store->hist1("mc_cl_eratio")->Fill( clus->eratio() );
  
    MSG_INFO( "Truth particle:" );
    MSG_INFO( "Et       : " << particle->et() );
    MSG_INFO( "Eta      : " << particle->eta() );
    MSG_INFO( "Phi      : " << particle->phi() );
    MSG_INFO( "Cluster with truth energy:" );
    MSG_INFO( "Et       :" << clus->et()    );
    MSG_INFO( "Eta      :" << clus->eta()   );
    MSG_INFO( "Phi      :" << clus->phi()   );
    MSG_INFO( "Reta     :" << clus->reta()  );
    MSG_INFO( "Rphi     :" << clus->rphi()  );
    MSG_INFO( "Rhad     :" << clus->rhad()  );
    MSG_INFO( "Eratio   :" << clus->eratio());
 
  }

  for( const auto& clus : **clusters.ptr() ){
    
    store->hist1("cl_et")->Fill( clus->et() / 1.e3);
    store->hist1("cl_eta")->Fill( clus->eta() );
    store->hist1("cl_phi")->Fill( clus->phi() );
    store->hist1("cl_reta")->Fill( clus->reta() );
    store->hist1("cl_rphi")->Fill( clus->rphi() );
    store->hist1("cl_rhad")->Fill( clus->rhad() );
    store->hist1("cl_eratio")->Fill( clus->eratio() );
  
    MSG_INFO( "Cluster with estimated energy:" );
    MSG_INFO( "Et       :" << clus->et()    );
    MSG_INFO( "Eta      :" << clus->eta()   );
    MSG_INFO( "Phi      :" << clus->phi()   );
    MSG_INFO( "Reta     :" << clus->reta()  );
    MSG_INFO( "Rphi     :" << clus->rphi()  );
    MSG_INFO( "Rhad     :" << clus->rhad()  );
    MSG_INFO( "Eratio   :" << clus->eratio());
  
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





