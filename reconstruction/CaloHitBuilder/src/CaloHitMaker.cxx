
#include "helper/CaloHitCollection.h"
#include "G4Kernel/CaloPhiRange.h"
#include "CaloHit/CaloHit.h"
#include "EventInfo/EventInfoContainer.h"
#include "G4Kernel/constants.h"
#include "CaloHitMaker.h"
#include "TVector3.h"
#include <cstdlib>
#include "G4SystemOfUnits.hh"

#include "TH1F.h"
#include "TH2F.h"
#include "TH2Poly.h"
#include "TGraph.h"

using namespace Gaugi;
using namespace SG;




CaloHitMaker::CaloHitMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "EventKey"                 , m_eventKey="EventInfo"                );
  declareProperty( "HistogramPath"            , m_histPath="/CaloHitMaker"            );
  declareProperty( "CaloHitFile"              , m_caloHitFile                         );
  declareProperty( "CollectionKey"            , m_collectionKey="CaloHitCollection"   );
  declareProperty( "BunchIdStart"             , m_bcid_start=-7                       );
  declareProperty( "BunchIdEnd"               , m_bcid_end=8                          );
  declareProperty( "BunchDuration"            , m_bc_duration=25                      );
  declareProperty( "OutputLevel"              , m_outputLevel=1                       );
  declareProperty( "DetailedHistograms"       , m_detailedHistograms=false            );
}

//!=====================================================================

StatusCode CaloHitMaker::initialize()
{
  CHECK_INIT();

  // Set message level
  setMsgLevel( (MSG::Level)m_outputLevel );
  
  // Read the file
  std::ifstream file(m_caloHitFile);

  std::string line;
  while (std::getline(file, line))
  {
    std::stringstream ss(line);
    std::string command;
    // Get the command
    ss >> command;
    // Layer configuration
    if (command=="config"){
      ss >> m_detector >> m_sampling >> m_segmentation >> m_eta_min >> m_eta_max >> m_rmin >> m_rmax;
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

  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();

  store->mkdir(m_histPath);
  int nEtabins = m_eta_bins.size() -1;
  int nPhibins = m_phi_bins.size() -1;

 
  // Create the 2D histogram for monitoring purpose
  store->add(new TH2F( "hits_edep", "Hit Energy (Truth); #eta; #phi; Energy [MeV]", nEtabins, m_eta_bins.data(), nPhibins, m_phi_bins.data() ) );
  

  if (m_detailedHistograms){

    int nbunchs = m_bcid_end - m_bcid_start + 1;
    //store->add(new TH2F( "edep_per_bunch", "", nbunchs, m_bcid_start, m_bcid_end+1, 100, 0, 3.5) );
    store->add(new TH1F( "timesteps", "Step time per bunch; time[ns]; Count", nbunchs*50, (m_bcid_start - 0.5)*m_bc_duration, (m_bcid_end + 0.5)*m_bc_duration) );

    store->add(new TH1F( "main_event_timesteps", "Step time main event; time[ns]; Count", 50, -0.5*m_bc_duration, m_bc_duration*0.5 ) );

    store->add(new TH2F( "timesteps_per_energy", "Step time per bunch; time [ns]; Energy [MeV];", 
          nbunchs, (m_bcid_start-0.5)*m_bc_duration, (m_bcid_end+0.5)*m_bc_duration, 100, 0, 30) );     

  }

  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMaker::pre_execute( EventContext &ctx ) const
{
  // Build the CaloHitCollection and attach into the EventContext
  // Create the cell collection into the event context
  SG::WriteHandle<xAOD::CaloHitCollection> collection( m_collectionKey, ctx );
  
  collection.record( std::unique_ptr<xAOD::CaloHitCollection>(new xAOD::CaloHitCollection( m_eta_min,m_eta_max,m_eta_bins, m_phi_bins,
                                                                                             m_rmin,m_rmax,
                                                                                             (Detector)m_detector,
                                                                                             (CaloSampling)m_sampling, 
                                                                                             m_segmentation)));
  // Read the file:
  std::ifstream file( m_caloHitFile );

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
      int detector, sampling; // Calorimeter layer and eta/phi ids
      unsigned long int hash;
      //std::string hash;
      ss >> detector >> sampling >> eta >> phi >> deta >> dphi >> rmin >> rmax >> zmin >> zmax >> hash;

      // Create the calorimeter cell
      auto *hit = new xAOD::CaloHit( eta, phi, deta, dphi, hash, 
                                     (CaloSampling)sampling,
                                     (Detector)detector,
                                     m_bc_duration, m_bcid_start, m_bcid_end );
      
      // Add the CaloHit into the collection
      collection->push_back( hit );
    } 
  }
  file.close();
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMaker::execute( EventContext &ctx , const G4Step *step ) const
{

  SG::ReadHandle<xAOD::CaloHitCollection> collection( m_collectionKey, ctx );

  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloHitCollection using this key: " << m_collectionKey);
  }

  // Get the position
  G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();
  // Apply all necessary transformation (x,y,z) to (eta,phi,r) coordinates
  // Get ATLAS coordinates (in transverse plane xy)
  auto vpos = TVector3( pos.x(), pos.y(), pos.z());

  // This object can not be const since we will change the intenal value
  xAOD::CaloHit *hit=nullptr;
  collection->retrieve( vpos, hit );

  if(hit) hit->Fill( step );

  if (m_detailedHistograms ){
    // Fill time steps
    G4StepPoint* point = step->GetPreStepPoint();
    float t = (float)point->GetGlobalTime() / ns;
    auto store = ctx.getStoreGateSvc();
    store->cd(m_histPath);
    store->hist1("main_event_timesteps")->Fill(t);
    store->hist1("timesteps")->Fill(t);
    float edep = (float)step->GetTotalEnergyDeposit();
    store->hist1("timesteps_per_energy")->Fill(t,edep/MeV);
  }

  return StatusCode::SUCCESS;
}

//!=====================================================================

// standlone execute
StatusCode CaloHitMaker::execute( EventContext &ctx, int /*evt*/ ) const
{
  return pre_execute(ctx);
}

//!=====================================================================

StatusCode CaloHitMaker::post_execute( EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMaker::fillHistograms( EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();
  SG::ReadHandle<xAOD::CaloHitCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloHitCollection using this key: " << m_collectionKey);
  }

  store->cd(m_histPath);

  for ( const auto& p : **collection.ptr() ){ 

    const auto *hit = p.second;
    
    {// Fill truth energy 2D histograms
      int x = store->hist2("hits_edep")->GetXaxis()->FindBin(hit->eta());
      int y = store->hist2("hits_edep")->GetYaxis()->FindBin(hit->phi());
      int bin = store->hist2("hits_edep")->GetBin(x,y,0);
      float energy = store->hist2("hits_edep")->GetBinContent( bin );
      store->hist2("hits_edep")->SetBinContent( bin, (energy + hit->edep()) );
    }

  }

  return StatusCode::SUCCESS;
}



