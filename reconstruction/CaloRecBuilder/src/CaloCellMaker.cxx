#include "CaloCellCollection.h"
#include "CaloCluster/CaloClusterContainer.h"
#include "CaloCell/RawCell.h"
#include "EventInfo/EventInfoContainer.h"
#include "G4Kernel/constants.h"
#include "CaloCellMaker.h"
#include "TVector3.h"
#include <cstdlib>

using namespace Gaugi;
using namespace SG;
using namespace CaloSampling;




CaloCellMaker::CaloCellMaker( std::string name ) : 
  IMsgService(name),
  Algorithm(),
  m_bcid_truth( special_bcid_for_truth_reconstruction )
{

  declareProperty( "EventKey"         , m_eventKey="EventInfo"                );
  declareProperty( "HistogramPath"    , m_histPath="/CaloCellMaker"           );
  declareProperty( "CaloCellFile"     , m_caloCellFile                        );
  declareProperty( "CollectionKey"    , m_collectionKey="CaloCellCollection"  );
  declareProperty( "BunchIdStart"     , m_bcid_start=-7                       );
  declareProperty( "BunchIdEnd"       , m_bcid_end=8                          );
  declareProperty( "BunchDuration"    , m_bc_duration=25                      );
  declareProperty( "NumberOfSamplesPerBunch" , m_bc_nsamples=1                );
  declareProperty( "OutputLevel"      , m_outputLevel=1                       );

}


void CaloCellMaker::push_back( CaloTool* tool )
{
  m_toolHandles.push_back(tool);
}


StatusCode CaloCellMaker::initialize()
{
  // Set message level
  setMsgLevel( (MSG::Level)m_outputLevel );
  
  // Read the file
  std::ifstream file(m_caloCellFile);

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

  for ( auto tool : m_toolHandles )
  {
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
      MSG_ERROR( "It's not possible to finalize " << tool->name() << " tool." );
    }
  }
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMaker::bookHistograms( StoreGate &store ) const
{

  store.mkdir(m_histPath);
  store.add( new TH1F("res_layer" ,"(#E_{Estimated}-#E_{Truth})/#E_{Truth};res_{E};Count",100,-40,40) );


  store.mkdir(m_histPath+"/reco");
  // Create the 2D histogram for monitoring purpose
  store.add(new TH2F( "cells_layer", "Estimated Cells Energy; #eta; #phi; Energy [MeV]", m_eta_bins, m_eta_min, m_eta_max, 
                       m_phi_bins, m_phi_min, m_phi_max) );

  store.mkdir(m_histPath+"/truth");
  // Create the 2D histogram for monitoring purpose
  store.add(new TH2F( "cells_layer", "Truth Cells Energy; #eta; #phi; Energy [MeV]", m_eta_bins, m_eta_min, m_eta_max, 
                         m_phi_bins, m_phi_min, m_phi_max) );

  return StatusCode::SUCCESS;
}


StatusCode CaloCellMaker::pre_execute( EventContext &ctx ) const
{
  // Build the CaloCellCollection and attach into the EventContext
  // Create the cell collection into the event context
  SG::WriteHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
  
  collection.record( std::unique_ptr<xAOD::CaloCellCollection>(new xAOD::CaloCellCollection(m_eta_min,m_eta_max,m_eta_bins,m_phi_min,
                                                                                            m_phi_max,m_phi_bins,m_rmin,m_rmax,
                                                                                            (CaloSample)m_sampling)));
  // Read the file
  std::ifstream file( m_caloCellFile );

	std::string line;
	while (std::getline(file, line))
	{
    std::string command;
    // Get the command
    file >> command;
    // Get only cell config 
    if (command=="C"){
      float  eta, phi, deta, dphi, rmin, rmax;
      int sampling, channel_eta, channel_phi; // Calorimeter layer and eta/phi ids
      std::string hash;
      file >> sampling >> eta >> phi >> deta >> dphi >> rmin >> rmax >> hash >> channel_eta >> channel_phi;

      // Create the calorimeter cell
      auto *cell = new xAOD::RawCell( eta, phi, deta, dphi, rmin, rmax, hash, channel_eta, channel_phi, (CaloSample)sampling,
                                      m_bc_duration, m_bc_nsamples, m_bcid_start, m_bcid_end, m_bcid_truth);
      
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
  xAOD::RawCell *cell=nullptr;
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

  
  // Event info
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);
  
  if( !event.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
  }


  auto evt = (**event.ptr()).front();

  for ( const auto& p : **collection.ptr() )
  {
    for ( auto tool : m_toolHandles )
    {
      if( tool->executeTool( evt, p.second ).isFailure() ){
        MSG_ERROR( "It's not possible to execute the tool with name " << tool->name() );
        return StatusCode::FAILURE;
      }
    }
  }
  
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMaker::fillHistograms( EventContext &ctx , StoreGate &store) const
{
  
  SG::ReadHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellCollection using this key: " << m_collectionKey);
  }



  for ( const auto& p : **collection.ptr() ){ 
    const auto *cell = p.second;

    {
      std::stringstream ss; ss << "res_layer_" << m_sampling;
      store.cd(m_histPath);
      store.hist1(ss.str())->Fill( (cell->energy()-cell->truthRawEnergy())/cell->truthRawEnergy() );
    }


    {// Fill estimated energy 2D histograms
      store.cd(m_histPath+"/reco");
      std::stringstream ss; ss << "cells_layer_" << (int)cell->sampling();
      int x = store.hist2(ss.str())->GetXaxis()->FindBin(cell->eta());
      int y = store.hist2(ss.str())->GetYaxis()->FindBin(cell->phi());
      int bin = store.hist2(ss.str())->GetBin(x,y,0);
      float energy = store.hist2(ss.str())->GetBinContent( bin );
      store.hist2(ss.str())->SetBinContent( bin, (energy + cell->energy()) );
    }
    
    {// Fill truth energy 2D histograms
      store.cd(m_histPath+"/truth");
      std::stringstream ss; ss << "cells_layer_" << (int)cell->sampling();
      int x = store.hist2(ss.str())->GetXaxis()->FindBin(cell->eta());
      int y = store.hist2(ss.str())->GetYaxis()->FindBin(cell->phi());
      int bin = store.hist2(ss.str())->GetBin(x,y,0);
      float energy = store.hist2(ss.str())->GetBinContent( bin );
      store.hist2(ss.str())->SetBinContent( bin, (energy + cell->truthRawEnergy()) );
    }


    {
      store.cd(m_histPath+"/truth");
       

    } 

  }

  return StatusCode::SUCCESS;
}



