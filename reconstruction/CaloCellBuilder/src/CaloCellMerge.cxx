
#include "CaloCell/CaloCellContainer.h"
#include "CaloCellMerge.h"
#include "TVector3.h"
#include <cstdlib>

#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2Poly.h"


using namespace Gaugi;
using namespace SG;


CaloCellMerge::CaloCellMerge( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "CollectionKeys"   , m_collectionKeys={}           );
  declareProperty( "CellsKey"         , m_cellsKey="Cells"            );
  declareProperty( "TruthCellsKey"    , m_truthCellsKey="TruthCells"  );
  declareProperty( "OutputLevel"      , m_outputLevel=1               );
}


CaloCellMerge::~CaloCellMerge()
{;}


StatusCode CaloCellMerge::initialize()
{
  setMsgLevel( m_outputLevel );
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::bookHistograms( SG::EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

  
StatusCode CaloCellMerge::execute( EventContext &/*ctx*/ , const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::post_execute( EventContext &ctx ) const
{

  MSG_DEBUG( "Starting collection merge algorithm..." );

  MSG_DEBUG( "Creating reco cells containers with key " << m_cellsKey);
  SG::WriteHandle<xAOD::CaloCellContainer> recoContainer( m_cellsKey , ctx );
  recoContainer.record( std::unique_ptr<xAOD::CaloCellContainer>(new xAOD::CaloCellContainer()) );
  
  MSG_DEBUG( "Creating truth cells containers with key " << m_truthCellsKey);
  SG::WriteHandle<xAOD::CaloCellContainer> truthContainer( m_truthCellsKey , ctx );
  truthContainer.record( std::unique_ptr<xAOD::CaloCellContainer>(new xAOD::CaloCellContainer()) );

  for ( auto key : m_collectionKeys ){

    MSG_DEBUG( "Reading all cells from collection with key " << key );
    SG::ReadHandle<xAOD::CaloCellCollection> collection( key, ctx );
    
    if( !collection.isValid() ){
      MSG_WARNING( "It's not possible to read the xAOD::CaloCellCollection from this Context using this key: " << key );
      continue;
    }

    MSG_DEBUG( "Creating new cells and attach the object into the container" );
    for (const auto &pair : **collection.ptr() )
    {
      // descriptor Cell with all geant/bunch/pulse information
      const xAOD::CaloDetDescriptor* descriptor=pair.second;
     
      // Create the truth cell 
      auto truth_cell = new xAOD::CaloCell();

      truth_cell->setEta( descriptor->eta() );
      truth_cell->setPhi( descriptor->phi() );
      truth_cell->setDeltaEta( descriptor->deltaEta() );
      truth_cell->setDeltaPhi( descriptor->deltaPhi() );
      truth_cell->setE( descriptor->edep() ); // The truth will be the energy deposity
      truth_cell->setEt( truth_cell->e() / std::cosh( truth_cell->eta() ) );

      truth_cell->setDescriptor( descriptor );
      truthContainer->push_back( truth_cell );

      // Create the Reco cell
      auto cell = new xAOD::CaloCell();
      truth_cell->setEta( descriptor->eta() );
      cell->setEta( descriptor->eta() );
      cell->setPhi( descriptor->phi() );
      cell->setDeltaEta( descriptor->deltaEta() );
      cell->setDeltaPhi( descriptor->deltaPhi() );
      cell->setE( descriptor->e() ); // Estimated energy from OF
      cell->setEt( cell->e() / std::cosh( cell->eta() ) );

      cell->setDescriptor( descriptor );
      recoContainer->push_back( cell );

    }// Loop over all descriptorCells
  }// Loop over all collections

  MSG_DEBUG( "All collections were merged into two CaloCellContainer" );
  return StatusCode::SUCCESS;
}


StatusCode CaloCellMerge::fillHistograms( EventContext & /*ctx*/ ) const
{

  /*
  auto store = ctx.getStoreGateSvc();
  SG::ReadHandle<xAOD::CaloCellContainer> container( m_cellsKey, ctx );
 
  if( !container.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellContainer using this key: " << m_cellsKey);
  }

  auto canvas = new TCanvas("canvas","canvas",500,500);
  auto hist = new TH2Poly();
  hist->SetName("cells_x");
  hist->SetTitle("Estimated Cells Energy; #eta; #phi; Energy [MeV]" );
 
  for ( const auto cell : **container.ptr() ){ 
    if( cell->layer() != CaloLayer::EM2 ) continue;
    hist->AddBin( cell->eta() - cell->deltaEta(), cell->eta() + cell->deltaEta(),
                  cell->phi() - cell->deltaPhi(), cell->phi() + cell->deltaPhi() );
  
  }
  hist->Ddescriptor("colz");
  canvas->SaveAs("test.pdf");
  delete canvas, hist;
  */

  return StatusCode::SUCCESS;
}



