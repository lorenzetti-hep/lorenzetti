
#include "JF17.h"
#include <algorithm>
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
#include "Pythia8Plugins/HepMC2.h"
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh"
#include "ParticleFilter.h"

using namespace Pythia8;

JF17::JF17(): 
  IMsgService("JF17"), 
  EventGenerator()
{
  declareProperty( "EtaWindow", m_etaWindow=0.4 );
  declareProperty( "PhiWindow", m_phiWindow=0.4 );
}



StatusCode JF17::generate( Event &event, std::vector<Pythia8::Particle*> &main_particles, std::vector<std::vector<Pythia8::Particle*>>& particles)
{

  ParticleFilter filter( m_select, m_etaMax + .05, 0.7, 0.05 );
  filter.filter(event);
  
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
  const float minPt=m_minPt; const float etaMax=m_etaMax;
  inclusive_jets.erase(std::remove_if(inclusive_jets.begin(),
                                      inclusive_jets.end(),
                                      [minPt,etaMax](fastjet::PseudoJet& j){return (std::abs(j.eta()) > etaMax) || (j.pt() < minPt);}
                                     ), inclusive_jets.end());

  if ( inclusive_jets.empty() ){
    throw NotInterestingEvent();
  }

  // Map back from the PseudoJet to particles
  particles.clear();

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
    if (etot > m_minPt){
      main_particles.push_back( main_p );
      particles.push_back( cluster );
    }
  }


  // Print all particles and clusters
  int cx=0;
  for ( auto pi : main_particles ){
    auto pj_vec = particles.at(cx);
    MSG_INFO( "======== Cluster " << cx << " ==========" );

    float etot=0.0;
    for ( auto pj : pj_vec ){
      etot+= pj->pT();
      MSG_INFO( "Eta =" << pj->eta() << " Phi = " << pj->phi() << " Pt = " << pj->pT() );
    }
    MSG_INFO( "Eta_center =" << pi->eta() << " Phi_center = " << pi->phi() << " Pt = " << etot );
    MSG_INFO( "========================================" );
    cx++;
  }


  return StatusCode::SUCCESS;
}






