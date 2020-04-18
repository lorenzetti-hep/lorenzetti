
#include "G4Kernel/CaloPhiRange.h"
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
  declareProperty( "MinCenterEnergy", m_minCenterEnergy=15*GeV          );
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
  
  store.add( new TH1F("res_et" ,"#E_{TTruth}-#E_{T,Cluster};res_{E_{T}};Count",100,-40,40) );
  store.add( new TH1F("res_eta", "#eta_{Truth}-#eta_{Cluster};res_{#eta};Count",100,-2,2 ) );
  store.add( new TH1F("res_phi", "#phi_{Truth}-#phi_{Cluster};res_{#phi};Count",100,-2,2 ) );
  store.add( new TH1F("cl_et"   , ";Count;E_{T};"       , 100, 0.0  , 100 ) );
  store.add( new TH1F("cl_eta"  , ";Count;#eta;"        , 100, -1.5 , 1.5 ) );
  store.add( new TH1F("cl_phi"  , ";Count;#phi;"        , 100, -3.2 , 3.2 ) );
  store.add( new TH1F("cl_f1"   , ";Count;f_{1};"       , 100, -0.02, 0.7 ) );
  store.add( new TH1F("cl_f3"   , ";Count;f_{3};"       , 200, -0.05, 0.15) );
  store.add( new TH1F("cl_weta2", ";Count;W_{#eta2};"   , 100, 0.005, 0.02) );
  store.add( new TH1F("cl_reta" ,  ";Count;R_{#eta};"   , 200, 0.8  , 1.10) );
  store.add( new TH1F("cl_rphi" , ";Count;R_{#phi};"    , 200, 0.45 , 1.05) );
  store.add( new TH1F("cl_rhad" , ";Count;R_{had};"     , 200, -0.05, 0.05) );
  store.add( new TH1F("cl_eratio", ";Count;E_{ratio};"  , 100, 0.0  , 1.05) );
  
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

    

    // Searching the hottest cell looking for EM2 layer
    for (const auto cell : **container.ptr() ){
      if( cell->sampling() != CaloSample::EM2 ) continue;
      float deltaEta = std::abs( seed.eta - cell->eta() );
      float deltaPhi = std::abs( CaloPhiRange::fix( seed.phi - cell->phi() ));
      if (deltaEta < m_etaWindow/2 && deltaPhi < m_phiWindow/2 && cell->energy() > emaxs2){
        hotcell=cell; emaxs2=cell->energy();
      }
    }

    xAOD::TruthParticle *particle = new xAOD::TruthParticle();
    xAOD::CaloCluster *clus=nullptr;
    
    particle->setEt( seed.et );
    particle->setEta( seed.eta );
    particle->setPhi( seed.phi );
    particle->setPdgid( seed.pdgid );

    if(hotcell){
      // Apply simple algorithm to check if most part of energy is not in the edges or not. Applying 0.1 X 0.1 window
      float etot=0.0;
      for (const auto cell : **container.ptr() ){
        if( cell->detector()!=CaloLayer::ECal ) continue;
        float deltaEta = std::abs( hotcell->eta() - cell->eta() );
        float deltaPhi = std::abs( CaloPhiRange::fix( hotcell->phi() - cell->phi() ));
        if (deltaEta < 0.05 && deltaPhi < 0.05){
          etot+=cell->energy();
        }
      }
      MSG_INFO( "Eletromagnetic energy in 0.1 x 0.1 center in the hotcell is: " << etot );
   
      if(etot >= m_minCenterEnergy ){
        MSG_INFO( "Creating one cluster since the center energy is higher than the energy cut" );
        clus = new xAOD::CaloCluster( hotcell->energy(), hotcell->eta(), hotcell->phi(), m_etaWindow/2., m_phiWindow/2. );
        fillCluster( ctx, clus, m_cellsKey );
        m_showerShapes->executeTool( clus );
      }
    }else{
      MSG_INFO( "There is not hottest cell for this particle.");
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
    
    float deltaEta = std::abs( clus->eta() - cell->eta() );
    float deltaPhi = std::abs( CaloPhiRange::fix( clus->phi() - cell->phi() ));
  
    if( deltaEta < m_etaWindow/2 && deltaPhi < m_phiWindow/2 ){
        // Add the cell to the cluster
        clus->push_back(cell);
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



  for( const auto& particle : **particles.ptr() ){
  

    if ( !particle->caloCluster() ) continue;
    MSG_INFO("============== Cluster Information ==============");

    const auto* clus = particle->caloCluster() ;

    store.hist1("cl_et")->Fill( clus->et() / 1.e3);
    store.hist1("cl_eta")->Fill( clus->eta() );
    store.hist1("cl_phi")->Fill( clus->phi() );
    store.hist1("cl_reta")->Fill( clus->reta() );
    store.hist1("cl_rphi")->Fill( clus->rphi() );
    store.hist1("cl_rhad")->Fill( clus->rhad() );
    store.hist1("cl_eratio")->Fill( clus->eratio() );
    store.hist1("cl_f1")->Fill( clus->f1() );
    store.hist1("cl_f3")->Fill( clus->f3() );
    store.hist1("cl_weta2")->Fill( clus->weta2() );
    store.hist1("res_et")->Fill( (particle->et() - clus->et()/1.e3) );
    store.hist1("res_eta")->Fill( (particle->eta() - clus->eta()) );
    store.hist1("res_phi")->Fill( (particle->phi() - clus->phi()) );
    
    MSG_INFO( "Truth Particle information:" );
    MSG_INFO( "Et       : " << particle->et() );
    MSG_INFO( "Eta      : " << particle->eta() );
    MSG_INFO( "Phi      : " << particle->phi() );
    MSG_INFO( "Cluster information:" );
    MSG_INFO( "Eta      : " << clus->eta()    );
    MSG_INFO( "Phi      : " << clus->phi()    );
    MSG_INFO( "Et       : " << clus->et()/1.e3);
    MSG_INFO( "e1       : " << clus->e1()     );
    MSG_INFO( "e2       : " << clus->e2()     );
    MSG_INFO( "e3       : " << clus->e3()     );
    MSG_INFO( "ehad1    : " << clus->ehad1()  );
    MSG_INFO( "ehad2    : " << clus->ehad2()  );
    MSG_INFO( "ehad3    : " << clus->ehad3()  );
    MSG_INFO( "etot     : " << clus->etot()   );
    MSG_INFO( "Reta     : " << clus->reta()   );
    MSG_INFO( "Rphi     : " << clus->rphi()   );
    MSG_INFO( "Rhad     : " << clus->rhad()   );
    MSG_INFO( "Eratio   : " << clus->eratio() );
    MSG_INFO( "f1       : " << clus->f1()     );
    MSG_INFO( "f3       : " << clus->f3()     );
    MSG_INFO( "Weta2    : " << clus->weta2()  );
 
    MSG_INFO("=================================================");
  }

  return StatusCode::SUCCESS;
}



float CaloClusterMaker::dR( float eta1, float phi1, float eta2, float phi2 ) const
{
  float deta = fabs(eta1-eta2);
  float dphi = fabs(phi1-phi2)  < TMath::Pi() ? fabs(phi1-phi2) : 2*TMath::Pi() - fabs(phi1-phi2) ;
  return sqrt( deta*deta + dphi*dphi);
}





