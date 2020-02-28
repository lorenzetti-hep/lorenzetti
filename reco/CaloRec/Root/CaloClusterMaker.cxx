
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
  getContext()->attach( new xAOD::CaloClusterContainer() );
  return SUCCESS;
}


StatusCode CaloClusterMaker::pre_execute( EventContext *ctx )
{
  xAOD::CaloClusterContainer *caloClusterCont=nullptr;

  ctx->retrieve( caloClusterCont );

  if( caloClusterCont )
    caloClusterCont->clear();

  return SUCCESS;

}


StatusCode CaloClusterMaker::post_execute( EventContext *ctx )
{
  // Retrieve the calo cell collection
  xAOD::CaloCellCollection * collection=nullptr;
  ctx->retrieve( collection );

  // Retrieve the calo cluster container
  xAOD::CaloClusterContainer *caloClusterContainer=nullptr;
  ctx->retrieve( caloClusterContainer );
 

  xAOD::TruthContainer  *truthContainer=nullptr;
  ctx->retrieve( truthContainer );


  if( !collection )
  {
    MSG_ERROR("It's not possible to retrieve the calo cell collection.");
    return FAILURE;
  }
 
  if( !caloClusterCont )
  {
    MSG_ERROR("It's not possible to retrieve the calo cluster container");
    return FAILURE;
  }

  
  // Get all clusters
  auto clusters = getAllClusters( collection, truthContainer );

  // Associated each cell to your cluster
  fillClusters( collection, clusters );



  // Pass the memory control to the container
  for ( auto& cl : clusters )
    caloClusterContainer->push_back( cl );


  return SUCCESS;
}





StatusCode CaloClusterMaker::fill( EventContext * /*ctx*/ )
{
  return SUCCESS;
}



std::vector < xAOD::CaloCluster > CaloClusterMaker::getAllClusters( xAOD::CaloCellCollection *collection )
{


  std::vector<xAOD::CaloCluster*> vec_cluster;
  
  // Get all cells from the second calorimeter layer
  for (auto &cell : collection->getCollection(CaloSampling::CaloSample::EM2) )
  {

    if (cell->rawEnergy() < m_energyThreshold ) continue;


    if ( truthMatched ){
      bool matched=false; 
      for ( const auto& truth : truthContainer.cptr() ){
        if (  (abs(truth->eta() - cell->eta()) < m_etaWindow/2 ) && ( abs(truth->phi()-cell->phi()) < m_phiWindow/2) )
          matched=true;
      }

      if ( ! matched )  continue;
    }


    if (vec_cluster.empty()){
      // Build the first cluster
      vec_cluster.push_back ( new xAOD::CaloCluster( cell->rawEnergy(), cell->eta(), cell->phi(), m_etaWindow/2, m_phiWindow/2 ) );
    }else{
      // Check cluster overlap. Get the higher energy cluster energy in case of overlap
      float eta = cell->eta(); float phi = cell->phi(); float energy =  cell->rawEnergy();
      for( unsigned int i=0; i < vec_roi.size(); ++i )
      {
        if (abs(eta - vec_roi[i]->eta())<m_etaWindow/2 && abs( phi - vec_roi[i]->phi())<m_phiWindow/2 ){
          if (energy > vec_roi[i]->energyCenter())
            // Destroy the old one and avoid memory leak
            delete vec_cluster[i];
            // Create the new one
            vec_cluster[i] = new xAOD::CaloCluster(eta, phi, m_etaWindow/2, m_phiWindow/2, energy);
        }
      }
    }
  }
  
  return vec_cluster;
}



void CaloClusterMaker::fillClusters( std::vector< xAOD::CaloCluster* > &clusters )
{


  // Attach all cells into your cluster
  for ( auto& cell : collection->getCollection() )
  {
    for (auto & caloCluster : vec_roi){
      // Check if the current cell is in the eta window
      if( (cell->eta() < caloCluster->eta()+m_etaWindow/2) && (cell->eta() > caloCluster->eta()-m_etaWindow/2) )
      {
        // Check if the current cell is in the phi window
        if( (cell->phi() < caloCluster->phi()+m_phiWindow/2) && (cell->phi() > caloCluster->phi()-m_phiWindow/2) )
        {
          // Add the cell to the cluster
          caloCluster->push_back(cell);
        }
      }
    }
  }// Loop over all cells






