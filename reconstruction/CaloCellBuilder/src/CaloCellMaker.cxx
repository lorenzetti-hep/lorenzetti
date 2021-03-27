
#include "CaloCell/helper/CaloCellCollection.h"
#include "G4Kernel/CaloPhiRange.h"
#include "CaloCell/CaloDetDescriptor.h"
#include "EventInfo/EventInfoContainer.h"
#include "G4Kernel/constants.h"
#include "CaloCellMaker.h"
#include "TVector3.h"
#include <cstdlib>
#include "G4SystemOfUnits.hh"

#include "TH1F.h"
#include "TH2F.h"
#include "TH2Poly.h"

using namespace Gaugi;
using namespace SG;




CaloCellMaker::CaloCellMaker( std::string name ) : 
  IMsgService(name),
  Algorithm(),
  m_bcid_truth( special_bcid_for_truth_reconstruction )
{
  declareProperty( "Detector"                 , m_detector=0                          );
  declareProperty( "EventKey"                 , m_eventKey="EventInfo"                );
  declareProperty( "HistogramPath"            , m_histPath="/CaloCellMaker"           );
  declareProperty( "CaloCellFile"             , m_caloCellFile                        );
  declareProperty( "CollectionKey"            , m_collectionKey="CaloCellCollection"  );
  declareProperty( "BunchIdStart"             , m_bcid_start=-7                       );
  declareProperty( "BunchIdEnd"               , m_bcid_end=8                          );
  declareProperty( "BunchDuration"            , m_bc_duration=25                      );
  declareProperty( "NumberOfSamplesPerBunch"  , m_bc_nsamples=1                       );
  declareProperty( "OutputLevel"              , m_outputLevel=1                       );
  declareProperty( "DetailedHistograms"       , m_detailedHistograms=false            );
  declareProperty( "OnlyRoI"                  , m_onlyRoI=false                       );
}


void CaloCellMaker::push_back( Gaugi::AlgTool* tool )
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
    std::stringstream ss(line);
    std::string command;
    // Get the command
    ss >> command;
    // Layer configuration
    if (command=="config"){
      ss >> m_sampling >> m_segmentation >> m_eta_min >> m_eta_max >> m_rmin >> m_rmax;
    }else if(command=="eta_bins"){
      float value;
      while(ss>>value)
        m_eta_bins.push_back(value);
    }else if(command=="phi_bins"){
      float value;
      while(ss>>value)
        m_phi_bins.push_back(value);
    }else if(command=="#"){
      continue;
    }else{
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


StatusCode CaloCellMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();

  store->mkdir(m_histPath);

  store->add( new TH1F("res_layer" ,"(#E_{Estimated}-#E_{Truth})/#E_{Truth};res_{E};Count",20,-1,1) );

  int nEtabins = m_eta_bins.size() -1;
  int nPhibins = m_phi_bins.size() -1;

 

  // Create the 2D histogram for monitoring purpose
  store->add(new TH2F( "cells", "Estimated Cells Energy; #eta; #phi; Energy [MeV]", nEtabins, m_eta_bins.data(), nPhibins, m_phi_bins.data() ) );
  // Create the 2D histogram for monitoring purpose
  store->add(new TH2F( "truth_cells", "Truth Cells Energy; #eta; #phi; Energy [MeV]", nEtabins, m_eta_bins.data(), nPhibins, m_phi_bins.data() ) );

  if (m_detailedHistograms){
    int nbunchs = m_bcid_end - m_bcid_start + 1;
    store->add(new TH2F( "energy_samples_per_bunch", "", nbunchs, m_bcid_start, m_bcid_end+1, 100, 0, 3.5) );
    store->add(new TH1F( "timesteps", "Step time per bunch; time[ns]; Count", nbunchs*20, (m_bcid_start-0.5)*m_bc_duration, (m_bcid_end+0.5)*m_bc_duration) );
    store->add(new TH2F( "timestepsVsEnergy", "Step time per bunch; time [ns]; Energy [MeV];", 
          nbunchs*20, (m_bcid_start-0.5)*m_bc_duration, (m_bcid_end+0.5)*m_bc_duration, 100, 0, 30) );
  }

  return StatusCode::SUCCESS;
}


StatusCode CaloCellMaker::pre_execute( EventContext &ctx ) const
{
  // Build the CaloCellCollection and attach into the EventContext
  // Create the cell collection into the event context
  SG::WriteHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
  
  collection.record( std::unique_ptr<xAOD::CaloCellCollection>(new xAOD::CaloCellCollection( m_eta_min,m_eta_max,m_eta_bins, m_phi_bins,
                                                                                             m_rmin,m_rmax,
                                                                                             (CaloSampling)m_sampling, 
                                                                                             m_segmentation)));
  // Read the file:
  std::ifstream file( m_caloCellFile );

  std::string line;
  while (std::getline(file, line))
  {
    std::stringstream ss(line);
    std::string command;
    // Get the command
    ss >> command;
    // Get only cell config 
    if (command=="cell"){
      float  eta, phi, deta, dphi, rmin, rmax, zmin, zmax;
      int sampling; // Calorimeter layer and eta/phi ids
      unsigned int hash;
      //std::string hash;
      ss >> sampling >> eta >> phi >> deta >> dphi >> rmin >> rmax >> zmin >> zmax >> hash;

      // Create the calorimeter cell
      auto *descriptor = new xAOD::CaloDetDescriptor( eta, phi, deta, dphi, rmin, rmax, hash, (CaloSampling)sampling,
                                                (Detector)m_detector,
                                                m_bc_duration, m_bc_nsamples, m_bcid_start, m_bcid_end, m_bcid_truth);
      
      // Add the CaloCell into the collection
      collection->push_back( descriptor );
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

  /*
  if(m_onlyRoI){
    // Event info
    SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);
    
    if( !event.isValid() ){
      MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
    }

    auto evt = (**event.ptr()).front();
    for ( auto& seed : evt->allSeeds() )
    {
      float deltaEta = std::abs( seed.eta - vpos.PseudoRapidity() );
      float deltaPhi = std::abs( CaloPhiRange::diff( seed.phi , vpos.Phi() ));
      if ( !( deltaEta < 0.3 && deltaPhi < 0.3) ){
        return StatusCode::SUCCESS;
      }
    }
  }
  */

  // This object can not be const since we will change the intenal value
  xAOD::CaloDetDescriptor *cell=nullptr;
  collection->retrieve( vpos, cell );

  if(cell) cell->Fill( step );

  if (m_detailedHistograms ){
    // Fill time steps
    G4StepPoint* point = step->GetPreStepPoint();
    float t = (float)point->GetGlobalTime() / ns;
    auto store = ctx.getStoreGateSvc();
    store->cd(m_histPath);
    store->hist1("timesteps")->Fill(t);
    float edep = (float)step->GetTotalEnergyDeposit();
    store->hist1("timestepsVsEnergy")->Fill(t,edep/MeV);
  }

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
      // hit digitalization
      if( tool->execute( evt, p.second ).isFailure() ){
        MSG_ERROR( "It's not possible to execute the tool with name " << tool->name() );
        return StatusCode::FAILURE;
      }
    }
  }
 

  return StatusCode::SUCCESS;
}


StatusCode CaloCellMaker::fillHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  SG::ReadHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellCollection using this key: " << m_collectionKey);
  }



  for ( const auto& p : **collection.ptr() ){ 
    const auto *cell = p.second;

    store->cd(m_histPath);
    if( cell->truthEnergy() > 0 && cell->energy() > 1*GeV )
      store->hist1("res_layer")->Fill( (cell->energy()-cell->truthEnergy())/cell->truthEnergy() );


    {// Fill estimated energy 2D histograms
      int x = store->hist2("cells")->GetXaxis()->FindBin(cell->eta());
      int y = store->hist2("cells")->GetYaxis()->FindBin(cell->phi());
      int bin = store->hist2("cells")->GetBin(x,y,0);
      float energy = store->hist2("cells")->GetBinContent( bin );
      store->hist2("cells")->SetBinContent( bin, (energy + cell->energy()) );
  
      if(m_detailedHistograms){
        int i=0;
        auto samples = cell->energySamples();
        for ( int bc=m_bcid_start; bc<m_bcid_end+1; ++bc)
        {
          store->hist2("energy_samples_per_bunch")->Fill(bc,samples[i]/1000.);
          ++i;
        }
      }

    }
    
    {// Fill truth energy 2D histograms
      int x = store->hist2("truth_cells")->GetXaxis()->FindBin(cell->eta());
      int y = store->hist2("truth_cells")->GetYaxis()->FindBin(cell->phi());
      int bin = store->hist2("truth_cells")->GetBin(x,y,0);
      float energy = store->hist2("truth_cells")->GetBinContent( bin );
      store->hist2("truth_cells")->SetBinContent( bin, (energy + cell->truthEnergy()) );
    }


  }
  return StatusCode::SUCCESS;
}



