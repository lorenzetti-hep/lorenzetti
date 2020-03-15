
#include "CaloRec/CaloClusterMaker.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "core/macros.h"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

using namespace std;




CaloClusterMaker::CaloClusterMaker( std::string name ) : 
  AlgTool( name ),
  m_etaWindow( 0.4 ),
  m_phiWindow( 0.4 ),
  m_energyThreshold( 3*GeV ),
  m_forceTruthMatch(true),
  m_dR(0.15),
  m_inputCaloKey("xAOD__CaloCellContainer"),
  m_inputEventKey("xAOD__EventInfoContainer"),
  m_outputClusterKey("xAOD__CaloClusterContainer"),
  m_outputTruthKey("xAOD__TruthParticleContainer")
{;}



CaloClusterMaker::~CaloClusterMaker()
{;}


StatusCode CaloClusterMaker::initialize()
{
  auto store = getStoreGateSvc();
  
  store->mkdir( "cluster" );  

  store->AddHistogram("truth_eta", "Count;#eta;", 100, -1.5, 1.5);
  store->AddHistogram("truth_phi", "Count;#phi;", 100, -3.2, 3.2);
  store->AddHistogram("truth_eratio", "Count;E_{ratio};", 100, 0.0, 1.05);
  
  
  
  store->AddHistogram("et", "Count;E_{T};", 100, 0, 100);
  store->AddHistogram("eta", "Count;#eta;", 100, -1.5, 1.5);
  store->AddHistogram("phi", "Count;#phi;", 100, -3.2, 3.2);
  store->AddHistogram("eratio", "Count;E_{ratio};", 100, 0.0, 1.05);
  store->AddHistogram("reta", "Count;R_{#eta};", 100, 0.5, 1.1);
  store->AddHistogram("rphi", "Count;R_{#phi};", 100, 0.5, 1.1);
  store->AddHistogram("rhad", "Count;R_{had};", 100, 0.1, 1.1);
  

  store->AddHistogram("res_eta", "Count;res_{#eta};",100, -1, 1 );
  store->AddHistogram("res_phi", "Count;res_{#phi};",100, -3.2, 3.2 );
  store->AddHistogram("res_eratio", "Count;res_{E_{ratio}};",100, -1, 1 );

  
  return ErrorCode::SUCCESS;
}


StatusCode CaloClusterMaker::pre_execute( EventContext *ctx )
{
  m_eventInfoContainer=nullptr;
  m_caloCellContainer=nullptr;
  return ErrorCode::SUCCESS;
}

StatusCode CaloClusterMaker::execute( EventContext *ctx )
{
  return ErrorCode::SUCCESS;
}


StatusCode CaloClusterMaker::finalize()
{
  return ErrorCode::SUCCESS;
}



StatusCode CaloClusterMaker::post_execute( EventContext *ctx )
{
  // Retrieve the calo cluster container
  xAOD::CaloClusterContainer *caloClusterContainer=new xAOD::CaloClusterContainer();
  xAOD::TruthParticleContainer  *particleContainer= new xAOD::TruthParticleContainer();
  


  ctx->retrieve( m_eventInfoContainer , m_inputEventKey);
  bool forceTruthMatch = !m_eventInfoContainer? false : m_forceTruthMatch;


  ctx->retrieve( m_caloCellContainer , m_inputCaloKey);

  auto particles = getAllParticles();
  auto clusters = getAllClusters();

  if ( forceTruthMatch ){

    for( auto& caloCluster : clusters )
    {
      for( auto& particle : particles )
      {
        fillParticle( particle );
        calculate( particle );
    
        MSG_INFO( "DeltaR between particle and cluster is "<< dR(caloCluster, particle) );
        if( !particle->caloCluster() && dR( caloCluster, particle ) < m_dR ){
          fillCluster( caloCluster );
          calculate( caloCluster );
          particle->setCaloCluster( caloCluster );
          caloClusterContainer->push_back( caloCluster );
        }
        particleContainer->push_back( particle );
      }
    }

  }else{
    for( auto& caloCluster : clusters ){
      fillCluster( caloCluster );
      calculate( caloCluster );
      caloClusterContainer->push_back( caloCluster );
    }

    for( auto& particle : particles ){
      fillParticle( particle );
      calculate( particle );
      particleContainer->push_back( particle );
    }
  }



  MSG_INFO( "We found " << clusters.size() << " clusters (RoIs) inside of this event." );
  MSG_INFO( "We found " << particles.size() << " particles (seeds) inside of this event." );

  for (auto& particle : particles)
  {
    bool matched = particle->caloCluster()?true:false;
    MSG_INFO( "Particle in (eta="<<particle->eta() << ",phi="<< particle->phi()<< ") with " << particle->et()<< ". With cluster? " << (matched?"Yes":"No") );
  }


  ctx->attach( caloClusterContainer, m_outputClusterKey);
  ctx->attach( particleContainer, m_outputTruthKey);

  return ErrorCode::SUCCESS;
}




std::vector< xAOD::TruthParticle* > CaloClusterMaker::getAllParticles()
{
  std::vector< xAOD::TruthParticle* > particles;

  if(!m_eventInfoContainer)  return particles;

  // All seeds for this event
  auto event = m_eventInfoContainer->all().front();
 
  for ( auto& seed : event->allSeeds() )
  {
    xAOD::TruthParticle *particle = new xAOD::TruthParticle();
    particle->setEta( seed.eta );
    particle->setPhi( seed.phi );
    particle->setPdgid( seed.pdgid );
    particles.push_back( particle );
  }

  return particles;
}



std::vector< xAOD::CaloCluster* > CaloClusterMaker::getAllClusters() 
{


  std::vector<xAOD::CaloCluster*> vec_cluster;
  

  // Get all cells from the second calorimeter layer
  for (const auto &cell : m_caloCellContainer->all() )
  {
    // Only second layer cells
    if(cell->sampling() != CaloSampling::CaloSample::EM2)  continue;
    
    // Must be higher than energy cut to be considere an roi
    if (cell->energy() < m_energyThreshold ) continue;
 

    float eta = cell->eta(); 
    float phi = cell->phi(); 
    float emaxs2 =  cell->rawEnergy();
    
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


void CaloClusterMaker::fillCluster( xAOD::CaloCluster* caloCluster)
{

  // Attach all cells into your cluster
  for ( auto& cell : m_caloCellContainer->all() )
  {
    // Check if the current cell is in the eta window
    if( (cell->eta() < caloCluster->eta()+m_etaWindow/2.) && (cell->eta() > caloCluster->eta()-m_etaWindow/2.) )
    {
      // Check if the current cell is in the phi window
      if( (cell->phi() < caloCluster->phi()+m_phiWindow/2.) && (cell->phi() > caloCluster->phi()-m_phiWindow/2.) )
      {
        // Add the cell to the cluster
        caloCluster->push_back(cell);
      }
    }
  }// Loop over all cells

}



void CaloClusterMaker::fillParticle( xAOD::TruthParticle *particle )
{
  // Attach all cells into your particleter
  for ( auto& cell : m_caloCellContainer->all() )
  {
    // Check if the current cell is in the eta window
    if( (cell->eta() < particle->eta()+m_etaWindow/2.) && (cell->eta() > particle->eta()-m_etaWindow/2.) )
    {
      // Check if the current cell is in the phi window
      if( (cell->phi() < particle->phi()+m_phiWindow/2.) && (cell->phi() > particle->phi()-m_phiWindow/2.) )
      {
        // Add the cell to the particleter
        particle->push_back(cell);
      }
    }
  }// Loop over all cells
}






StatusCode CaloClusterMaker::fillHistograms(EventContext *ctx)
{
  auto store = getStoreGateSvc();
 
  /*
  if( m_forceTruthMatch )
  {
    const xAOD::TruthParticleContainer *container = nullptr; 
    ctx->retrieve( container, "" );

    for( auto& particle : container->all() )
    {
      store->hist1("cluster/truth_eta")->Fill( particle->eta() );
      store->hist1("cluster/truth_phi")->Fill( particle->phi() );

      //if ( particle->caloCluster() ){
      //  store->hist1("cluster/eta")->Fill( particle->caloCluster()->eta() );
      //  store->hist1("cluster/phi")->Fill( particle->caloCluster()->phi() );
      //}
    }

  }*/

  const xAOD::CaloClusterContainer *container = nullptr; 
  ctx->retrieve( container, m_outputClusterKey );

  for( const auto& clus : container->all() ){
    store->hist1("cluster/et")->Fill( clus->et() / 1000.);
    store->hist1("cluster/eta")->Fill( clus->eta() );
    store->hist1("cluster/phi")->Fill( clus->phi() );
    store->hist1("cluster/reta")->Fill( clus->reta() );
    store->hist1("cluster/rphi")->Fill( clus->rphi() );
    store->hist1("cluster/rhad")->Fill( clus->rhad() );
    store->hist1("cluster/eratio")->Fill( clus->eratio() );


  }


  return ErrorCode::SUCCESS;
}


float CaloClusterMaker::dR( xAOD::CaloCluster *clus, xAOD::TruthParticle *p )
{
  return sqrt( pow(clus->eta() - p->eta(),2) + pow( clus->phi() - p->phi() ,2));
}



float CaloClusterMaker::sumEnergy( xAOD::TruthParticle *particle, CaloSampling::CaloSample sampling, 
                                   unsigned eta_ncell, unsigned phi_ncell )
{
  float energy = 0.0;
  for ( const auto& cell : particle->allCells() )
  {
    if(cell->sampling() != sampling)  continue;
   

    // deta/dphi is the half of the cell sizei
    if( ( cell->eta() > ( particle->eta() - eta_ncell * cell->deltaEta() ) ) && 
        ( cell->eta() < ( particle->eta() + eta_ncell * cell->deltaEta() ) ) )
    {
      if( ( cell->phi() > ( particle->phi() - phi_ncell * cell->deltaPhi() ) ) && 
          ( cell->phi() < ( particle->phi() + phi_ncell * cell->deltaPhi() ) ) )
      {  
          energy+=cell->truthRawEnergy();
      }
    }
  }
  return energy;
}


float CaloClusterMaker::sumEnergy( xAOD::CaloCluster *clus, CaloSampling::CaloSample sampling, 
                                   unsigned eta_ncell, unsigned phi_ncell )
{
  float energy = 0.0;
  for ( const auto& cell : clus->allCells() )
  {
    if(cell->sampling() != sampling)  continue;
   

    // deta/dphi is the half of the cell sizei
    if( ( cell->eta() > ( clus->eta() - eta_ncell * cell->deltaEta() ) ) && 
        ( cell->eta() < ( clus->eta() + eta_ncell * cell->deltaEta() ) ) )
    {
      if( ( cell->phi() > ( clus->phi() - phi_ncell * cell->deltaPhi() ) ) && 
          ( cell->phi() < ( clus->phi() + phi_ncell * cell->deltaPhi() ) ) )
      {  
          energy+=cell->energy();
      }
    }
  }
  return energy;
}


float CaloClusterMaker::maxEnergy( xAOD::TruthParticle *particle, CaloSampling::CaloSample sampling, 
                                   const xAOD::CaloCell *&maxCell, bool exclude )
{
  float energy = 0.0;
  for ( const auto& cell : particle->allCells() )
  {
    if(cell->sampling() != sampling)  continue;
    // Useful strategy to get the second highest energy cell
    if( exclude && maxCell && cell==maxCell) continue;

    if ( cell->truthRawEnergy() > energy ){
      energy=cell->truthRawEnergy();
      if(!exclude)  maxCell = cell;
    }
  }
  
  return energy;
}


float CaloClusterMaker::maxEnergy( xAOD::CaloCluster *clus, CaloSampling::CaloSample sampling, 
                                   const xAOD::CaloCell *&maxCell , bool exclude)
{
  float energy = 0.0;
  for ( const auto& cell : clus->allCells() )
  {
    if(cell->sampling() != sampling)  continue;
    // Useful strategy to get the second highest energy cell
    if( exclude && maxCell && cell==maxCell)  continue;

    if ( cell->energy() > energy ){
      energy=cell->energy();
      if(!exclude)  maxCell = cell;
    }
  }
  return energy;
}



void CaloClusterMaker::calculate( xAOD::CaloCluster* clus )
{
  MSG_INFO("Calculate shower shapes for this cluster." );
  
  const xAOD::CaloCell *maxCell=nullptr;
  float emaxs1 = maxEnergy( clus, CaloSampling::CaloSample::EM1, maxCell );
  float e2tsts1 = maxEnergy( clus, CaloSampling::CaloSample::EM1, maxCell, true );
  MSG_INFO( "EMAX 1 = " << emaxs1 );
  MSG_INFO( "EMAX 2 = " << e2tsts1 );
  float eratio = (emaxs1 - e2tsts1)/(emaxs1 + e2tsts1);
  float e277 = sumEnergy( clus, CaloSampling::CaloSample::EM2, 7, 7 );
  float e233 = sumEnergy( clus, CaloSampling::CaloSample::EM2, 3, 3 );
  float e237 = sumEnergy( clus, CaloSampling::CaloSample::EM2, 3, 7 );
  float reta = e237/e277;
  float rphi = e233/e237;
  float e1 = sumEnergy( clus, CaloSampling::CaloSample::EM1 );
  float e2 = sumEnergy( clus, CaloSampling::CaloSample::EM2 );
  float e3 = sumEnergy( clus, CaloSampling::CaloSample::EM3 );
  float ehad1 = sumEnergy( clus, CaloSampling::CaloSample::HAD1 );
  float ehad2 = sumEnergy( clus, CaloSampling::CaloSample::HAD2 );
  float ehad3 = sumEnergy( clus, CaloSampling::CaloSample::HAD3 );

  float etot = e1+e2+e3+ehad1+ehad2+ehad3;

  // fraction of energy deposited in 1st sampling
  float f1 = e1 / etot;
  float f3 = e3 / etot;
  float rhad = (ehad1+ehad2+ehad3) / etot;

  clus->setE1( e1 );
  clus->setE2( e2 );
  clus->setE3( e3 );
  clus->setEhad1( ehad1 );
  clus->setEhad2( ehad2 );
  clus->setEhad3( ehad3 );
  clus->setEtot( etot );
  clus->setE277( e277 );
  clus->setE237( e237 );
  clus->setE233( e233 );
  clus->setReta( reta );
  clus->setRphi( rphi );
  clus->setEratio( eratio );
  clus->setEmaxs1( emaxs1 );
  clus->setE2tsts1( e2tsts1 );
  clus->setF1( f1 );
  clus->setF3( f3 );
  clus->setRhad( rhad );
  clus->setEt( clus->eta() != 0.0 ? clus->etot()/cosh(fabs(clus->eta())) : 0.0 ); 
  clus->Print();
}


void CaloClusterMaker::calculate( xAOD::TruthParticle *particle )
{
  MSG_INFO("Calculate shower shapes for this particle." );
  
  const xAOD::CaloCell *maxCell=nullptr;
  
  float emaxs1 = maxEnergy( particle, CaloSampling::CaloSample::EM2, maxCell );
  float e2tsts1 = maxEnergy( particle, CaloSampling::CaloSample::EM2, maxCell, true );
  float eratio = (emaxs1 - e2tsts1)/(emaxs1 + e2tsts1);


  float e277 = sumEnergy( particle, CaloSampling::CaloSample::EM2, 7, 7 );
  float e233 = sumEnergy( particle, CaloSampling::CaloSample::EM2, 3, 3 );
  float e237 = sumEnergy( particle, CaloSampling::CaloSample::EM2, 3, 7 );

  float reta = e237/e277;
  float rphi = e233/e237;
  
  float e1 = sumEnergy( particle, CaloSampling::CaloSample:: EM1 );
  float e2 = sumEnergy( particle, CaloSampling::CaloSample:: EM2 );
  float e3 = sumEnergy( particle, CaloSampling::CaloSample:: EM3 );

  float ehad1 = sumEnergy( particle, CaloSampling::CaloSample::HAD1 );
  float ehad2 = sumEnergy( particle, CaloSampling::CaloSample::HAD2 );
  float ehad3 = sumEnergy( particle, CaloSampling::CaloSample::HAD3 );

  float etot = e1+e2+e3+ehad1+ehad2+ehad3;

  //fraction of energy deposited in 1st sampling
  float f1 = e1 / etot;
  float f3 = e3 / etot;
  float rhad = (ehad1+ehad2+ehad3) / etot;

  particle->setEtot( etot );
  particle->setReta( reta );
  particle->setRphi( rphi );
  particle->setEratio( eratio );
  particle->setEmaxs1( emaxs1 );
  particle->setE2tsts1( e2tsts1 );
  particle->setF1( f1 );
  particle->setF3( f3 );
  particle->setRhad( rhad );
  particle->setEt( particle->eta() != 0.0 ? particle->etot()/cosh(fabs(particle->eta())) : 0.0 ); 

  particle->Print();
}




