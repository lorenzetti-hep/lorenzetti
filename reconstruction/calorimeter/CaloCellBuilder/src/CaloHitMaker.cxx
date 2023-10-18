

#include "CaloHit/CaloHit.h"
#include "CaloHit/CaloHitCollection.h"
#include "EventInfo/EventInfoContainer.h"
#include "CaloHitMaker.h"

#include "G4Kernel/CaloPhiRange.h"
#include "G4Kernel/constants.h"
#include "TVector3.h"
#include "G4SystemOfUnits.hh"

#include "TH1F.h"
#include "TH2F.h"
#include "TH2Poly.h"
#include "TGraph.h"
#include <cstdlib>

using namespace Gaugi;
using namespace SG;




CaloHitMaker::CaloHitMaker( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "OutputCollectionKey"      , m_collectionKey="CaloHitCollection"   ); // output
  
  declareProperty( "EtaBins"                  , m_etaBins                             );
  declareProperty( "PhiBins"                  , m_phiBins                             );
  declareProperty( "RMin"                     , m_rMin                                );
  declareProperty( "RMax"                     , m_rMax                                );
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
  declareProperty( "HistogramPath"            , m_histPath="/CaloHitMaker"            );
  declareProperty( "SamplingNoiseStd"         , m_noiseStd=0                          );



}

//!=====================================================================

StatusCode CaloHitMaker::initialize()
{
  MSG_INFO("1 Initialize..." << m_outputLevel);

  CHECK_INIT();
  setMsgLevel( (MSG::Level)m_outputLevel );
  m_nEtaBins = m_etaBins.size() - 1;
  m_nPhiBins = m_phiBins.size() - 1;

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
  MSG_DEBUG("Booking histograms...");
  auto store = ctx.getStoreGateSvc();

  store->mkdir(m_histPath);

  // Create the 2D histogram for monitoring purpose
  store->add(new TH2F( "hits_edep", "Hit Energy (Truth); #eta; #phi; Energy [MeV]", m_nEtaBins, m_etaBins.data(), m_nPhiBins, m_phiBins.data() ) );
  
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMaker::pre_execute( EventContext &ctx ) const
{
  // Build the CaloHitCollection and attach into the EventContext
  // Create the hit collection into the event context
  SG::WriteHandle<xAOD::CaloHitCollection> collection( m_collectionKey, ctx );
  collection.record( std::unique_ptr<xAOD::CaloHitCollection>(new xAOD::CaloHitCollection()) );

  float deltaEta = std::abs(m_etaBins[1] - m_etaBins[0]);
  float deltaPhi = std::abs(m_phiBins[1] - m_phiBins[0]);

  //
  // Prepare all sensitive objects like a two dimensional histogram
  //
  for ( unsigned etaBin = 0; etaBin < m_nEtaBins; ++etaBin){ // Rows

    //if (std::abs(m_etaBins[etaBin]) == std::abs(m_etaBins[etaBin+1]))
    //  continue;

    for ( unsigned phiBin = 0; phiBin < m_nPhiBins; ++phiBin){ // Cols

      float etaCenter = m_etaBins[etaBin] + deltaEta / 2;
      float phiCenter = m_phiBins[phiBin] + deltaPhi / 2;
     
      // local hash
      unsigned bin = m_nPhiBins * etaBin + phiBin;
      
      // Create the calorimeter cell
      auto *hit = new xAOD::CaloHit( etaCenter, phiCenter, deltaEta, deltaPhi, hash(bin), 
                                     (CaloSampling)m_sampling,
                                     (Detector)m_detector,
                                     m_bc_duration, m_bcid_start, m_bcid_end );
      if( !collection->insert( hit->hash(), hit) )
      {
        MSG_FATAL( "It is not possible to include hit hash ("<< hit->hash() << ") into the collection. hash already exist.");
      }

    } // Loop over phi bins
  }// Loop over eta bins

  MSG_DEBUG("Pre_execute done.");
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
  float eta = vpos.PseudoRapidity();
  float phi = vpos.Phi();
  float radius = vpos.Perp();

  // In plan xy
  if( !(radius > m_rMin && radius <= m_rMax) )
    return StatusCode::SUCCESS;

  if( !(pos.z() > m_zMin && pos.z() <= m_zMax))
    return StatusCode::SUCCESS;

  int etaBin = find(m_etaBins, eta);

  if(etaBin < 0) 
    return StatusCode::SUCCESS;

  int phiBin = find(m_phiBins, phi);

  if(phiBin < 0)
    return StatusCode::SUCCESS;

  int bin = m_nPhiBins * etaBin + phiBin;

  xAOD::CaloHit *hit=nullptr;

  if(collection->retrieve(hash(bin), hit)){
    // hit->fill( step );
    hit->fill( step , 1*m_noiseStd); // hit with tof selection sensible by 1*sigma of sampling noise.
  }else{
    MSG_FATAL( "Its not possible to retrieve the hit. Bin ("<< bin << ") not exist");
  }
  
  return StatusCode::SUCCESS;
}

//!=====================================================================

// standlone execute
StatusCode CaloHitMaker::execute( EventContext &/*ctx*/, int /*evt*/ ) const
{
  MSG_ERROR("This method can not be execute in standalone mode.");
  return StatusCode::FAILURE;
}

//!=====================================================================

StatusCode CaloHitMaker::post_execute( EventContext &/*ctx*/ ) const
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

//!=====================================================================

int CaloHitMaker::find( const std::vector<float> &vec, float value) const 
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
unsigned long int CaloHitMaker::hash(unsigned bin) const
{
  // NOTE: Max of 4,294,967,295 for unsigned long int (10 digits)

  // two for sammpling+side (0-99), one for segment (0-9) and six for bin index (0-999999), 9 digits
  if(m_zMin > 0){ // Right side 
    return ( (m_sampling + 1*17 ) * 1e7 + m_segment * 1e6 + bin);
  }else if(m_zMax < 0){// Left side
    return ( (m_sampling + 2*17 ) * 1e7 + m_segment * 1e6 + bin);
  }else{ // Center (barrel)
    return ( (m_sampling + 0*17 ) * 1e7 + m_segment * 1e6 + bin);
  }
}
