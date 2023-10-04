
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


bool EventSeedConverter::serialize( std::string &key, SG::EventContext &ctx, TTree *tree)
{
  m_seeds_t.clear();

  MSG_INFO( "Create and link all branches..." );
  tree->Branch( ("EventSeedConverter_"+key).c_str(), &m_seeds_t      );

  MSG_INFO("Serialize EventSeed...");
  SG::ReadHandle<xAOD::EventSeedContainer> container( key, ctx );

  if( !container.isValid() )
  {
    MSG_ERROR("It's not possible to read the xAOD::EventSeedContainer from this Context using this key " << key );
    return false;
  }

  for (const auto seed : **container.ptr() ){
    xAOD::EventSeed_t seed_t;
    convert( seed, seed_t );
    m_seeds_t.push_back(seed_t);
  }
  return true;
}


bool EventSeedConverter::deserialize( std::string &key , int &evt, TTree* tree, SG::EventContext &ctx)
{
  std::vector<xAOD::EventSeed_t> seeds_t;
 
  MSG_DEBUG( "Link all branches..." );
  InitBranch( tree, (key).c_str() , &seeds_t     );
  tree->GetEntry( evt );
  SG::WriteHandle<xAOD::EventSeedContainer> container(key, ctx);
  container.record( std::unique_ptr<xAOD::EventSeedContainer>(new xAOD::EventSeedContainer()));

  for( auto& seed_t : seeds_t)
  {
    xAOD::EventSeed  *seed=nullptr;
    convert(seed_t, seed);
    container->push_back(seed);
  }

  return true;
}


template <class T>
bool EventSeedConverter::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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

