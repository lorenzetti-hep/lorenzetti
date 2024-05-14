
#include "G4Kernel/CaloPhiRange.h"
#include "CaloCutBasedHypoTool.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

using namespace SG;
using namespace Gaugi;



CaloCutBasedHypoTool::CaloCutBasedHypoTool( std::string name ) : 
  IMsgService(name),
  Algorithm()
{
  declareProperty( "OutputLevel"    , m_outputLevel=1                   );
}

//!=====================================================================

CaloCutBasedHypoTool::~CaloCutBasedHypoTool()
{
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::initialize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::bookHistograms( SG::EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::execute( EventContext &ctx, int /*evt*/ ) const
{
  MSG_INFO("running hypo code");
  return post_execute(ctx);
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::post_execute( EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::fillHistograms(EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================




