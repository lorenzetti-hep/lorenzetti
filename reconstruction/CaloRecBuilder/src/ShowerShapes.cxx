
#include "CaloCluster/CaloCluster.h"
#include "G4Kernel/CaloPhiRange.h"
#include "ShowerShapes.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

using namespace std;
using namespace SG;
using namespace CaloSampling;



ShowerShapes::ShowerShapes( std::string name ) : 
  IMsgService(name),
  AlgTool()
{;}


StatusCode ShowerShapes::initialize()
{
  return StatusCode::SUCCESS;
}

StatusCode ShowerShapes::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode ShowerShapes::executeTool( const xAOD::EventInfo * /*evt*/, Gaugi::EDM *edm ) const
{
  MSG_DEBUG("Calculate shower shapes for this cluster." );
  
  auto *clus = static_cast<xAOD::CaloCluster*>(edm);

  // Eratio for strip em layer (EM1)
  auto em1Cells = clus->allCells();
	em1Cells.erase(std::remove_if(em1Cells.begin(),em1Cells.end(),
                [](const xAOD::CaloCell* c){return c->sampling() != CaloSampling::CaloSample::EM1;}),
                em1Cells.end());
  std::sort(em1Cells.begin(), em1Cells.end(),
            [](const xAOD::CaloCell* c1, const xAOD::CaloCell* c2){return c1->energy() > c2->energy();});

  
  
  float emaxs1  = (em1Cells.size()>=4)?(em1Cells[0]->energy() + em1Cells[1]->energy()):0;
  float e2tsts1 = (em1Cells.size()>=4)?(em1Cells[2]->energy() + em1Cells[3]->energy()):0;
  float eratio = (emaxs1 + e2tsts1)?((emaxs1 - e2tsts1)/(emaxs1 + e2tsts1)):0.;

 
  
  float e277 = sumEnergy( clus, CaloSample::EM2, 7, 7 );
  float e233 = sumEnergy( clus, CaloSample::EM2, 3, 3 );
  float e237 = sumEnergy( clus, CaloSample::EM2, 3, 7 );
  float reta = e237/e277;
  float rphi = e233/e237;
  float e0 = sumEnergy( clus, CaloSample::PS );
  float e1 = sumEnergy( clus, CaloSample::EM1 );
  float e2 = sumEnergy( clus, CaloSample::EM2 );
  float e3 = sumEnergy( clus, CaloSample::EM3 );
  float ehad1 = sumEnergy( clus, CaloSample::HAD1 ) + sumEnergy( clus, CaloSample::HAD1_Extended );
  float ehad2 = sumEnergy( clus, CaloSample::HAD2 ) + sumEnergy( clus, CaloSample::HAD2_Extended );
  float ehad3 = sumEnergy( clus, CaloSample::HAD3 ) + sumEnergy( clus, CaloSample::HAD3_Extended );
    
  float weta2 = calculateWeta2(clus, CaloSample::EM2, 3, 5);

  float etot = e0+e1+e2+e3+ehad1+ehad2+ehad3;
  float emtot = e0+e1+e2+e3;
  // fraction of energy deposited in 1st sampling
  float f0 = e0 / emtot;
  float f1 = e1 / emtot;
  float f2 = e2 / emtot;
  float f3 = e3 / emtot;

  float rhad = (ehad1+ehad2+ehad3) / emtot;
  float rhad1 = (ehad1) / emtot;

  clus->setE0( e0 );
  clus->setE1( e1 );
  clus->setE2( e2 );
  clus->setE3( e3 );
  clus->setEhad1( ehad1 );
  clus->setEhad2( ehad2 );
  clus->setEhad3( ehad3 );
  clus->setEtot( etot );
  clus->setE277( e277 );
  clus->setE237( e237 );
  clus->setE233( e233 );
  clus->setReta( reta );
  clus->setRphi( rphi );
  clus->setWeta2( weta2 );
  // Eratio for strip em layer (EM1)
  clus->setEratio( eratio );
  // Extra eratio information for each sampling layer
  clus->setEmaxs1( emaxs1 );
  clus->setE2tsts1( e2tsts1 );
  clus->setF0( f0 );
  clus->setF1( f1 );
  clus->setF2( f2 );
  clus->setF3( f3 );
  clus->setRhad( rhad );
  clus->setRhad1( rhad1 );
  // Only EM energy since this is a eletromagnetic cluster
  clus->setEt( clus->eta() != 0.0 ? (e0+e1+e2+e3)/cosh(fabs(clus->eta())) : 0.0 ); 
  return StatusCode::SUCCESS;
}


float ShowerShapes::sumEnergy( xAOD::CaloCluster *clus, CaloSample sampling, unsigned eta_ncell, unsigned phi_ncell ) const
{
  float energy = 0.0;
  for ( const auto& cell : clus->allCells() )
  {
    if(cell->sampling() != sampling)  continue;
    float deltaEta = std::abs( clus->eta() - cell->eta() );
    float deltaPhi = std::abs( CaloPhiRange::fix( clus->phi() - cell->phi() ) );
    
    if( deltaEta < eta_ncell*cell->deltaEta() && deltaPhi < phi_ncell*cell->deltaPhi() ){
      energy+=cell->energy();
    }
  }
  return energy;
}





float ShowerShapes::calculateWeta2( xAOD::CaloCluster *clus , CaloSample sampling, unsigned eta_ncell, unsigned phi_ncell) const
{
  float En2=0.0;
  float En=0.0;
  float E=0.0;

  for ( auto& cell : clus->allCells() ){
    if(cell->sampling() != sampling)  continue;
    
    float deltaEta = std::abs( clus->eta() - cell->eta() );
    float deltaPhi = std::abs( CaloPhiRange::diff( clus->phi() , cell->phi() ) );
 
    if( deltaEta < eta_ncell*cell->deltaEta() && deltaPhi < phi_ncell*cell->deltaPhi() ){
      En2 += cell->energy() * std::pow(cell->eta(),2);
      En += cell->energy() * cell->eta();
      E += cell->energy();
    }
  }// Loop over all cells inside of the cluster

  return std::sqrt( (En2/E) - std::pow( (En/E),2 ) );
}


