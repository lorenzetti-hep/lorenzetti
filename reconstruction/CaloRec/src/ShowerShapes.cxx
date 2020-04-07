
#include "ShowerShapes.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

using namespace std;
using namespace SG;
using namespace CaloSampling;



ShowerShapes::ShowerShapes( std::string name ) : 
  CaloRecTool( name ),
  IMsgService(name)
{;}



ShowerShapes::~ShowerShapes()
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
  auto em1Cells = clus->allCells();

	em1Cells.erase(std::remove_if(em1Cells.begin(),
                                em1Cells.end(),
                              [](const xAOD::CaloCell* c){return c->sampling() != CaloSample::EM1;}),
                 em1Cells.end());

  std::sort(em1Cells.begin(), em1Cells.end(),
                              [](const xAOD::CaloCell* c1, const xAOD::CaloCell* c2){return c1->energy() > c2->energy();});


  float emaxs1 = (em1Cells.size()>=4)?(em1Cells[0]->energy() + em1Cells[1]->energy()):0;
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
  clus->setEratio( eratio );
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






StatusCode ShowerShapes::executeTool( xAOD::TruthParticle *particle ) const
{
  MSG_INFO("Calculate shower shapes for this particle." );
  auto em1Cells = particle->allCells();
	em1Cells.erase(std::remove_if(em1Cells.begin(),
                                em1Cells.end(),
                              [](const xAOD::CaloCell* c){return c->sampling() != CaloSample::EM1;}),
                 em1Cells.end());

  std::sort(em1Cells.begin(), em1Cells.end(),
                              [](const xAOD::CaloCell* c1, const xAOD::CaloCell* c2){return c1->truthRawEnergy() > c2->truthRawEnergy();});


  float emaxs1 = (em1Cells.size()>=4)?(em1Cells[0]->truthRawEnergy() + em1Cells[1]->truthRawEnergy()):0;
  float e2tsts1 = (em1Cells.size()>=4)?(em1Cells[2]->truthRawEnergy() + em1Cells[3]->truthRawEnergy()):0;
  float eratio = (emaxs1 + e2tsts1)?((emaxs1 - e2tsts1)/(emaxs1 + e2tsts1)):0.;

  float e277 = sumEnergy( particle, CaloSample::EM2, 7, 7 );
  float e233 = sumEnergy( particle, CaloSample::EM2, 3, 3 );
  float e237 = sumEnergy( particle, CaloSample::EM2, 3, 7 );

  float reta = e237/e277;
  float rphi = e233/e237;

  float e0 = sumEnergy( particle, CaloSample:: PS );
  float e1 = sumEnergy( particle, CaloSample:: EM1 );
  float e2 = sumEnergy( particle, CaloSample:: EM2 );
  float e3 = sumEnergy( particle, CaloSample:: EM3 );

  float ehad1 = sumEnergy( particle, CaloSample::HAD1 ) + sumEnergy( particle, CaloSample::HAD1_Extended );
  float ehad2 = sumEnergy( particle, CaloSample::HAD2 ) + sumEnergy( particle, CaloSample::HAD2_Extended );
  float ehad3 = sumEnergy( particle, CaloSample::HAD3 ) + sumEnergy( particle, CaloSample::HAD3_Extended );

  float etot = e0+e1+e2+e3+ehad1+ehad2+ehad3;

  //fraction of energy deposited in 1st sampling
  float f0 = e0 / etot;
  float f1 = e1 / etot;
  float f2 = e2 / etot;
  float f3 = e3 / etot;
  float rhad = (ehad1+ehad2+ehad3) / etot;

  particle->setEtot( etot );
  particle->setReta( reta );
  particle->setRphi( rphi );
  particle->setEratio( eratio );
  particle->setEmaxs1( emaxs1 );
  particle->setE2tsts1( e2tsts1 );
  particle->setF1( f0 );
  particle->setF1( f1 );
  particle->setF2( f2 );
  particle->setF3( f3 );
  particle->setRhad( rhad );
  particle->setEt( particle->eta() != 0.0 ? particle->etot()/cosh(fabs(particle->eta())) : 0.0 ); 
  return StatusCode::SUCCESS;

}







float ShowerShapes::sumEnergy( xAOD::TruthParticle *particle, CaloSample sampling, unsigned eta_ncell, unsigned phi_ncell ) const
{
  float energy = 0.0;
  for ( const auto& cell : particle->allCells() )
  {
    if(cell->sampling() != sampling)  continue;
    // deta/dphi is the half of the cell sizei
    if( ( cell->eta() > ( particle->eta() - eta_ncell * cell->deltaEta() ) ) && 
        ( cell->eta() < ( particle->eta() + eta_ncell * cell->deltaEta() ) ) )
    {
      if( ( cell->phi() > ( particle->phi() - phi_ncell * cell->deltaPhi() ) ) && 
          ( cell->phi() < ( particle->phi() + phi_ncell * cell->deltaPhi() ) ) )
      {  
          energy+=cell->truthRawEnergy();
      }
    }
  }
  return energy;
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



StatusCode ShowerShapes::executeTool( xAOD::CaloCell * ) const {return StatusCode::SUCCESS;}



