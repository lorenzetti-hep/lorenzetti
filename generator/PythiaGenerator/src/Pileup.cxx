
#include "Pileup.h"
#include "G4Kernel/CaloPhiRange.h"
#include "P8Kernel/ParticleFilter.h"


using namespace Pythia8;
using namespace SG;
using namespace generator;


Pileup::Pileup(): IMsgService("Pileup"), Algorithm()
{
  declareProperty( "File"           , m_file=""          );
  declareProperty( "PileupAvg"      , m_nPileupAvg=0     );
  declareProperty( "BunchIdStart"   , m_bc_id_start=-8   );
  declareProperty( "BunchIdEnd"     , m_bc_id_end=7      );
  declareProperty( "EtaMax"         , m_etaMax=1.4       );
  declareProperty( "Select"         , m_select=2         );
  declareProperty( "DeltaEta"       , m_delta_eta=0.22   );
  declareProperty( "DeltaPhi"       , m_delta_phi=0.22   );
}



Pileup::~Pileup()
{}



StatusCode Pileup::initialize()
{
  setMsgLevel(m_outputLevel);

  MSG_INFO( "Initialize pythia..." );
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed; 
  m_generator.readFile( m_file );
  m_generator.readString("Random:setSeed = on");
  m_generator.readString(cmdseed.str());
  m_generator.init();
 
  return StatusCode::SUCCESS;
}



StatusCode Pileup::finalize()
{
  MSG_INFO( "Finalize the Event generator." );
  m_generator.stat();
  return StatusCode::SUCCESS;
}



StatusCode Pileup::execute(  generator::Event &event )
{

  ParticleFilter det_acc_filter( m_select, m_etaMax + .05, 0.7, 0.05 );
  const int nWin = m_bc_id_end - m_bc_id_start + 1;
  int iAbort=0;

  double nPileUpMean(0);
  
  for ( int bc_id = m_bc_id_start; bc_id <= m_bc_id_end; ++bc_id ){

    // Select the number of pileup events to generate.
    int nPileup = poisson(m_nPileupAvg);
    nPileUpMean += nPileup;


    // Generate a number of pileup events.
    for (int iPileup = 0; iPileup < nPileup; ++iPileup) 
    {

      if (!m_generator.next()) {
        if (++iAbort < m_nAbort) continue;
        MSG_WARNING("Event generation aborted prematurely, owing to error in minbias generation!");
        break;
      }

      const auto mb_event_t = sample_t();
      const auto mb_event_z = sample_z();

      double weight = m_generator.info.mergingWeight();
      double evtweight = m_generator.info.weight();
      weight *= evtweight;

      // Do not print zero-weight events.
      if ( weight == 0. ) {
        --iPileup; continue;
      }

      // Find final charged particles with |eta| < etaMax
      det_acc_filter.filter( m_generator.event );
      

      for ( const auto p : det_acc_filter.getParticlesRef() ){

        for ( auto &seed : *event ) {

          const double deta = abs( p->eta() - seed.eta() );
          const double dphi = abs( CaloPhiRange::diff( p->phi(), seed.phi() ) );
          
          if ( ( deta < m_delta_eta ) && ( dphi < m_delta_phi ) ) 
          {
            // Pileup is always !main event
            seed.emplace_back( 0, bc_id, p->id(), p->px(), p->py(), p->pz(), p->eta(), p->phi(), 
                               p->xProd(), p->yProd(), p->zProd()+mb_event_z, p->tProd()+mb_event_t, 
                               p->e(), p->eT() ); 

          }// Is inside of the minimum bias windows?

        }// Loop over all clusters
      }// Loop over all generated minimum bias particles

    } // Created all pile_up events for this BC
    //if (bc_id != 0){
    //  event[0].emplace_back( 0, bc_id, 211, 10, 0, 0, 0, 0, 
    //                     0, 0, 0, 0, 
    //                     10, 10); 
    //}
  } // Finished all BCs

  // Fill window specific information
  nPileUpMean /= nWin; 
  event.setAvgmu( nPileUpMean );
  
  return StatusCode::SUCCESS;
}


