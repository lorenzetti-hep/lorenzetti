
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
  declareProperty( "InputClusterKey",   m_clusterKey="Clusters"           );
  declareProperty( "OutputElectronKey", m_electronKey="Electrons"         );
  declareProperty( "OutputLevel"    ,   m_outputLevel=1                   );
  declareProperty( "TightCuts",         m_tightCuts                       );
  declareProperty( "MediumCuts",        m_mediumCuts                      );
  declareProperty( "LooseCuts",         m_looseCuts                       );
  declareProperty( "VLooseCuts",        m_vlooseCuts                      );
}

//!=====================================================================

CaloCutBasedHypoTool::~CaloCutBasedHypoTool()
{
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::initialize()
{
  CHECK_INIT();
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
  SG::WriteHandle<xAOD::ElectronContainer> electron(m_electronKey, ctx);
  electron.record( std::unique_ptr<xAOD::ElectronContainer>(new xAOD::ElectronContainer()) );

  SG::ReadHandle<xAOD::CaloClusterContainer> clusters(m_clusterKey, ctx);

  for( const auto* clus : **clusters.ptr()){
    auto el = new xAOD::Electron();
    std::vector<bool> decisions;
    decisions.push_back(computeDecision(clus,"tight"));
    decisions.push_back(computeDecision(clus,"medium"));
    decisions.push_back(computeDecision(clus,"loose"));
    decisions.push_back(computeDecision(clus,"vloose"));
    el->setDecisions(decisions);
    el->setEta(clus->eta());
    el->setPhi(clus->phi());
    el->setEt(clus->et());
    el->setE(clus->e());
    el->setCaloCluster( clus );
    electron->push_back( el );

  }
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode CaloCutBasedHypoTool::fillHistograms(EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

bool CaloCutBasedHypoTool::computeDecision(const xAOD::CaloCluster* cluster, std::string workingPoint) const{
  if (workingPoint == "tight"){
    if (cluster->rhad() > m_tightCuts[0]) return false;
    if (cluster->reta() < m_tightCuts[1]) return false;
    if (cluster->eratio() < m_tightCuts[2]) return false;
    return true;
  }
  if (workingPoint == "medium"){
    if (cluster->rhad() > m_mediumCuts[0]) return false;
    if (cluster->reta() < m_mediumCuts[1]) return false;
    if (cluster->eratio() < m_mediumCuts[2]) return false;
    return true;
  }
  if (workingPoint == "loose"){
    if (cluster->rhad() > m_looseCuts[0]) return false;
    if (cluster->reta() < m_looseCuts[1]) return false;
    if (cluster->eratio() < m_looseCuts[2]) return false;
    return true;
  }
  if (workingPoint == "vloose"){
    if (cluster->rhad() > m_vlooseCuts[0]) return false;
    if (cluster->reta() < m_vlooseCuts[1]) return false;
    if (cluster->eratio() < m_vlooseCuts[2]) return false;
    return true;
  }
  MSG_DEBUG("No working point adressed. Rejecting by default");
  return false;

}


