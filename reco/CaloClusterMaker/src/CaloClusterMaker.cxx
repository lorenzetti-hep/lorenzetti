
#include "CaloClusterMaker.hh"
#include "CaloCell.hh"
#include "CaloCluster.hh"
#include "G4PhysicalConstants.hh"
#include "macros.hh"

using namespace std;




CaloClusterMaker::CaloClusterMaker( std::string &name ) : 
  IAlgTool( name ),
  m_etaWindow( 0.4 ),
  m_phiWindow( 0.4 ),
  m_energyThreshold( 3*GeV )
{;}



CaloClusterMaker::~CaloClusterMaker()
{;}


CaloClusterMaker::initialize()
{
  getContext()->attach( new xAOD::CaloClusterContainer() );
}


bool CaloClusterMaker::preExecute( EventContext *ctx )
{
  xAOD::CaloClusterContainer *caloClusterCont=nullptr;

  ctx->retrieve( caloClusterCont );

  if( caloClusterCont )
    caloClusterCont->clear();

}


StatusCode CaloClusterMaker::postExecute( EventContext *ctx )
{
  // Retrieve the calo cell collection
  const xAOD::CaloCellCollection * collection=nullptr;
  ctx->retrieve( collection );

  // Retrieve the calo cluster container
  xAOD::CaloClusterContainer *caloClusterCont=nullptr;
  ctx->retrieve( caloClusterCont );
 

  if( !collection )
  {
    MSG_ERROR("It's not possible to retrieve the calo cell collection.");
    return StatusCode::FAILURE;
  }
 
  if( !caloClusterCont )
  {
    MSG_ERROR("It's not possible to retrieve the calo cluster container");
    return StatusCode::FAILURE;
  }


  std::vector<xAOD::CaloCluster*> vec_roi;
  
  // Get all cells from the second calorimeter layer
  for (const &cell : collection->getCollection(CaloSampling::CaloSample::EM2) )
  {
    // Check only cells with higher than the stablish energy threshold
    if (cell->rawEnergy() > m_energyThreshold ){

      if (vec_roi.empty()){
        // Build the first cluster
        vec_roi.push_back ( new xAOD::CaloCluster( cell->rawEnergy(), cell->eta(), cell->phi(), m_etaWindow/2, m_phiWindow/2 ) );
      }else{
        // Check cluster overlap. Get the higher energy cluster energy in case of overlap
        float eta = cell->eta(); float phi = cell->phi(); float energy =  cell->rawEnergy();
        for( unsigned int i=0; i < vec_roi.size(); ++i )
        {
          if (abs(eta - vec_roi[i].eta())<m_etaWindow/2 && abs( phi - vec_roi[i].phi())<m_phiWindow/2 ){
            if (energy > vec_roi[i].energyCenter())
              // Destroy the old one and avoid memory leak
              delete vec_roi[i];
              // Create the new one
              vec_roi[i] = new xAOD::CaloCluster(eta, phi, m_etaWindow/2, m_phiWindow/2, energy);
          }
        }
      }
    }
  }

  // Attach all cells into your cluster
  for ( auto& cell : collection->getCollection() )
  {
    for (auto & caloCluster : vec_roi){
      // Check if the current cell is in the eta window
      if( (cell->eta() < caloCluster->eta()+m_etaWindow/2) && (cell->eta() > caloCluster->eta()-m_etaWindow/2)
      {
        // Check if the current cell is in the phi window
        if( (cell->phi() < caloCluster->phi()+m_phiWindow/2) && (cell->phi() > caloCluster->phi()-m_phiWindow/2)
        {
          // Add the cell to the cluster
          caloCluster->push_back(cell);
        }
      }
    }
  }// Loop over all cells

 
  for (auto & caloCluster : vec_roi){
    // push the cluster into the container
    caloClusterCont->push_back( caloCluster );
  }


  return StatusCode::SUCCESS;
}





void CaloClusterMaker::fill( EventContext * /*ctx*/ )
{;}



