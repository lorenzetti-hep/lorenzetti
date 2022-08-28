#include "CrossTalk.h"
#include "CaloCell/CaloDetDescriptor.h"
#include "CaloCell/CaloDetDescriptorCollection.h"
#include "G4Kernel/CaloPhiRange.h"
#include "G4Kernel/constants.h"
#include "G4SystemOfUnits.hh"


using namespace Gaugi;


CrossTalk::CrossTalk( std::string name ) : 
  IMsgService(name),
  AlgTool()
{
  declareProperty( "MinEnergy"        , m_minEnergy=1*GeV          );
  declareProperty( "CollectionKey"    , m_collectionKey="CaloDetDescriptorCollection"  ); // input
}

//!=====================================================================

CrossTalk::~CrossTalk()
{}

//!=====================================================================

StatusCode CrossTalk::initialize()
{
  setMsgLevel(m_outputLevel);
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CrossTalk::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CrossTalk::execute( SG::EventContext &ctx, Gaugi::EDM *edm ) const
{
  auto *hotcell = static_cast<xAOD::CaloDetDescriptor*>(edm);


  // Step 1: check if we need to apply cx methos. Only for cells higher than
  // min energy. At this point we dont have the estimated energy from pulse.
  // Here, lets use the truth energy from the main bunch crossing.
  if (hotcell->edep() > m_minEnergy){
    
    // Since this is a cell candidate, lets take all cells around this cells using a 3x3 window.
    // First lets retrieve the full container in memory (not const objects inside of the collection)
    SG::ReadHandle<xAOD::CaloDetDescriptorCollection> collection( m_collectionKey, ctx );
    if( !collection.isValid() ){
      MSG_FATAL("It's not possible to retrieve the CaloDetDescriptorCollection using this key: " << m_collectionKey);
    }

    
    std::vector<xAOD::CaloDetDescriptor*> cells_around;
    for (auto &pair : **collection.ptr()){
      auto *cell = pair.second;
      float deltaEta = std::abs( hotcell->eta() - cell->eta() );
      float deltaPhi = std::abs( CaloPhiRange::fix( hotcell->phi() - cell->phi() ) );
      if (cell == hotcell){
        continue;
      }
      // 3x3 window around the hotcell
      if( deltaEta < 3*cell->deltaEta()/2 && deltaPhi < 3*cell->deltaPhi()/2 ){
        cells_around.push_back(cell);
      }
    }

    //MSG_INFO("We find numbe rof celles "<<cells_around.size() );
    

    // With cells_around and hotcell , we can apply the crosstalk logic.
    // NOTE: at this point we can modify all objects (CaloDet) since 
    // we not make this as const.


  }

  return StatusCode::SUCCESS;
}


