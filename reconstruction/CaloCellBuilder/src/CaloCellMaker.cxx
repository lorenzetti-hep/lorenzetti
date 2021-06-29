
#include "helper/CaloCellCollection.h"
#include "G4Kernel/CaloPhiRange.h"
#include "CaloCell/CaloDetDescriptor.h"
#include "CaloHit/CaloHitContainer.h"
#include "EventInfo/EventInfoContainer.h"
#include "G4Kernel/constants.h"
#include "CaloCellMaker.h"
#include "TVector3.h"
#include <cstdlib>
#include "G4SystemOfUnits.hh"

#include "TH1F.h"
#include "TH2F.h"
#include "TH2Poly.h"
#include "TGraph.h"

using namespace Gaugi;
using namespace SG;


CaloCellMaker::CaloCellMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "EventKey"                 , m_eventKey="EventInfo"                );
  declareProperty( "HitsKey"                  , m_hitsKey="Hits"                      );
  declareProperty( "HistogramPath"            , m_histPath="/CaloCellMaker"           );
  declareProperty( "CaloCellFile"             , m_caloCellFile                        );
  declareProperty( "CollectionKey"            , m_collectionKey="CaloCellCollection"  );
  declareProperty( "BunchIdStart"             , m_bcid_start=-7                       );
  declareProperty( "BunchIdEnd"               , m_bcid_end=8                          );
  declareProperty( "BunchDuration"            , m_bc_duration=25                      );
  declareProperty( "OutputLevel"              , m_outputLevel=1                       );
  declareProperty( "DetailedHistograms"       , m_detailedHistograms=false            );
}

//!=====================================================================

void CaloCellMaker::push_back( Gaugi::AlgTool* tool )
{
  m_toolHandles.push_back(tool);
}

//!=====================================================================

StatusCode CaloCellMaker::initialize()
{
  CHECK_INIT();
  
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

  for ( auto tool : m_toolHandles )
  {
    if (tool->initialize().isFailure() )
    {
      MSG_FATAL( "It's not possible to iniatialize " << tool->name() << " tool." );
    }
  }

  return StatusCode::SUCCESS;
}

//!=====================================================================

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

//!=====================================================================

StatusCode CaloCellMaker::bookHistograms( SG::EventContext &ctx ) const
{
  auto store = ctx.getStoreGateSvc();

  store->mkdir(m_histPath);
  int nEtabins = m_eta_bins.size() -1;
  int nPhibins = m_phi_bins.size() -1;

  // Create the 2D histogram for monitoring purpose
  store->add(new TH2F( "cells_e", "Estimated Cells Energy; #eta; #phi; Energy [MeV]", nEtabins, m_eta_bins.data(), nPhibins, m_phi_bins.data() ) );
  // Create the 2D histogram for monitoring purpose
  store->add(new TH2F( "cells_edep", "Truth Cells Energy; #eta; #phi; Energy [MeV]", nEtabins, m_eta_bins.data(), nPhibins, m_phi_bins.data() ) );
  // Create the 2D histogram for monitoring purpose
  store->add(new TH1F( "res_cells", "(E_{estimated}-E_{dep}); res_{E} [MeV]; Count",  100, -2*GeV, 2*GeV ) );



  if (m_detailedHistograms){

    int nbunchs = m_bcid_end - m_bcid_start + 1;
    store->add(new TH1F( "timesteps", "Step time per bunch; time[ns]; Count", nbunchs*50, (m_bcid_start - 0.5)*m_bc_duration, (m_bcid_end + 0.5)*m_bc_duration) );
    store->add(new TH1F( "main_event_timesteps", "Step time main event; time[ns]; Count", 50, -0.5*m_bc_duration, m_bc_duration*0.5 ) );
    store->add(new TH2F( "timesteps_per_energy", "Step time per bunch; time [ns]; Energy [MeV];", 
          nbunchs, (m_bcid_start-0.5)*m_bc_duration, (m_bcid_end+0.5)*m_bc_duration, 100, 0, 30) );
  }

  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCellMaker::pre_execute( EventContext &ctx ) const
{
  // Build the CaloCellCollection and attach into the EventContext
  // Create the cell collection into the event context
  SG::WriteHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
  
  collection.record( std::unique_ptr<xAOD::CaloCellCollection>(new xAOD::CaloCellCollection( m_eta_min,m_eta_max,m_eta_bins, m_phi_bins,
                                                                                             m_rmin,m_rmax,
                                                                                             (Detector)m_detector,
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
      int detector, sampling; // Calorimeter layer and eta/phi ids
      unsigned long int hash;
      //std::string hash;
      ss >> detector >> sampling >> eta >> phi >> deta >> dphi >> rmin >> rmax >> zmin >> zmax >> hash;

      // Create the calorimeter cell
      auto *descriptor = new xAOD::CaloDetDescriptor( eta, phi, deta, dphi, rmin, rmax, hash, 
                                                      (CaloSampling)sampling,
                                                      (Detector)detector,
                                                      m_bc_duration, m_bcid_start, m_bcid_end );
      
      // Add the CaloCell into the collection
      collection->push_back( descriptor );
    } 
  }
  file.close();
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCellMaker::execute( EventContext &/*ctx*/ , const G4Step */*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCellMaker::execute( EventContext &ctx , int /*evt*/ ) const
{
  
  if( pre_execute(ctx).isFailure())
  {
    MSG_FATAL("Its not possible to pre_execute. Abort!");
  }

  if( post_execute(ctx).isFailure())
  {
    MSG_FATAL("Its not possible to post_execute. Abort!");
  }

  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCellMaker::post_execute( EventContext &ctx ) const
{

  SG::ReadHandle<xAOD::CaloHitContainer> hits( m_hitsKey, ctx );
 
  if( !hits.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloHitContainer using this key: " << m_hitsKey);
  }

  // Event info
  SG::ReadHandle<xAOD::EventInfoContainer> event(m_eventKey, ctx);
  
  if( !event.isValid() ){
    MSG_FATAL( "It's not possible to read the xAOD::EventInfoContainer from this Context" );
  }

  SG::ReadHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellCollection using this key: " << m_collectionKey);
  }

  auto evt = (**event.ptr()).front();

  for ( const auto& hit : **hits.ptr() )
  {
    xAOD::CaloDetDescriptor *descriptor=nullptr;

    // Check if the current hit allow to this cells collection
    collection->retrieve( hit, descriptor );
    if(descriptor){

      if (descriptor->hash() != hit->hash()){
        MSG_FATAL( "Descriptor hash code is different than hit hash code. Abort!");
      }

   

      for ( int bcid = hit->bcid_start();  bcid <= hit->bcid_end(); ++bcid )
      {
        // transfer truth energy for each bunch crossing to descriptor
        descriptor->edep( bcid, hit->edep(bcid) ); 
      }  

      for ( auto tool : m_toolHandles )
      {
        // digitalization
        if( tool->execute( evt, descriptor ).isFailure() ){
          MSG_ERROR( "It's not possible to execute the tool with name " << tool->name() );
          return StatusCode::FAILURE;
        }
      }
    }
    
  }// loop over all hits
 

  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCellMaker::fillHistograms( EventContext &ctx ) const
{
  

  auto store = ctx.getStoreGateSvc();
  SG::ReadHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellCollection using this key: " << m_collectionKey);
  }

  store->cd(m_histPath);

  const xAOD::CaloDetDescriptor *hotCell = nullptr;

  for ( const auto& p : **collection.ptr() ){ 

    const auto *cell = p.second;

    if(!hotCell)  hotCell=cell;
    if(cell->edep() > hotCell->edep())  hotCell=cell;


    store->hist1("res_cells")->Fill( cell->e()-cell->edep() );

    {// Fill estimated energy 2D histograms
      int x = store->hist2("cells_e")->GetXaxis()->FindBin(cell->eta());
      int y = store->hist2("cells_e")->GetYaxis()->FindBin(cell->phi());
      int bin = store->hist2("cells_e")->GetBin(x,y,0);
      float energy = store->hist2("cells_e")->GetBinContent( bin );
      store->hist2("cells_e")->SetBinContent( bin, (energy + cell->e()) );
    }
    
    {// Fill truth energy 2D histograms
      int x = store->hist2("cells_edep")->GetXaxis()->FindBin(cell->eta());
      int y = store->hist2("cells_edep")->GetYaxis()->FindBin(cell->phi());
      int bin = store->hist2("cells_edep")->GetBin(x,y,0);
      float energy = store->hist2("cells_edep")->GetBinContent( bin );
      store->hist2("cells_edep")->SetBinContent( bin, (energy + cell->edep()) );
    }

  }

  if(hotCell && m_detailedHistograms){
      auto pulse = hotCell->pulse();
      std::vector<float> samples;
      for (int samp=0;samp<(int)pulse.size();++samp)
        samples.push_back(samp);
      TGraph *graph = new TGraph(pulse.size(), samples.data(), pulse.data());
      graph->SetName("test");
      //raph->Fit("pol5");
      store->add(graph);
      graph->Write();
  }
  return StatusCode::SUCCESS;
}



