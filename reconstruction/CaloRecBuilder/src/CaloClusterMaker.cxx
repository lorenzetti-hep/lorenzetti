
#include "G4Kernel/CaloPhiRange.h"
#include "CaloCellCollection.h"
#include "CaloClusterMaker.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "TH1F.h"
#include "TH2F.h"

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
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
}



CaloClusterMaker::~CaloClusterMaker()
{;}


StatusCode CaloClusterMaker::initialize()
{
  setMsgLevel(m_outputLevel);
  m_showerShapes = new ShowerShapes( "ShowerShapes" );
  return StatusCode::SUCCESS;
}


StatusCode CaloClusterMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();

  store->mkdir( m_histPath );  
  store->add( new TH1F("cl_et"   , ";Count;E_{T};"       , 100, 0.0  , 100 ) );
  store->add( new TH1F("cl_eta"  , ";Count;#eta;"        , 100, -1.5 , 1.5 ) );
  store->add( new TH1F("cl_phi"  , ";Count;#phi;"        , 100, -3.2 , 3.2 ) );
  store->add( new TH1F("cl_f1"   , ";Count;f_{1};"       , 100, -0.02, 0.7 ) );
  store->add( new TH1F("cl_f3"   , ";Count;f_{3};"       , 200, -0.05, 0.15) );
  store->add( new TH1F("cl_weta2", ";Count;W_{#eta2};"   , 100, 0.005, 0.02) );
  store->add( new TH1F("cl_reta" ,  ";Count;R_{#eta};"   , 200, 0.8  , 1.10) );
  store->add( new TH1F("cl_rphi" , ";Count;R_{#phi};"    , 200, 0.45 , 1.05) );
  store->add( new TH1F("cl_rhad" , ";Count;R_{had};"     , 200, -0.05, 0.05) );
  store->add( new TH1F("cl_eratio", ";Count;E_{ratio};"  , 100, 0.0  , 1.05) );
  store->add( new TH1F("res_eta", "#eta_{Cluster}-#eta_{Truth}/#eta_{Truth};res_{#eta};Count",100,-1.5,1.5 ) );
  store->add( new TH1F("res_phi", "#phi_{Cluster}-#phi_{Truth}/#phi_{Truth};res_{#phi};Count",100,-1.5,1.5 ) );
  store->add( new TH1F("res_e157"  ,"(E_{1}-E_{Truth})/E_{Truth};res_{E_{1}};Count",100,-1.5,1.5) );
  store->add( new TH1F("res_e257"  ,"(E_{2}-E_{Truth})/E_{Truth};res_{E_{2}};Count",100,-1.0,1.0) );
  store->add( new TH1F("res_e357"  ,"(E_{3}-E_{Truth})/E_{Truth};res_{E_{3}};Count",100,-1.5,1.5) );
  store->add( new TH1F("res_etot57","(E_{tot}-E_{Truth})/E_{Truth};res_{E_{tot}};Count",100,-0.4,0.4) );
  store->add( new TH2F("res_etot57vsEt","(E_{tot}-E_{Truth})/E_{Truth} Vs E_{T};res_{E_{tot}}; E_{T}; Count",100,-10,10, 100, 0 , 100) );

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

 
  MSG_DEBUG( "Associate all truth particles and clusters");
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

  auto *evt = (**event.ptr()).front();

  for ( auto& seed : evt->allSeeds() )
  {
    const xAOD::CaloCell *hotcell=nullptr;
    float emaxs2 = 0.0;

    // Searching the hottest cell looking for EM2 layer
    for (const auto cell : **container.ptr() ){
      if( cell->layer() != CaloLayer::EM2 ) continue;
      float deltaEta = std::abs( seed.eta - cell->eta() );
      float deltaPhi = std::abs( CaloPhiRange::diff( seed.phi , cell->phi() ));
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
        if( cell->section()!=CaloSection::ECal ) continue;
        float deltaEta = std::abs( hotcell->eta() - cell->eta() );
        float deltaPhi = std::abs( CaloPhiRange::diff( hotcell->phi() , cell->phi() ));
        if (deltaEta < 0.05 && deltaPhi < 0.05){
          etot+=cell->energy();
        }
      }
      MSG_INFO( "Eletromagnetic energy in 0.1 x 0.1 center in the hotcell is: " << etot );
      MSG_DEBUG( "Is cluster energy higher than " << m_minCenterEnergy ); 
      if(etot >= m_minCenterEnergy ){
        MSG_INFO( "Creating one cluster since the center energy is higher than the energy cut" );
        clus = new xAOD::CaloCluster( hotcell->energy(), hotcell->eta(), hotcell->phi(), m_etaWindow/2., m_phiWindow/2. );
        fillCluster( ctx, clus, m_cellsKey );
        m_showerShapes->executeTool( evt, clus );
      }
    }else{
      MSG_DEBUG( "There is not hottest cell for this particle.");
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
    float deltaPhi = std::abs( CaloPhiRange::diff( clus->phi() , cell->phi() ));
    if( deltaEta < m_etaWindow/2 && deltaPhi < m_phiWindow/2 ){
        // Add the cell to the cluster
        clus->push_back(cell);
    }
  }// Loop over all cells
}


StatusCode CaloClusterMaker::fillHistograms(EventContext &ctx ) const
{

  MSG_DEBUG( "Fill all histograms" );
  
  auto store = ctx.getStoreGateSvc();

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

  MSG_DEBUG( "We found " << clusters->size() << " clusters (RoIs) inside of this event." );
  MSG_DEBUG( "We found " << particles->size() << " particles (seeds) inside of this event." );

  store->cd(m_histPath);



  for( const auto& particle : **particles.ptr() ){
  

    if ( !particle->caloCluster() ) continue;
    MSG_DEBUG("============== Cluster Information ==============");

    const auto* clus = particle->caloCluster() ;
    
    
    store->hist1("res_eta")->Fill( (clus->eta() - particle->eta())/particle->eta() );
    store->hist1("res_phi")->Fill( (clus->phi() - particle->phi())/particle->phi() );

    {
      // Calculate the energy resolution between the estimated energy by the OF and the truth
      // energy calculated by the geant.
      float raw_e157 = sumEnergy(clus, CaloLayer::EM1, 5, 7, true);
      float e157 = sumEnergy(clus, CaloLayer::EM1, 5, 7);
      store->hist1("res_e157")->Fill( (e157-raw_e157)/raw_e157 );
      float raw_e257 = sumEnergy(clus, CaloLayer::EM2, 5, 7, true);
      float e257 = sumEnergy(clus, CaloLayer::EM2, 5, 7);
      store->hist1("res_e257")->Fill( (e257-raw_e257)/raw_e257 );
      float raw_e357 = sumEnergy(clus, CaloLayer::EM3, 5, 7, true);
      float e357 = sumEnergy(clus, CaloLayer::EM3, 5, 7);
      store->hist1("res_e357")->Fill( (e357-raw_e357)/raw_e357 );
      float etot57=e157+e257+e357;
      float raw_etot57=raw_e157+raw_e257+raw_e357;
      store->hist1("res_etot57")->Fill( (etot57-raw_etot57)/raw_etot57 );
      
      
      store->hist2("res_etot57vsEt")->Fill( (etot57-raw_etot57)*1e-3, clus->et()*1e-3 );
    
    
    }


    store->hist1("cl_et")->Fill( clus->et() / 1.e3);
    store->hist1("cl_eta")->Fill( clus->eta() );
    store->hist1("cl_phi")->Fill( clus->phi() );
    store->hist1("cl_reta")->Fill( clus->reta() );
    store->hist1("cl_rphi")->Fill( clus->rphi() );
    store->hist1("cl_rhad")->Fill( clus->rhad() );
    store->hist1("cl_eratio")->Fill( clus->eratio() );
    store->hist1("cl_f1")->Fill( clus->f1() );
    store->hist1("cl_f3")->Fill( clus->f3() );
    store->hist1("cl_weta2")->Fill( clus->weta2() );


    
    
    MSG_DEBUG( "Truth Particle information:" );
    MSG_DEBUG( "Et       : " << particle->et() );
    MSG_DEBUG( "Eta      : " << particle->eta() );
    MSG_DEBUG( "Phi      : " << particle->phi() );
    MSG_DEBUG( "Cluster information:" );
    MSG_DEBUG( "Eta      : " << clus->eta()    );
    MSG_DEBUG( "Phi      : " << clus->phi()    );
    MSG_DEBUG( "Et       : " << clus->et()/1.e3);
    MSG_DEBUG( "e1       : " << clus->e1()     );
    MSG_DEBUG( "e2       : " << clus->e2()     );
    MSG_DEBUG( "e3       : " << clus->e3()     );
    MSG_DEBUG( "ehad1    : " << clus->ehad1()  );
    MSG_DEBUG( "ehad2    : " << clus->ehad2()  );
    MSG_DEBUG( "ehad3    : " << clus->ehad3()  );
    MSG_DEBUG( "etot     : " << clus->etot()   );
    MSG_DEBUG( "Reta     : " << clus->reta()   );
    MSG_DEBUG( "Rphi     : " << clus->rphi()   );
    MSG_DEBUG( "Rhad     : " << clus->rhad()   );
    MSG_DEBUG( "Eratio   : " << clus->eratio() );
    MSG_DEBUG( "f1       : " << clus->f1()     );
    MSG_DEBUG( "f3       : " << clus->f3()     );
    MSG_DEBUG( "Weta2    : " << clus->weta2()  );
    MSG_DEBUG("=================================================");
  }

  return StatusCode::SUCCESS;
}



float CaloClusterMaker::dR( float eta1, float phi1, float eta2, float phi2 ) const
{
  float deta = fabs(eta1-eta2);
  float dphi = fabs(phi1-phi2)  < TMath::Pi() ? fabs(phi1-phi2) : 2*TMath::Pi() - fabs(phi1-phi2) ;
  return sqrt( deta*deta + dphi*dphi);
}


float CaloClusterMaker::sumEnergy( const xAOD::CaloCluster *clus, CaloLayer layer, unsigned eta_ncell, unsigned phi_ncell, bool raw ) const
{
  float energy = 0.0;
  for ( const auto& cell : clus->allCells() )
  {
    if(cell->layer() != layer)  continue;
    float deltaEta = std::abs( clus->eta() - cell->eta() );
    float deltaPhi = std::abs( CaloPhiRange::fix( clus->phi() - cell->phi() ) );
    
    if( deltaEta < eta_ncell*cell->deltaEta() && deltaPhi < phi_ncell*cell->deltaPhi() ){
      energy+= raw? cell->parent()->truthRawEnergy():cell->energy();
    }
  }
  return energy;
}


