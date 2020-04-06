
#include "CaloCellMaker.h"
#include "TVector3.h"
#include <cstdlib>

using namespace Gaugi;
using namespace SG;
using namespace CaloSampling;


CaloCellMaker::CaloCellMaker( std::string name , int msglevel ) : 
  Algorithm( name , (MSG::Level)msglevel )
{

  declareProperty( "HistPath"         , m_histPath      , "/CaloCellMaker"            );
  declareProperty( "CellConfig"       , m_cellConfig    , ""                          );
  declareProperty( "BunchConfig"      , m_bunchConfig   , ""                          );
  declareProperty( "CollectionKey"    , m_collectionKey , "xAOD__CaloCellCollection"  );
}



CaloCellMaker::~CaloCellMaker()
{;}





StatusCode CaloCellMaker::initialize()
{
  // This is a default path
  std::ifstream bc_file( m_bunchConfig );
  bc_file >> m_bcid_start >> m_bcid_end >> m_bc_duration >> m_bc_nsamples;
  bc_file.close();
  
  auto store = getStoreGateSvc();
  store->mkdir( m_histPath );


  // Read the file
  std::ifstream file(m_cellConfig);

	std::string line;
	while (std::getline(file, line))
	{
    std::string command;
    // Get the command
    file >> command;
    // Layer configuration
    if (command=="L"){
      file >> m_sampling >> m_eta_min >> m_eta_max >> m_eta_bins >> m_phi_min >> m_phi_max >> m_phi_bins >> m_rmin >> m_rmax;
      break;
    }
	}
  file.close();

  std::stringstream ss; ss << "cell_etaVsPhi_sampling_" << m_sampling;
  // Create the 2D histogram for monitoring purpose
  store->add(new TH2F( ss.str().c_str(), "Cell Energy; #eta; #phi; Energy [GeV]", m_eta_bins, m_eta_min, m_eta_max, 
                       m_phi_bins, m_phi_min, m_phi_max) );
  
  for ( auto tool : m_toolHandles )
  {
    // StoreGate link
    tool->setStoreGateSvc( store );
    if (tool->initialize().isFailure() )
    {
      MSG_FATAL( "It's not possible to iniatialize " << tool->name() << " tool." );
    }

  }

  return StatusCode::SUCCESS;
}



StatusCode CaloCellMaker::finalize()
{
  for ( auto tool : m_toolHandles )
  {
    if (tool->finalize().isFailure() )
    {
      MSG_ERROR( "It's not possible to iniatialize " << tool->name() << " tool." );
    }
  }
  return StatusCode::SUCCESS;
}





StatusCode CaloCellMaker::pre_execute( EventContext &ctx ) const
{
  // Build the CaloCellCollection and attach into the EventContext
  // Create the cell collection into the event context
  SG::WriteHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
  collection.record( std::unique_ptr<xAOD::CaloCellCollection>(new xAOD::CaloCellCollection(m_eta_min,
                                                                                            m_eta_max,
                                                                                            m_eta_bins,
                                                                                            m_phi_min,
                                                                                            m_phi_max,
                                                                                            m_phi_bins,
                                                                                            m_rmin,
                                                                                            m_rmax,
                                                                                            (CaloSample)m_sampling)
                                                              ) );

   
  // Read the file
  std::ifstream file( m_cellConfig );

	std::string line;
	while (std::getline(file, line))
	{
    std::string command;
    // Get the command
    file >> command;
    // Get only cell config 
    if (command=="C"){
      float  eta, phi, deta, dphi, rmin, rmax;
      int sampling; // Calorimeter layer
      std::string hash;
      file >> sampling >> eta >> phi >> deta >> dphi >> rmin >> rmax >> hash;

      // Create the calorimeter cell
      auto *cell = new xAOD::CaloCell( eta, 
                                       phi, 
                                       deta , 
                                       dphi,  
                                       rmin, 
                                       rmax, 
                                       hash,
                                       (CaloSample)sampling,
                                       m_bc_duration,
                                       m_bc_nsamples,
                                       m_bcid_start,
                                       m_bcid_end,
                                       m_bcid_truth);
      
      // Add the CaloCell into the collection
      collection->push_back( cell );
    } 
	}
  file.close();
  return StatusCode::SUCCESS;
}



  
StatusCode CaloCellMaker::execute( EventContext &ctx , const G4Step *step ) const
{
   
  SG::ReadHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );

  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellCollection using this key: " << m_collectionKey);
  }

  // Get the position
  G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
  // Apply all necessary transformation (x,y,z) to (eta,phi,r) coordinates
  // Get ATLAS coordinates (in transverse plane xy)
  auto vpos = TVector3( pos.x(), pos.y(), pos.z());

  // This object can not be const since we will change the intenal value
  xAOD::CaloCell *cell=nullptr;
  collection->retrieve( vpos, cell );
  
  if(cell)  
    cell->Fill( step );
  
  return StatusCode::SUCCESS;
}




StatusCode CaloCellMaker::post_execute( EventContext &ctx ) const
{
  
  SG::ReadHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellCollection using this key: " << m_collectionKey);
  }


  for ( const auto& p : **collection.ptr() )
  {
    for ( auto tool : m_toolHandles )
    {
      if( tool->executeTool( p.second ).isFailure() ){
        MSG_ERROR( "It's not possible to execute the tool with name " << tool->name() );
        return StatusCode::FAILURE;
      }
    }
  }

  return StatusCode::SUCCESS;
}




StatusCode CaloCellMaker::fillHistograms( EventContext &ctx ) const
{
  
  SG::ReadHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellCollection using this key: " << m_collectionKey);
  }

  auto store = getStoreGateSvc();

  /* 
  for ( const auto& p : *collection ){ 
    const auto *cell = p.second;
    // Skip cells with energy equal zero
    std::stringstream ss; ss << "cells/layer_" << (int)cell->sampling();
    int x = store->hist2(ss.str())->GetXaxis()->FindBin(cell->eta());
    int y = store->hist2(ss.str())->GetYaxis()->FindBin(cell->phi());
    int bin = store->hist2(ss.str())->GetBin(x,y,0);
    float energy = store->hist2(ss.str())->GetBinContent( bin );
    store->hist2(ss.str())->SetBinContent( bin, (energy + cell->energy()) );
  }*/

  return StatusCode::SUCCESS;
}



