
#include "CaloCell/CaloCellContainer.h"
#include "CaloCell/CaloDetDescriptorCollection.h"

#include "CaloRingsMerge.h"
#include "TVector3.h"
#include <cstdlib>

#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2Poly.h"


using namespace Gaugi;
using namespace SG;


CaloRingsMerge::CaloRingsMerge( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "InputRingsKeys"         , m_collectionKeys={}           );
  declareProperty( "OutputRingsKey"         , m_ringerKey="Rings"           );
  declareProperty( "OutputLevel"            , m_outputLevel=1               );
}

//!=====================================================================

CaloRingsMerge::~CaloRingsMerge()
{;}

//!=====================================================================

StatusCode CaloRingsMerge::initialize()
{
  CHECK_INIT();
  setMsgLevel( m_outputLevel );
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloRingsMerge::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloRingsMerge::bookHistograms( SG::EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloRingsMerge::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloRingsMerge::execute( EventContext &/*ctx*/ , const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloRingsMerge::execute( EventContext &ctx , int /*evt*/ ) const
{
  return post_execute(ctx);
}

//!=====================================================================

StatusCode CaloRingsMerge::post_execute( EventContext &ctx ) const
{

  MSG_DEBUG( "Starting collection merge algorithm..." );

  SG::WriteHandle<xAOD::CaloRingsContainer> ringsContainer( m_ringerKey , ctx );
  ringsContainer.record( std::unique_ptr<xAOD::CaloRingsContainer>(new xAOD::CaloRingsContainer()) );

  
  for ( auto key : m_collectionKeys ){

    MSG_DEBUG( "Reading all cells from collection with key " << key );

    MSG_DEBUG( "Reading the CaloRingsContainer from the Context" );
    SG::ReadHandle<xAOD::CaloRingsContainer> collection( key , ctx );
    
    if( !collection.isValid() ){
      MSG_WARNING( "It's not possible to read the xAOD::CaloRingsCollection from this Context using this key: " << key );
      continue;
    }

    MSG_DEBUG( "Creating new ring and attach the object into the container" ); 
    for( auto ring : *collection ){
      xAOD::CaloRings* newRing = new xAOD::CaloRings();
      newRing->setRings( ring->rings() );
      newRing->setCaloCluster( ring->caloCluster() );
      ringsContainer->push_back( newRing );
    }

  }// Loop over all collections

  MSG_DEBUG( "All collections were merged..." );
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloRingsMerge::fillHistograms( EventContext & /*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}



