
#include "G4Kernel/CaloPhiRange.h"
#include "CaloClusterMaker.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

using namespace SG;
using namespace Gaugi;



CaloClusterMaker::CaloClusterMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  // Key inputs
  declareProperty( "CellsKey"       , m_cellsKey="Cells"                );
  declareProperty( "EventKey"       , m_eventKey="EventInfo"            );
  declareProperty( "TruthKey"       , m_truthKey="Particles"            );
  // Key outputs
  declareProperty( "ClusterKey"     , m_clusterKey="Clusters"           );
  // Algorithm configuration
  declareProperty( "EtaWindow"      , m_etaWindow=0.4                   );
  declareProperty( "PhiWindow"      , m_phiWindow=0.4                   );
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
  declareProperty( "HistogramPath"  , m_histPath="Clusters"             );
  declareProperty( "MinCenterEnergy", m_minCenterEnergy=15*GeV          );
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
}

//!=====================================================================

CaloClusterMaker::~CaloClusterMaker()
{
  delete m_showerShapes;  
}

//!=====================================================================

StatusCode CaloClusterMaker::initialize()
{
  CHECK_INIT();
  setMsgLevel(m_outputLevel);
  m_showerShapes = new ShowerShapes( "ShowerShapes" );
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloClusterMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();

  store->mkdir( m_histPath );  
  store->add( new TH1F("cl_e"   , ";Count;E_{T};"       , 100, 0.0  , 200 ) );
  store->add( new TH1F("cl_et"   , ";Count;E_{T};"       , 100, 0.0  , 200 ) );
  store->add( new TH1F("cl_eta"  , ";Count;#eta;"        , 100, -1.5 , 1.5 ) );
  store->add( new TH1F("cl_phi"  , ";Count;#phi;"        , 100, -3.2 , 3.2 ) );
  store->add( new TH1F("cl_f1"   , ";Count;f_{1};"       , 100, -0.02, 0.7 ) );
  store->add( new TH1F("cl_f3"   , ";Count;f_{3};"       , 200, -0.05, 0.15) );
  store->add( new TH1F("cl_weta2", ";Count;W_{#eta2};"   , 100, 0.005, 0.02) );
  store->add( new TH1F("cl_reta" ,  ";Count;R_{#eta};"   , 200, 0.8  , 1.10) );
  store->add( new TH1F("cl_rphi" , ";Count;R_{#phi};"    , 200, 0.45 , 1.05) );
  store->add( new TH1F("cl_rhad" , ";Count;R_{had};"     , 200, -0.05, 0.05) );
  store->add( new TH1F("cl_eratio", ";Count;E_{ratio};"  , 100, 0.0  , 1.05) );

  store->add( new TH1F("res_eta", "#eta_{Cluster}-#eta_{Truth};res_{#eta};Count",100,-1.5,1.5 ) );
  store->add( new TH1F("res_phi", "#phi_{Cluster}-#phi_{Truth};res_{#phi};Count",100,-1.5,1.5 ) );
  store->add( new TH1F("res_e"  , "E_{Cluster}-E_{Truth};res_{e} [MeV];Count",100,-10*GeV,10*GeV ) );
  store->add( new TH1F("res_et" , "E_{T,Cluster}-E_{T,Truth};res_{Et} [MeV];Count",100,-10*GeV,10*GeV ) );


  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloClusterMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloClusterMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloClusterMaker::execute( EventContext &ctx, int /*evt*/ ) const
{
  return post_execute(ctx);
}

//!=====================================================================

StatusCode CaloClusterMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloClusterMaker::post_execute( EventContext &ctx ) const
{

  SG::WriteHandle<xAOD::CaloClusterContainer> clusters(m_clusterKey, ctx);
  clusters.record( std::unique_ptr<xAOD::CaloClusterContainer>(new xAOD::CaloClusterContainer()) );
  
  // Event info
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);
  SG::ReadHandle<xAOD::TruthParticleContainer> particles(m_truthKey, ctx);
  SG::ReadHandle<xAOD::CaloCellContainer> container(m_cellsKey, ctx);
  

  if( !event.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
  }

  if( !particles.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::TruthParticleContainer from this Context" );
  }

  if( !container.isValid() )
  {
    MSG_FATAL("It's not possible to read the xAOD::CaloCellContainer from this Contaxt using this key " << m_cellsKey );
  }

  MSG_DEBUG( "Associate all truth particles and clusters");

  // Loop over all truth particles (here, we have seeds)
  for ( const auto part : **particles.ptr() )
  {
    const xAOD::CaloCell *hotcell=nullptr;
    float emaxs2 = 0.0;

    // Searching the hottest cell looking for EM2 layer
    for (const auto cell : **container.ptr() ){

      const xAOD::CaloDetDescriptor* det = cell->descriptor();

      
      // Must be EM2 cells layer
      if( (det->sampling() != CaloSampling::EMB2) && (det->sampling() != CaloSampling::EMEC2) ) continue;      
      // Check if cell is inside 
      float deltaEta = std::abs( part->eta() - cell->eta() );
      float deltaPhi = std::abs( CaloPhiRange::diff( part->phi() , cell->phi() ));
      if (deltaEta < m_etaWindow/2 && deltaPhi < m_phiWindow/2 && cell->e() > emaxs2){
        hotcell=cell; emaxs2=cell->e();
      }
    }

    if(hotcell){

      // Apply simple algorithm to check if most part of energy is not in the edges or not. Applying 0.1 X 0.1 window
      float etot=0.0;
      for (const auto cell : **container.ptr() ){

        // Only PS, EM1, EM2 and EM3 cells
        if( cell->descriptor()->detector()!=Detector::TTEM ) continue;
        
        float deltaEta = std::abs( hotcell->eta() - cell->eta() );
        float deltaPhi = std::abs( CaloPhiRange::diff( hotcell->phi() , cell->phi() ));
        // 0.1 X 0.1 window
        if (deltaEta < 0.05 && deltaPhi < 0.05){
          etot+=cell->e();
        }
      }


      MSG_INFO( "Eletromagnetic energy in 0.1 x 0.1 center in the hotcell is: " << etot );

      MSG_DEBUG( "Is cluster energy higher than " << m_minCenterEnergy ); 
      if(etot >= m_minCenterEnergy ){
        MSG_INFO( "Creating one cluster since the center energy is higher than the energy cut" );
        xAOD::CaloCluster *clus = new xAOD::CaloCluster( hotcell->e(), hotcell->eta(), hotcell->phi(), m_etaWindow/2., m_phiWindow/2. );
        fillCluster( ctx, clus, m_cellsKey );
        m_showerShapes->execute( ctx, clus );
        clusters->push_back( clus );
      }
    }else{
      MSG_DEBUG( "There is not hottest cell for this particle.");
    }

  }
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloClusterMaker::fillHistograms(EventContext &ctx ) const
{

  MSG_DEBUG( "Fill all histograms" );
  
  auto store = ctx.getStoreGateSvc();

  SG::ReadHandle<xAOD::CaloClusterContainer> clusters( m_clusterKey, ctx );

  if( !clusters.isValid() ){
    MSG_ERROR("It's not possible to get the CaloClusterContainer inside of this Context using this key: " << m_clusterKey);
    return StatusCode::FAILURE;
  }

  MSG_DEBUG( "We found " << clusters->size() << " clusters (RoIs) inside of this event." );

  SG::ReadHandle<xAOD::TruthParticleContainer> particles(m_truthKey, ctx);
  if( !particles.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::TruthParticleContainer from this Context" );
  }

  store->cd(m_histPath);

  for( const auto& clus : **clusters.ptr() ){
  
    
    const xAOD::TruthParticle *particle=nullptr;

    // TODO: Probably some c++ expert can reduce this **var.ptr(), too verbose...
    // Try to find the associated truth particle
    for(const auto& part : **particles.ptr() ){
      if( dR(clus->eta(), clus->phi(), part->eta(), part->phi() ) < 0.4){
        particle=part;
        break;
      }
    }



    MSG_DEBUG("============== Cluster Information ==============");


    if(particle){
      store->hist1("res_eta")->Fill( (clus->eta() - particle->eta()) );
      store->hist1("res_phi")->Fill( (clus->phi() - particle->phi()) );
      // estimated transverse energy and truth
      store->hist1("res_et")->Fill( (clus->et() - particle->et() ) );
      // estimated energy and truth
      store->hist1("res_e")->Fill( (clus->e() - particle->e()) );
      MSG_DEBUG( "Truth Particle information:" );
      MSG_DEBUG( "E        : " << particle->e() );
      MSG_DEBUG( "Et       : " << particle->et() );
      MSG_DEBUG( "Eta      : " << particle->eta() );
      MSG_DEBUG( "Phi      : " << particle->phi() );
    }

    store->hist1("cl_e")->Fill( clus->e() / GeV);
    store->hist1("cl_et")->Fill( clus->et() / GeV);
    store->hist1("cl_eta")->Fill( clus->eta() );
    store->hist1("cl_phi")->Fill( clus->phi() );
    store->hist1("cl_reta")->Fill( clus->reta() );
    store->hist1("cl_rphi")->Fill( clus->rphi() );
    store->hist1("cl_rhad")->Fill( clus->rhad() );
    store->hist1("cl_eratio")->Fill( clus->eratio() );
    store->hist1("cl_f1")->Fill( clus->f1() );
    store->hist1("cl_f3")->Fill( clus->f3() );
    store->hist1("cl_weta2")->Fill( clus->weta2() );



    MSG_DEBUG( "Cluster information:" );
    MSG_DEBUG( "Eta      : " << clus->eta()    );
    MSG_DEBUG( "Phi      : " << clus->phi()    );
    MSG_DEBUG( "Et       : " << clus->et()     );
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

//!=====================================================================

float CaloClusterMaker::dR( float eta1, float phi1, float eta2, float phi2 ) const
{
  float deta = fabs(eta1-eta2);
  float dphi = fabs(phi1-phi2)  < TMath::Pi() ? fabs(phi1-phi2) : 2*TMath::Pi() - fabs(phi1-phi2) ;
  float dr = sqrt( deta*deta + dphi*dphi);
  MSG_DEBUG( "dR = " << dr);
  return dr;
}

//!=====================================================================

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




