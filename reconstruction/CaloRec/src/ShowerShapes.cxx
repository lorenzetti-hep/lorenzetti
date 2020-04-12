
#include "ShowerShapes.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

using namespace std;
using namespace SG;
using namespace CaloSampling;



ShowerShapes::ShowerShapes( std::string name ) : 
  IMsgService(name),
  CaloRecTool()
{;}


StatusCode ShowerShapes::initialize()
{
  return StatusCode::SUCCESS;
}

StatusCode ShowerShapes::finalize()
{
  return StatusCode::SUCCESS;
}


StatusCode ShowerShapes::executeTool( xAOD::CaloCluster* clus ) const
{
  MSG_INFO("Calculate shower shapes for this cluster." );
  
  // Eratio for strip em layer (EM1)
  auto em1Cells = sortCells( clus , CaloSample::EM1 );
  float emaxs1  = (em1Cells.size()>=4)?(em1Cells[0]->energy() + em1Cells[1]->energy()):0;
  float e2tsts1 = (em1Cells.size()>=4)?(em1Cells[2]->energy() + em1Cells[3]->energy()):0;
  float eratio = (emaxs1 + e2tsts1)?((emaxs1 - e2tsts1)/(emaxs1 + e2tsts1)):0.;

  // Extra eratio information for each sampling layer
  float eratio2 = calculateEratio( clus, CaloSample::EM2  );
  float eratio3 = calculateEratio( clus, CaloSample::EM3  );
  float eratio4 = calculateEratio( clus, CaloSample::HAD1 );
  float eratio5 = calculateEratio( clus, CaloSample::HAD2 );
  float eratio6 = calculateEratio( clus, CaloSample::HAD3 );
  
  
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
  MSG_INFO("WETA2 = " << weta2 );

  float etot = e0+e1+e2+e3+ehad1+ehad2+ehad3;

  // fraction of energy deposited in 1st sampling
  float f0 = e0 / etot;
  float f1 = e1 / etot;
  float f2 = e2 / etot;
  float f3 = e3 / etot;
  float rhad = (ehad1+ehad2+ehad3) / etot;

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
  clus->setEratio2( eratio2 );
  clus->setEratio3( eratio3 );
  clus->setEratio4( eratio4 );
  clus->setEratio5( eratio5 );
  clus->setEratio6( eratio6 );
  clus->setEmaxs1( emaxs1 );
  clus->setE2tsts1( e2tsts1 );
  clus->setF0( f0 );
  clus->setF1( f1 );
  clus->setF2( f2 );
  clus->setF3( f3 );
  clus->setRhad( rhad );
  clus->setEt( clus->eta() != 0.0 ? clus->etot()/cosh(fabs(clus->eta())) : 0.0 ); 
  return StatusCode::SUCCESS;
}


float ShowerShapes::sumEnergy( xAOD::CaloCluster *clus, CaloSample sampling, unsigned eta_ncell, unsigned phi_ncell ) const
{
  float energy = 0.0;
  for ( const auto& cell : clus->allCells() )
  {
    if(cell->sampling() != sampling)  continue;
    // deta/dphi is the half of the cell sizei
    if( ( cell->eta() > ( clus->eta() - eta_ncell * cell->deltaEta() ) ) && 
        ( cell->eta() < ( clus->eta() + eta_ncell * cell->deltaEta() ) ) )
    {
      if( ( cell->phi() > ( clus->phi() - phi_ncell * cell->deltaPhi() ) ) && 
          ( cell->phi() < ( clus->phi() + phi_ncell * cell->deltaPhi() ) ) )
      {  
          energy+=cell->energy();
      }
    }
  }
  return energy;
}


// Just for python import in ROOT
StatusCode ShowerShapes::executeTool( xAOD::CaloCell * ) const {return StatusCode::SUCCESS;}
StatusCode ShowerShapes::executeTool( xAOD::RawCell * ) const {return StatusCode::SUCCESS;}
StatusCode ShowerShapes::executeTool( xAOD::TruthParticle * ) const {return StatusCode::SUCCESS;}




std::vector<const xAOD::CaloCell*> ShowerShapes::sortCells( xAOD::CaloCluster *clus , CaloSample sampling) const
{
  std::vector<const xAOD::CaloCell*> cells;
  for ( auto& cell : clus->allCells() ){
    if(cell->sampling() != sampling )  continue;
    cells.push_back(cell);
  }
  std::sort(cells.begin(), cells.end(),[](const xAOD::CaloCell* c1, const xAOD::CaloCell* c2){return c1->energy() > c2->energy();});
  return cells;
}


float ShowerShapes::calculateEratio( xAOD::CaloCluster *clus, CaloSample sampling) const 
{
  auto cells = sortCells( clus , sampling );
  float emax  = (cells.size()>=4)?(cells[0]->energy() + cells[1]->energy()):0;
  float e2tst = (cells.size()>=4)?(cells[2]->energy() + cells[3]->energy()):0;
  return (emax + e2tst)?((emax - e2tst)/(emax + e2tst)):0.;
}


float ShowerShapes::calculateWeta2( xAOD::CaloCluster *clus , CaloSample sampling, unsigned eta_ncell, unsigned phi_ncell) const
{
  MSG_INFO("Calculate Weta2");
  //int eta_ncell = 3;
  //int phi_ncell = 5;

  float En2=0.0;
  float En=0.0;
  float E=0.0;

  for ( auto& cell : clus->allCells() ){
    if(cell->sampling() != sampling)  continue;
    // deta/dphi is the half of the cell sizei
    if( ( cell->eta() > ( clus->eta() - eta_ncell * cell->deltaEta() ) ) && 
        ( cell->eta() < ( clus->eta() + eta_ncell * cell->deltaEta() ) ) )
    {
      if( ( cell->phi() > ( clus->phi() - phi_ncell * cell->deltaPhi() ) ) && 
          ( cell->phi() < ( clus->phi() + phi_ncell * cell->deltaPhi() ) ) )
      {  



          En2 += cell->energy() * std::pow(cell->eta(),2);
          En += cell->energy() * cell->eta();
          E += cell->energy();
      }
    }
  }// Loop over all cells inside of the cluster

  return std::sqrt( (En2/E) - std::pow( (En/E),2 ) );
}








