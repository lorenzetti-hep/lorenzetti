
#include "G4Kernel/CaloPhiRange.h"
#include "CaloCell/CaloDetDescriptor.h"
#include "CaloCell/CaloDetDescriptorCollection.h"
#include "CaloHit/CaloHitContainer.h"
#include "CaloHit/CaloHitCollection.h"
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
  declareProperty( "InputHitsKey"             , m_hitsKey="Hits"                      ); // input
  declareProperty( "OutputCollectionKey"      , m_collectionKey="Cells"               ); // output

  declareProperty( "EtaBins"                  , m_etaBins                             );
  declareProperty( "PhiBins"                  , m_phiBins                             );
  declareProperty( "ZMin"                     , m_zMin                                );
  declareProperty( "ZMax"                     , m_zMax                                );
  declareProperty( "Sampling"                 , m_sampling                            );
  declareProperty( "Segment"                  , m_segment                             );
  declareProperty( "Detector"                 , m_detector                            );
  declareProperty( "BunchIdStart"             , m_bcid_start=-7                       );
  declareProperty( "BunchIdEnd"               , m_bcid_end=8                          );
  declareProperty( "BunchDuration"            , m_bc_duration=25                      );
  declareProperty( "OutputLevel"              , m_outputLevel=1                       );
  declareProperty( "DetailedHistograms"       , m_detailedHistograms=false            );
  declareProperty( "HistogramPath"            , m_histPath="/CaloCellMaker"           );


}

//!=====================================================================

void CaloCellMaker::push_back( Gaugi::AlgTool* tool )
{
  m_toolHandles.push_back(tool);
}

void CaloCellMaker::setPulseGenerator(Gaugi::AlgTool* tool)
{
  m_pulseGenerator=tool;
}


//!=====================================================================

StatusCode CaloCellMaker::initialize()
{
  CHECK_INIT();
  if (m_pulseGenerator->initialize().isFailure()) {
      MSG_FATAL( "It's not possible to iniatialize Pulse Generator tool." );
  }
  m_nEtaBins = m_etaBins.size() - 1;
  m_nPhiBins = m_phiBins.size() - 1;

  // Set message level
  setMsgLevel( (MSG::Level)m_outputLevel );
 
  for ( auto tool : m_toolHandles )
  {
    MSG_DEBUG(tool->name());
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
  // Create the 2D histogram for monitoring purpose
  store->add(new TH2F( "cells_e", "Estimated Cells Energy; #eta; #phi; Energy [MeV]", m_nEtaBins, m_etaBins.data(), m_nPhiBins, m_phiBins.data() ) );
  // Create the 2D histogram for monitoring purpose
  store->add(new TH2F( "cells_edep", "Truth Cells Energy; #eta; #phi; Energy [MeV]", m_nEtaBins, m_etaBins.data(), m_nPhiBins, m_phiBins.data() ) );
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
  // Build the CaloHitCollection and attach into the EventContext
  // Create the hit collection into the event context
  SG::WriteHandle<xAOD::CaloDetDescriptorCollection> collection( m_collectionKey, ctx );

  collection.record( std::unique_ptr<xAOD::CaloDetDescriptorCollection>(new xAOD::CaloDetDescriptorCollection()) );

  float deltaEta = std::abs(m_etaBins[1] - m_etaBins[0]); //[-3,-2], [2,3]
  float deltaPhi = std::abs(m_phiBins[1] - m_phiBins[0]);

  //
  // Prepare all sensitive objects like a two dimensional histogram
  //
  for ( unsigned etaBin = 0; etaBin < m_nEtaBins; ++etaBin){

    //if (std::abs(m_etaBins[etaBin]) == std::abs(m_etaBins[etaBin+1]))
    //  continue;

    for ( unsigned phiBin = 0; phiBin < m_nPhiBins; ++phiBin){

      float etaCenter = m_etaBins[etaBin] + deltaEta / 2;
      float phiCenter = m_phiBins[phiBin] + deltaPhi / 2;

      unsigned bin = m_nPhiBins * etaBin + phiBin;

      // Create the calorimeter cell
      auto *descriptor = new xAOD::CaloDetDescriptor( etaCenter, phiCenter, deltaEta, deltaPhi, 
                                                      hash(bin), 
                                                      (CaloSampling)m_sampling,
                                                      (Detector)m_detector,
                                                      m_bc_duration, m_bcid_start, m_bcid_end );

      if ( !collection->insert( descriptor->hash(), descriptor ) ){
        MSG_FATAL( "It is not possible to include cell hash ("<< descriptor->hash() << ") into the collection. hash already exist.");
      }

    } // Loop over phi bins
  }// Loop over eta bins

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

  SG::ReadHandle<xAOD::CaloDetDescriptorCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloDetDescriptorCollection using this key: " << m_collectionKey);
  }
  

  //
  // Digitalization
  //

  //
  // (step 1) Generate the pulse for all cells
  //
  for ( const auto& hit : **hits.ptr() )
  {
    xAOD::CaloDetDescriptor *descriptor=nullptr;

    // Check if the current hit allow to this cells collection
    if(collection->retrieve( hit->hash(), descriptor ))
    {
      if (descriptor->hash() != hit->hash()){
        MSG_FATAL( "Descriptor hash code is different than hit hash code. Abort!");
      }

      for ( int bcid = hit->bcid_start();  bcid <= hit->bcid_end(); ++bcid )
      {
        // transfer truth energy for each bunch crossing to descriptor
        descriptor->edep( bcid, hit->edep(bcid) ); 
        descriptor->tof ( bcid, hit->tof(bcid)  );
      }  

      if( m_pulseGenerator->execute(ctx, descriptor).isFailure() ){
          MSG_ERROR( "It's not possible to execute Pulse generator." );
          return StatusCode::FAILURE;
      }
    }

  }// loop over all hits
 

  for ( const auto& hit : **hits.ptr() )
  {
    xAOD::CaloDetDescriptor *descriptor=nullptr;

    // Check if the current hit allow to this cells collection
    if(collection->retrieve( hit->hash(), descriptor ))
    {
      if (descriptor->hash() != hit->hash()){
        MSG_FATAL( "Descriptor hash code is different than hit hash code. Abort!");
      }

      for ( auto tool : m_toolHandles )
      {
        // digitalization
        if( tool->execute( ctx, descriptor ).isFailure() ){
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
  SG::ReadHandle<xAOD::CaloDetDescriptorCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloDetDescriptorCollection using this key: " << m_collectionKey);
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

//!=====================================================================

int CaloCellMaker::find( const std::vector<float> &vec, float value) const 
{
  auto binIterator = std::adjacent_find( vec.begin(), vec.end(), [=](float left, float right){ return left < value and value <= right; }  );
  if ( binIterator == vec.end() ) return -1;
  return  binIterator - vec.begin();
}


//!=====================================================================
/*
class CaloSampling(EnumStringification):
    PSB       = 0
    PSE       = 1
    EMB1      = 2
    EMB2      = 3
    EMB3      = 4
    TileCal1  = 5
    TileCal2  = 6
    TileCal3  = 7
    TileExt1  = 8
    TileExt2  = 9
    TileExt3  = 10
    EMEC1     = 11
    EMEC2     = 12
    EMEC3     = 13
    HEC1      = 14
    HEC2      = 15
    HEC3      = 16
*/
unsigned long int CaloCellMaker::hash(unsigned bin) const
{
  // NOTE: Max of 4,294,967,295 for unsigned long int (10 digits)

  // two for sammpling+side (0-54), one for segment (0-9) and six for bin index (0-999999), 9 digits
  if(m_zMin > 0){ // Right side 
    return ( (m_sampling + 1*17 ) * 1e7 + m_segment * 1e6 + bin);
  }else if(m_zMax < 0){// Left side
    return ( (m_sampling + 2*17 ) * 1e7 + m_segment * 1e6 + bin);
  }else{ // Center (barrel)
    return ( (m_sampling + 0*17 ) * 1e7 + m_segment * 1e6 + bin);
  }
}