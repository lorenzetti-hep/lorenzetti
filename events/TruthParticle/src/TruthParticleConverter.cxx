
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


bool TruthParticleConverter::serialize( std::string &key, SG::EventContext &ctx, TTree *tree) const
{
  std::vector<xAOD::TruthParticle_t>  *particles_t=nullptr;
  std::string branch_name = "xAOD__TruthParticle";
  
  if ( tree->FindBranch(branch_name.c_str())){
    tree->SetBranchAddress( (branch_name).c_str() , &particles_t     );
    tree->GetBranch(branch_name.c_str() );
  }else{
    tree->Branch( (branch_name).c_str(), &particles_t     );
  }

  SG::ReadHandle<xAOD::TruthParticleContainer> container( key, ctx );

  if( !container.isValid() )
  {
    return false;
  }

  for (const auto par : **container.ptr() ){
    xAOD::TruthParticle_t par_t;
    convert( par, par_t );
    particles_t->push_back(par_t);
  }
  tree->Fill();
  return true;
}


bool TruthParticleConverter::deserialize( std::string &key , int &evt, TTree* tree, SG::EventContext &ctx) const
{
  std::vector<xAOD::TruthParticle_t> *particles_t=nullptr;
  std::string branch_name = "xAOD__TruthParticle";
  tree->SetBranchAddress( branch_name.c_str() , &particles_t     );
  tree->GetEntry( evt );
  SG::WriteHandle<xAOD::TruthParticleContainer> container(key, ctx);
  container.record( std::unique_ptr<xAOD::TruthParticleContainer>(new xAOD::TruthParticleContainer()));

  for( auto& par_t : *particles_t)
  {
    xAOD::TruthParticle  *par=nullptr;
    convert(par_t, par);
    container->push_back(par);
  }

  return true;
}



