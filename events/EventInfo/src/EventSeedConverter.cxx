
#include "EventInfo/EventSeedConverter.h"


using namespace xAOD;


bool EventSeedConverter::convert( const EventSeed *seed, EventSeed_t &seed_t ) const
{
  if(seed){
    seed_t.id = seed->id();
    seed_t.e  = seed->e();
    seed_t.et = seed->et();
    seed_t.eta = seed->eta();
    seed_t.phi = seed->phi();
    return true;
  }
  return false;
}


bool EventSeedConverter::convert( const EventSeed_t &seed_t , EventSeed *&seed ) const
{
  seed = new xAOD::EventSeed( seed_t.id,
                              seed_t.e,
                              seed_t.et,
                              seed_t.eta,
                              seed_t.phi
                             );
  return true;
}


bool EventSeedConverter::serialize( std::string &key, SG::EventContext &ctx, TTree *tree) const
{  
  std::string branch_name = "xAOD__EventSeed";
  std::vector<xAOD::EventSeed_t>  *seeds_t=nullptr;

  if ( tree->FindBranch(branch_name.c_str())){
    tree->SetBranchStatus((branch_name).c_str(), 1);
    tree->SetBranchAddress( (branch_name).c_str() , &seeds_t     );
    tree->GetBranch(branch_name.c_str() );
  }else{
    tree->Branch( (branch_name).c_str(), &seeds_t     );
  }

  SG::ReadHandle<xAOD::EventSeedContainer> container( key, ctx );

  if( !container.isValid() )
  {
    return false;
  }

  for (const auto seed : **container.ptr() ){
    xAOD::EventSeed_t seed_t;
    convert( seed, seed_t );
    seeds_t->push_back(seed_t);
  }
  tree->Fill();
  return true;
}


bool EventSeedConverter::deserialize( std::string &key , int &evt, TTree* tree, SG::EventContext &ctx) const
{
  std::vector<xAOD::EventSeed_t> *seeds_t=nullptr;
  std::string branch_name = "xAOD__EventSeed";
  tree->SetBranchAddress( branch_name.c_str() , &seeds_t );
  tree->GetEntry( evt );
  SG::WriteHandle<xAOD::EventSeedContainer> container(key, ctx);
  container.record( std::unique_ptr<xAOD::EventSeedContainer>(new xAOD::EventSeedContainer()));

  for( auto& seed_t : *seeds_t)
  {
    xAOD::EventSeed  *seed=nullptr;
    convert(seed_t, seed);
    container->push_back(seed);
  }
  return true;
}




