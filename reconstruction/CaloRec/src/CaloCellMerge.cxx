#include "CaloCluster/CaloClusterContainer.h"
#include "G4Kernel/EventReader.h"
#include "CaloCellMaker.h"
#include "TVector3.h"
#include <cstdlib>

using namespace Gaugi;
using namespace SG;
using namespace CaloSampling;




CaloCellMaker::CaloCellMaker( std::string name ) : 
  Algorithm( name ),
  IMsgService(name),
  m_bcid_truth( special_bcid_for_truth_reconstruction )
{

  declareProperty( "HistogramPath"    , m_histPath="/CaloCellMaker"           );
  declareProperty( "CaloCellFile"     , m_caloCellFile                        );
  declareProperty( "CollectionKey"    , m_collectionKey="CaloCellCollection"  );
  declareProperty( "OutputLevel"      , m_outputLevel=MSG::INFO               );
  
}



CaloCellMaker::~CaloCellMaker()
{;}



void CaloCellMaker::push_back( CaloRecTool* tool )
{
  m_toolHandles.push_back(tool);
}


StatusCode CaloCellMaker::initialize()
{
  // Set message level
  setMsgLevel( (MSG::Level)m_outputLevel );
  return StatusCode::SUCCESS;
}



StatusCode CaloCellMaker::finalize()
{
  return StatusCode::SUCCESS;
}





StatusCode CaloCellMaker::pre_execute( EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}



  
StatusCode CaloCellMaker::execute( EventContext &ctx , const G4Step *step ) const
{
  return StatusCode::SUCCESS;
}




StatusCode CaloCellMaker::post_execute( EventContext &ctx ) const
{

  SG::WriteHandle<xAOD::CaloCellContainer> cells( m_cellKey , ctx );
  SG::WriteHandle<xAOD::CaloCellContainer> truth_cells( m_truthCellKey , ctx );


  for ( auto key : m_collectionKeys ){

    SG::ReadHandle<xAOD::CaloCellCollection> collection( key, ctx );
    
    if( !collection.isValid() ){
      MSG_WARNING( "It's not possible to read the xAOD::CaloCellCollection from this Context using this key: " << key );
      continue;
    }


    for (const auto &pair : **collection.ptr() )
    {
      const auto* cell=pair.second;
     
      
      auto pp = new xAOD::CaloCell( cell );
      pp->setEnergy( cell->energy() );
      


      // Cell for TruthParticle
      auto mc = new xAOD::CaloCell( cell );
      mc->setEnergy( cell->truthRawEnergy() )


      // Prepare the cell for pp mode
      cell_pp = cell.copy();
      

      cell_truth = cell.copy();



    }
  }




 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellCollection using this key: " << m_collectionKey);
  }


  for ( const auto& p : **collection.ptr() )
  {
    for ( auto tool : m_toolHandles )
    {
      if( tool->executeTool( p.second ).isFailure() ){
        MSG_ERROR( "It's not possible to execute the tool with name " << tool->name() );
        return StatusCode::FAILURE;
      }
    }
  }

 

  return StatusCode::SUCCESS;
}




StatusCode CaloCellMaker::fillHistograms( EventContext &ctx ) const
{
  
  SG::ReadHandle<xAOD::CaloCellCollection> collection( m_collectionKey, ctx );
 
  if( !collection.isValid() ){
    MSG_FATAL("It's not possible to retrieve the CaloCellCollection using this key: " << m_collectionKey);
  }

  
  auto store = getStoreGateSvc();

  for ( const auto& p : **collection.ptr() ){ 
    const auto *cell = p.second;
    // Skip cells with energy equal zero
    std::stringstream ss; ss << m_histPath+"/cells_layer_" << (int)cell->sampling();
    int x = store->hist2(ss.str())->GetXaxis()->FindBin(cell->eta());
    int y = store->hist2(ss.str())->GetYaxis()->FindBin(cell->phi());
    int bin = store->hist2(ss.str())->GetBin(x,y,0);
    float energy = store->hist2(ss.str())->GetBinContent( bin );
    store->hist2(ss.str())->SetBinContent( bin, (energy + cell->energy()) );
  }

  return StatusCode::SUCCESS;
}



