

#include "JF17.h"
#include <algorithm>
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "helper.h"


using namespace generator;


JF17::JF17(const std::string name , IGenerator *gen): 
  IMsgService(name),
  IAlgorithm(gen)
{
  declareProperty( "EtaMax"         , m_etaMax=1.4                );
  declareProperty( "EtaMin"         , m_etaMin=0                  );
  declareProperty( "MinPt"          , m_minPt=0.0                 );
  declareProperty( "Select"         , m_select=2                  );
  declareProperty( "EtaWindow"      , m_etaWindow=0.4             );
  declareProperty( "PhiWindow"      , m_phiWindow=0.4             );
}

JF17::~JF17()
{;}


StatusCode JF17::initialize()
{
  setMsgLevel(m_outputLevel);

  MSG_INFO( "Initializing the JF17..." );
  if(generator()->initialize().isFailure()){
    MSG_FATAL("Its not possible to initialize the generator. Abort!");
  }
  return StatusCode::SUCCESS;
}



StatusCode JF17::execute( generator::Event &ctx )
{

  HepMC3::GenEvent evt( HepMC3::Units::GEV, HepMC3::Units::MM);
  // Generate main event. Quit if too many failures.
  if (generator()->execute(evt).isFailure()){
    return StatusCode::FAILURE;
  }
  
 
  const auto main_event_t = sample_t();
  const auto main_event_z = sample_z();

 

  ParticleHelper::ParticleFilter filter( m_select, m_etaMax + .05, m_etaMin - 0.05, 0.7, 0.05 );
  filter.filter(evt);



  // Make one-to-one correspondence between fastjet PseudoJet and Particle
  std::vector<fastjet::PseudoJet> input_particles;
  std::map< std::size_t, const HepMC3::GenParticle* > local_barcode_map;
  std::size_t barcode(0);
  

  for ( const auto part : filter.getParticlesRef() ) {
    fastjet::PseudoJet substruct_cand(part->momentum().px(),part->momentum().py(),part->momentum().pz(),part->momentum().e());
    substruct_cand.set_user_index( barcode );
    input_particles.push_back( substruct_cand );
    local_barcode_map.insert( std::pair< std::size_t, const HepMC3::GenParticle* >( barcode++, part ) );
  }

  // Clusterize hadrons
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm,0.4);
  fastjet::ClusterSequence clust_seq(input_particles,jet_def);
  // Apply filters
  auto inclusive_jets = sorted_by_pt( clust_seq.inclusive_jets() );
  
  const float minPt=m_minPt/1.e3; 
  const float etaMax=m_etaMax;
  const float etaMin=m_etaMin;
  inclusive_jets.erase(std::remove_if(inclusive_jets.begin(),
                                      inclusive_jets.end(),
                                      [minPt,etaMax, etaMin](fastjet::PseudoJet& j){return (std::abs(j.eta()) > etaMax) || (std::abs(j.eta()) < etaMin) || (j.pt() < minPt);}
                                     ), inclusive_jets.end());

  if ( inclusive_jets.empty() ){
    MSG_WARNING("inclusive_jets.empty()");
    throw NotInterestingEvent();
  }

  for ( const auto j : inclusive_jets ){

    const auto& substructs = j.constituents();
    // Find the hottest particle inside of the Jet Cluster
    const HepMC3::GenParticle* main_p=nullptr;
    {
      float pt=0.0;
      for ( auto& substruct : substructs )
      {
        auto p = local_barcode_map[ substruct.user_index() ];
        if( p->momentum().pt() > pt ){ main_p=p; pt=p->momentum().pt(); };
      }
    }

    // Find all particles inside of the window with center in the hot particle
    float etot=0.0;
    std::vector<const HepMC3::GenParticle*> cluster;
    for ( auto& substruct : substructs )
    {
      auto p = local_barcode_map[ substruct.user_index() ];
      if( p->momentum().eta() > main_p->momentum().eta()- m_etaWindow/2 && 
          p->momentum().eta() <= main_p->momentum().eta()+m_etaWindow/2 )
        {
        if( p->momentum().phi() > main_p->momentum().phi()-m_phiWindow/2 && 
            p->momentum().phi() <= main_p->momentum().phi()+m_phiWindow/2 )
          {
          etot+=ParticleHelper::et(p);  
          cluster.push_back( p );
        }
      }
    }

    
    // If the total cluster energy is higher than the cut, than we 
    // can include these particles to the jet cluster vector
    if (etot > minPt){


      auto seed = generator::Seed( main_p->momentum().eta(), main_p->momentum().phi() );

      for ( auto& part : cluster ){
  
        const auto  vtx         = part->production_vertex();
        
        //-----------
        seed.emplace_back( 1, 0, 
                           part->pid(), 
                           part->momentum().px(), 
                           part->momentum().py(), 
                           part->momentum().pz(), 
                           part->momentum().eta(), 
                           part->momentum().phi(), 
                           vtx->position().px(), 
                           vtx->position().py(), 
                           vtx->position().pz() + main_event_z, 
                           vtx->position().t()  + main_event_t, 
                           part->momentum().e(), 
                           part->momentum().pt() ); 
      }

      ctx.push_back( seed );
    }
    
  }

  ctx.setEventNumber(evt.event_number());
  
  // Print all particles and clusters
  int cx=0;
  for ( auto seed : *ctx ){
    
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
  //m_generator.stat();
  return StatusCode::SUCCESS;
}







