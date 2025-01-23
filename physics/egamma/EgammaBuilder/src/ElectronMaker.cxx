
#include "G4Kernel/CaloPhiRange.h"
#include "ElectronMaker.h"
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

using namespace SG;
using namespace Gaugi;



ElectronMaker::ElectronMaker( std::string name ) : 
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

ElectronMaker::~ElectronMaker()
{
}

//!=====================================================================

StatusCode ElectronMaker::initialize()
{
  CHECK_INIT();
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode ElectronMaker::bookHistograms( SG::EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode ElectronMaker::pre_execute( EventContext &/*ctx*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode ElectronMaker::execute( EventContext &/*ctx*/, const G4Step * /*step*/ ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode ElectronMaker::execute( EventContext &ctx, int /*evt*/ ) const
{
  return post_execute(ctx);
}

//!=====================================================================

StatusCode ElectronMaker::finalize()
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode ElectronMaker::post_execute( EventContext &ctx ) const
{
  SG::WriteHandle<xAOD::ElectronContainer> electron(m_electronKey, ctx);
  electron.record( std::unique_ptr<xAOD::ElectronContainer>(new xAOD::ElectronContainer()) );
  SG::ReadHandle<xAOD::CaloClusterContainer> clusters(m_clusterKey, ctx);

  for( const auto* clus : **clusters.ptr()){
    auto el = new xAOD::Electron();
    el->setEta(clus->eta());
    el->setPhi(clus->phi());
    el->setEt(clus->et());
    el->setE(clus->e());
    el->setCaloCluster( clus );
    el->setIsTight(compute(clus,"tight"));
    el->setIsMedium(compute(clus,"medium"));
    el->setIsLoose(compute(clus,"loose"));
    el->setIsVeryLoose(compute(clus,"vloose"));
    electron->push_back( el );

  }
  return StatusCode::SUCCESS;
}

//!=====================================================================

StatusCode ElectronMaker::fillHistograms(EventContext &ctx ) const
{
  return StatusCode::SUCCESS;
}

//!=====================================================================

bool ElectronMaker::compute(const xAOD::CaloCluster* cluster, std::string pidname) const{
  
  if(cluster->isForward()) {
    if (pidname == "loose"){
      if (cluster->secondLambda() > m_secondLambdaCuts[0])  return false;
      if (cluster->lateralMom() > m_lateralMomCuts[0])      return false;
      if (cluster->longitudinalMom() > m_longMomCuts[0])    return false;
      if (cluster->fracMax() < m_fracMaxCuts[0])            return false;
      if (cluster->secondR() > m_secondRCuts[0])            return false;
      if (cluster->lambdaCenter() > m_lambdaCenterCuts[0])  return false;
      return true;
    }
    else if (pidname == "medium"){
      if (cluster->secondLambda()    > m_secondLambdaCuts[1]) return false;
      if (cluster->lateralMom()      > m_lateralMomCuts[1])   return false;
      if (cluster->longitudinalMom() > m_longMomCuts[1])      return false;
      if (cluster->fracMax()         < m_fracMaxCuts[1])      return false;
      if (cluster->secondR()         > m_secondRCuts[1])      return false;
      if (cluster->lambdaCenter()    > m_lambdaCenterCuts[1]) return false;
      return true;
    }
    else if (pidname == "tight"){
      if (cluster->secondLambda()    > m_secondLambdaCuts[2]) return false;
      if (cluster->lateralMom()      > m_lateralMomCuts[2])   return false;
      if (cluster->longitudinalMom() > m_longMomCuts[2])      return false;
      if (cluster->fracMax()         < m_fracMaxCuts[2])      return false;
      if (cluster->secondR()         > m_secondRCuts[2])      return false;
      if (cluster->lambdaCenter()    > m_lambdaCenterCuts[2]) return false;
      return true;
    }

  }else{

    if (pidname == "tight"){
      if (cluster->rhad()   > m_tightCuts[0]) return false;
      if (cluster->reta()   < m_tightCuts[1]) return false;
      if (cluster->eratio() < m_tightCuts[2]) return false;
      return true;
    }
    else if (pidname == "medium"){
      if (cluster->rhad()   > m_mediumCuts[0]) return false;
      if (cluster->reta()   < m_mediumCuts[1]) return false;
      if (cluster->eratio() < m_mediumCuts[2]) return false;
      return true;
    }
    else if (pidname == "loose"){
      if (cluster->rhad()   > m_looseCuts[0]) return false;
      if (cluster->reta()   < m_looseCuts[1]) return false;
      if (cluster->eratio() < m_looseCuts[2]) return false;
      return true;
    }
    if (pidname == "vloose"){
      if (cluster->rhad()   > m_vlooseCuts[0]) return false;
      if (cluster->reta()   < m_vlooseCuts[1]) return false;
      if (cluster->eratio() < m_vlooseCuts[2]) return false;
      return true;
    }
  }
  MSG_DEBUG("No working point adressed. Rejecting by default");
  return false;
}


