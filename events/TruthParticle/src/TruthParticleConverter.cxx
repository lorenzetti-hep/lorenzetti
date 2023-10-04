
#include "TruthParticle/TruthParticleContainer.h"
#include "TruthParticle/TruthParticleConverter.h"


using namespace xAOD;



bool TruthParticleConverter::convert( const TruthParticle *truth, TruthParticle_t &truth_t ) const
{
  if(truth){
    truth_t.pdgid  = truth->pdgid();
    truth_t.seedid = truth->seedid();
    truth_t.e      = truth->e();
    truth_t.et     = truth->et();
    truth_t.eta    = truth->eta();
    truth_t.phi    = truth->phi();
    truth_t.px     = truth->px();
    truth_t.py     = truth->py();
    truth_t.pz     = truth->pz();
    truth_t.vx     = truth->vx();
    truth_t.vy     = truth->vy();
    truth_t.vz     = truth->vz();

    return true;
  }
  return false;
}


bool TruthParticleConverter::convert( const TruthParticle_t &truth_t , TruthParticle *&truth ) const
{
  truth = new xAOD::TruthParticle( 
                                   truth_t.pdgid,
                                   truth_t.seedid,
                                   truth_t.e,
                                   truth_t.et,
                                   truth_t.eta,
                                   truth_t.phi,
                                   truth_t.px,
                                   truth_t.py,
                                   truth_t.pz,
                                   truth_t.vx,
                                   truth_t.vy,
                                   truth_t.vz);                                  
  return true;
}


bool TruthParticleConverter::serialize( std::string &key, SG::EventContext &ctx, TTree *tree)
{
  m_particles_t.clear();
  m_key = "TruthParticleContainer_"+key;

  MSG_INFO( "Create and link all branches..." );
  tree->Branch( (m_key).c_str(), &m_particles_t     );

  MSG_INFO("Serialize TruthParticle...");
  SG::ReadHandle<xAOD::TruthParticleContainer> container( key, ctx );

  if( !container.isValid() )
  {
    MSG_ERROR("It's not possible to read the xAOD::TruthParticleContainer from this Context using this key " << key );
    return false;
  }

  for (const auto par : **container.ptr() ){
    xAOD::TruthParticle_t par_t;
    convert( par, par_t );
    m_particles_t.push_back(par_t);
  }
  return true;
}


bool TruthParticleConverter::deserialize( std::string &key , int &evt, TTree* tree, SG::EventContext &ctx)
{
  m_key = "TruthParticleContainer_"+key;
  std::vector<xAOD::TruthParticle_t> particles_t;
 
  MSG_DEBUG( "Link all branches..." );
  
  InitBranch( tree, (m_key).c_str() , &particles_t     );
  tree->GetEntry( evt );
  SG::WriteHandle<xAOD::TruthParticleContainer> container(key, ctx);
  container.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()));

  for( auto& par_t : particles_t)
  {
    xAOD::TruthParticle  *par=nullptr;
    convert(par_t, par);
    MSG_INFO( "Particle in eta = " << par->eta() << ", phi = " << par->phi() << ", pdgID = "<< par->pdgid());
    container->push_back(par);
  }

  return true;
}



template <class T>
bool TruthParticleConverter::InitBranch(TTree* fChain, std::string branch_name, T* param) const
{
  std::string bname = branch_name;
  if (fChain->GetAlias(bname.c_str()))
     bname = std::string(fChain->GetAlias(bname.c_str()));

  if (!fChain->FindBranch(bname.c_str()) ) {
    MSG_WARNING( "unknown branch " << bname );
    return false;
  }
  fChain->SetBranchStatus(bname.c_str(), 1.);
  fChain->SetBranchAddress(bname.c_str(), param);
  return true;
}
