
#include "Pileup.h"
#include "G4Kernel/CaloPhiRange.h"
#include "GenKernel/helper.h"


using namespace SG;
using namespace generator;


Pileup::Pileup(const std::string name, IGenerator *gen): 
  IMsgService(name), 
  IAlgorithm(gen)
{
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

  if(generator()->initialize().isFailure()){
    MSG_FATAL("Its not possible to initialize the generator. Abort!");
  }

  return StatusCode::SUCCESS;
}


StatusCode Pileup::execute(  generator::Event &ctx )
{

  ParticleFilter det_acc_filter( m_select, m_etaMax + .05, 0.7, 0.05 );
  const int nWin = m_bc_id_end - m_bc_id_start + 1;
  double nPileUpMean(0);
  
  MSG_INFO("Filling minbias event into the context... ");
  for ( int bc_id = m_bc_id_start; bc_id <= m_bc_id_end; ++bc_id )
  {
    // Select the number of pileup events to generate.
    int nPileup = poisson(m_nPileupAvg);
    nPileUpMean += nPileup;

    // Generate a number of pileup events.
    for (int iPileup = 0; iPileup < nPileup; ++iPileup) 
    {
      HepMC3::GenEvent evt( HepMC3::Units::GEV, HepMC3::Units::MM);

      if (generator()->execute(evt).isFailure()){
        --iPileup; 
        MSG_WARNING("Generator return status failure. Rerun event...");
        continue;
      }

      const auto mb_event_t = sample_t();
      const auto mb_event_z = sample_z();

      // Find final charged particles with |eta| < etaMax
      det_acc_filter.filter( evt );

      for ( const auto part : det_acc_filter.getParticlesRef() ){

        for ( auto &seed : *ctx ) {

          const double deta = abs( part->momentum().eta() - seed.eta() );
          const double dphi = abs( CaloPhiRange::diff( part->momentum().phi(), seed.phi() ) );
          
          if ( ( deta < m_delta_eta ) && ( dphi < m_delta_phi ) ) 
          {
            const auto vtx = part->production_vertex();
            
            // Pileup is always !main event
            seed.emplace_back( 0, bc_id, 
                       part->pid(), 
                       part->momentum().px(), 
                       part->momentum().py(), 
                       part->momentum().pz(), 
                       part->momentum().eta(), 
                       part->momentum().phi(), 
                       vtx->position().px(), 
                       vtx->position().py(), 
                       vtx->position().pz() + mb_event_z, 
                       vtx->position().t()  + mb_event_t, 
                       part->momentum().e(), 
                       part->momentum().pt() ); 

          }// Is inside of the minimum bias windows?
        }// Loop over all clusters
      }// Loop over all generated minimum bias particles
    } // Created all pile_up events for this BC
  } // Finished all BCs


  // Fill window specific information
  nPileUpMean /= nWin; 
  ctx.setAvgmu( nPileUpMean );
  
  return StatusCode::SUCCESS;
}


StatusCode Pileup::finalize()
{
  MSG_INFO( "Finalize the Pileup Event." );
  if(generator()->finalize().isFailure()){
    MSG_FATAL("Its not possible to finalize the generator. Abort!");
  }
  return StatusCode::SUCCESS;
}

