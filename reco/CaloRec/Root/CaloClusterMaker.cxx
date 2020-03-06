
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
  m_energyThreshold( 3*GeV )
{;}



CaloClusterMaker::~CaloClusterMaker()
{;}


StatusCode CaloClusterMaker::initialize()
{
  auto store = getStoreGateSvc();
  
  
  store->AddHistogram("eratio", "", 100, 0.0, 1.0);
  
  
  return ErrorCode::SUCCESS;
}


StatusCode CaloClusterMaker::pre_execute( EventContext *ctx )
{
  return ErrorCode::SUCCESS;
}


StatusCode CaloClusterMaker::post_execute( EventContext *ctx )
{
  // Retrieve the calo cluster container
  xAOD::CaloClusterContainer *caloClusterContainer=new xAOD::CaloClusterContainer();

  const xAOD::TruthContainer  *truthContainer=nullptr;
  ctx->retrieve( truthContainer );


  const xAOD::CaloCellContainer *caloCellContainer=nullptr;
  ctx->retrieve( caloCellContainer );

  // Get all clusters
  auto clusters = getAllClusters( caloCellContainer, truthContainer );
  // Fill all clusters with cells inside of the window
  fillClusters( clusters, caloCellContainer );
  // Extract all shower shapes and energy parameters
  calculate( clusters );


  for (auto& clus : clusters) 
  {
    MSG_INFO( "eta = " << clus->eta() << " phi = " << clus->phi() << " emaxs2 = " << clus->emaxs2() << " size " << clus->size() );
    caloClusterContainer->push_back( clus );
  }



  ctx->attach( caloClusterContainer);
  return ErrorCode::SUCCESS;
}





std::vector < xAOD::CaloCluster* > CaloClusterMaker::getAllClusters( const xAOD::CaloCellContainer *container , 
                                                                    const xAOD::TruthContainer *truthContainer )
{


  std::vector<xAOD::CaloCluster*> vec_cluster;
  
  // Get all cells from the second calorimeter layer
  for (const auto &cell : container->all() )
  {
    // Only second layer cells
    if(cell->sampling() != CaloSampling::CaloSample::EM2)  continue;
    
    // Must be higher than energy cut to be considere an roi
    if (cell->rawEnergy() < m_energyThreshold ) continue;
 

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

    if(newCluster)
      vec_cluster.push_back( new xAOD::CaloCluster( emaxs2, eta, phi, m_etaWindow/2., m_phiWindow/2. ) );
    
  }
  
  return vec_cluster;
}


void CaloClusterMaker::fillClusters( std::vector< xAOD::CaloCluster* > vec_cluster,  const xAOD::CaloCellContainer *container )
{


  // Attach all cells into your cluster
  for ( auto& cell : container->all() )
  {
    for (auto & caloCluster : vec_cluster){

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
    }// Loop over cluster
  }// Loop over all cells

}


void CaloClusterMaker::calculate( std::vector< xAOD::CaloCluster* > vec_cluster )
{

  for (auto& clus : vec_cluster )
  {

    const xAOD::CaloCell *maxCell=nullptr;
    float emaxs1 = maxEnergy( clus, CaloSampling::CaloSample::EM1, maxCell );
    float e2tsts1 = maxEnergy( clus, CaloSampling::CaloSample::EM1, maxCell, true );
    float eratio = (emaxs1 - e2tsts1)/(emaxs1 + e2tsts1);


    float e277 = sumEnergy( clus, CaloSampling::CaloSample::EM2, 7, 7 );
    float e233 = sumEnergy( clus, CaloSampling::CaloSample::EM2, 3, 3 );
    float e237 = sumEnergy( clus, CaloSampling::CaloSample::EM2, 3, 7 );

    float reta = e237/e277;
    float rphi = e233/e237;
    
    float e1 = sumEnergy( clus, CaloSampling::CaloSample:: EM1 );
    float e2 = sumEnergy( clus, CaloSampling::CaloSample:: EM2 );
    float e3 = sumEnergy( clus, CaloSampling::CaloSample:: EM3 );

    float ehad1 = sumEnergy( clus, CaloSampling::CaloSample::HAD1 );
    float ehad2 = sumEnergy( clus, CaloSampling::CaloSample::HAD2 );
    float ehad3 = sumEnergy( clus, CaloSampling::CaloSample::HAD3 );

    float etot = e1+e2+e3+ehad1+ehad2+ehad3;

    //fraction of energy deposited in 1st sampling
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

    // Shower shapes
    clus->setReta( reta );
    clus->setRphi( rphi );
    clus->setEratio( eratio );
    clus->setEmaxs1( emaxs1 );
    clus->setE2tsts1( e2tsts1 );
    clus->setF1( f1 );
    clus->setF3( f3 );
    clus->setRhad( rhad );

  }

}




float CaloClusterMaker::sumEnergy( xAOD::CaloCluster *clus, CaloSampling::CaloSample sampling, unsigned eta_ncell, unsigned phi_ncell )
{
  float energy = 0.0;

  for ( const auto& cell : clus->all() )
  {
    if(cell->sampling() != sampling)  continue;
    // deta/dphi is the half of the cell size
    if( ( cell->eta() < ( eta_ncell * cell->deltaEta() ) ) && ( cell->phi() < ( phi_ncell * cell->deltaPhi() ) ) )  
      energy+=cell->rawEnergy();
  }
  return energy;
}



float CaloClusterMaker::maxEnergy( xAOD::CaloCluster *clus, CaloSampling::CaloSample sampling, const xAOD::CaloCell *maxCell , bool exclude)
{
  float energy = 0.0;
  for ( const auto& cell : clus->all() )
  {
    if(cell->sampling() != sampling)  continue;
    // Useful strategy to get the second highest energy cell
    if( exclude && maxCell && cell==maxCell)  continue;

    if ( cell->rawEnergy() > energy ){
      energy=cell->rawEnergy();
      if(!exclude)  maxCell = cell;
    }
  }
  return energy;
}




StatusCode CaloClusterMaker::fillHistograms(EventContext *ctx)
{
  const xAOD::CaloClusterContainer *container = nullptr; 
  auto store = getStoreGateSvc();

  ctx->retrieve( container, "" );

  for( auto& clus : container->all() )
  {
    store->hist1("eratio")->Fill( clus->eratio() );
  }


  return ErrorCode::SUCCESS;
}
