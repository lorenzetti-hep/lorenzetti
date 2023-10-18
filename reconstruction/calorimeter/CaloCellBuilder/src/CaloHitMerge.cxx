
#include "CaloHit/CaloHitContainer.h"
#include "CaloHit/CaloHitCollection.h"

#include "CaloHitMerge.h"
#include "TVector3.h"
#include <cstdlib>

#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2Poly.h"


using namespace Gaugi;
using namespace SG;


CaloHitMerge::CaloHitMerge( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "InputCollectionKeys"    , m_collectionKeys={}           );
  declareProperty( "OutputHitsKey"          , m_hitsKey="Hits"              );
  declareProperty( "OutputLevel"            , m_outputLevel=1               );
}

//!=====================================================================

CaloHitMerge::~CaloHitMerge()
{;}

//!=====================================================================

StatusCode CaloHitMerge::initialize()
{
  CHECK_INIT();
  setMsgLevel( m_outputLevel );
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMerge::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMerge::bookHistograms( SG::EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMerge::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================
 
StatusCode CaloHitMerge::execute( EventContext &/*ctx*/ , const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================
// standlone execute
StatusCode CaloHitMerge::execute( EventContext &ctx, int /*evt*/ ) const
{
  return post_execute(ctx);
}

//!=====================================================================

StatusCode CaloHitMerge::post_execute( EventContext &ctx ) const
{

  MSG_DEBUG( "Starting collection merge algorithm..." );
  MSG_DEBUG( "Creating HIT containers with key " << m_hitsKey);

  SG::WriteHandle<xAOD::CaloHitContainer> container( m_hitsKey , ctx );
  container.record( std::unique_ptr<xAOD::CaloHitContainer>(new xAOD::CaloHitContainer()) );
  float etot=0;

  for ( auto key : m_collectionKeys ){

    MSG_DEBUG( "Reading all hits from collection with key " << key );
    SG::ReadHandle<xAOD::CaloHitCollection> collection( key, ctx );
    
    if( !collection.isValid() ){
      MSG_WARNING( "It's not possible to read the xAOD::CaloHitCollection from this Context using this key: " << key );
      continue;
    }

    MSG_DEBUG( "Creating new cells and attach the object into the container" );

    for (const auto &pair : **collection.ptr() )
    {
      const xAOD::CaloHit* const_hit=pair.second;
      
      // Create the truth cell 
      auto hit = new xAOD::CaloHit( const_hit->eta(),
                                    const_hit->phi(),
                                    const_hit->deltaEta(),
                                    const_hit->deltaPhi(),
                                    const_hit->hash(),
                                    const_hit->sampling(),
                                    const_hit->detector(),
                                    const_hit->bc_duration(),
                                    const_hit->bcid_start(),
                                    const_hit->bcid_end()
                                    );


      for ( int bcid = hit->bcid_start();  bcid <= hit->bcid_end(); ++bcid )
      {
        hit->edep( bcid, const_hit->edep(bcid) ); // truth energy for each bunch crossing
        hit->tof ( bcid, const_hit->tof(bcid) );  // truth time for each bunch crossing 
                                                  // (in fact, this time is the time from the signal at the
                                                  // moment its energy holds above 1*sigma noise threshold)
      }
      etot+=hit->edep();

      container->push_back( hit );
      
    }// Loop over all hits 
  }// Loop over all collections
  
  MSG_DEBUG( "Total of energy is " << etot << " MeV");
  MSG_DEBUG( "All collections were merged into two CaloHitContainer" );
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloHitMerge::fillHistograms( EventContext & /*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}



