

#include "JF17.h"
#include <algorithm>
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "P8Kernel/ParticleFilter.h"
#include "P8Kernel/Event.h"


using namespace Pythia8;
using namespace generator;


JF17::JF17(): 
  IMsgService("JF17"), 
  Algorithm()
{
  declareProperty( "File"           , m_file=""                   );
  declareProperty( "EtaMax"         , m_etaMax=1.4                );
  declareProperty( "MinPt"          , m_minPt=0.0                 );
  declareProperty( "Select"         , m_select=2                  );
  declareProperty( "EtaWindow"      , m_etaWindow=0.4             );
  declareProperty( "PhiWindow"      , m_phiWindow=0.4             );


}

JF17::~JF17()
{;}


StatusCode JF17::initialize()
{
  MSG_INFO( "Initializing the JF17..." );
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed;
  // Read in commands from external file.
  m_generator.readFile( m_file );
  m_generator.readString("Random:setSeed = on");
  m_generator.readString(cmdseed.str());
  m_nAbort = m_generator.mode("Main:timesAllowErrors");
  // Initialization for main (LHC) event
  m_generator.init();

  return StatusCode::SUCCESS;
}



StatusCode JF17::execute( generator::Event &event )
{

  // Generate main event. Quit if too many failures.
  if (!m_generator.next()) {
    if (m_iAbort++>m_nAbort){
      MSG_ERROR("Event generation aborted prematurely, owing to error in main event!" );
      throw AbortPrematurely();
    }
  }
 
  const auto main_event_t = sample_t();
  const auto main_event_z = sample_z();

  double weight = m_generator.info.mergingWeight();
  double evtweight = m_generator.info.weight();
  weight *= evtweight;

  // Do not print zero-weight events.
  if ( weight == 0. ) {
    MSG_WARNING("Pythia generation return weight zero.");
    return StatusCode::FAILURE;
  }

  ParticleFilter filter( m_select, m_etaMax + .05, 0.7, 0.05 );
  filter.filter(m_generator.event);



  // Make one-to-one correspondence between fastjet PseudoJet and Particle
  std::vector<fastjet::PseudoJet> input_particles;
  std::map< std::size_t, Pythia8::Particle* > local_barcode_map;
  std::size_t barcode(0);
  

  for ( const auto part : filter.getParticlesRef() ) {
    fastjet::PseudoJet substruct_cand(part->px(),part->py(),part->pz(),part->e());
    substruct_cand.set_user_index( barcode );
    input_particles.push_back( substruct_cand );
    local_barcode_map.insert( std::pair< std::size_t, Pythia8::Particle* >( barcode++, part ) );
  }

  // Clusterize hadrons
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm,0.4);
  fastjet::ClusterSequence clust_seq(input_particles,jet_def);
  // Apply filters
  auto inclusive_jets = sorted_by_pt( clust_seq.inclusive_jets() );
  
  const float minPt=m_minPt/1.e3; const float etaMax=m_etaMax;
  inclusive_jets.erase(std::remove_if(inclusive_jets.begin(),
                                      inclusive_jets.end(),
                                      [minPt,etaMax](fastjet::PseudoJet& j){return (std::abs(j.eta()) > etaMax) || (j.pt() < minPt);}
                                     ), inclusive_jets.end());

  if ( inclusive_jets.empty() ){
    MSG_WARNING("inclusive_jets.empty()");
    throw NotInterestingEvent();
  }

  for ( const auto j : inclusive_jets ){

    const auto& substructs = j.constituents();
    // Find the hottest particle inside of the Jet Cluster
    Particle* main_p=nullptr;
    {
      float pt=0.0;
      for ( auto& substruct : substructs )
      {
        auto p = local_barcode_map[ substruct.user_index() ];
        if( p->pT() > pt ){ main_p=p; pt=p->pT(); };
      }
    }

    // Find all particles inside of the window with center in the hot particle
    float etot=0.0;
    std::vector<Pythia8::Particle*> cluster;
    for ( auto& substruct : substructs )
    {
      auto p = local_barcode_map[ substruct.user_index() ];
      if( p->eta() > main_p->eta()- m_etaWindow/2 && p->eta() <= main_p->eta()+m_etaWindow/2 ){
        if( p->phi() > main_p->phi()-m_phiWindow/2 && p->phi() <= main_p->phi()+m_phiWindow/2 ){
          etot+=p->eT();  
          cluster.push_back( p );
        }
      }
    }
    // If the total cluster energy is higher than the cut, than we 
    // can include these particles to the jet cluster vector
    if (etot > minPt){

      auto seed = generator::Seed( main_p->eta(), main_p->phi() );

      for ( auto& p : cluster ){
        
        seed.emplace_back( 1, 0, p->id(), p->px(), p->py(), p->pz(), p->eta(), p->phi(), 
                           p->xProd(), p->yProd(), p->zProd()+main_event_z, p->tProd()+main_event_t, 
                           p->e(), p->eT() ); 



      }

      event.push_back( seed );
    }
  }

  // Print all particles and clusters
  int cx=0;
  for ( auto seed : *event ){
    
    MSG_INFO( "======== Cluster " << cx << " ==========" );
    float etot=0.0;
    for ( auto p : *seed ){
      etot+= p.eT;
      MSG_INFO( "Eta =" << p.eta << " Phi = " << p.phi << " Pt = " << p.eT );
    }

    MSG_INFO( "Eta_center =" << seed.eta() << " Phi_center = " << seed.phi() << " Et = " << etot );
    MSG_INFO( "========================================" );
    cx++;
  }

  return StatusCode::SUCCESS;
}


StatusCode JF17::finalize()
{
  MSG_INFO( "Finalize the JF17 Event." );
  m_generator.stat();
  return StatusCode::SUCCESS;
}







