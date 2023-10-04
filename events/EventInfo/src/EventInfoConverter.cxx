
#include "EventInfo/EventInfoConverter.h"

using namespace xAOD;


bool EventInfoConverter::convert( const EventInfo *event, EventInfo_t &event_t) const
{
  if(event){
    event_t.runNumber   = event->runNumber();
    event_t.eventNumber = event->eventNumber();
    event_t.avgmu       = event->avgmu();
    return true;
  }
  return false;
}

bool EventInfoConverter::convert(const EventInfo_t &event_t, EventInfo *&event ) const
{
  event = new xAOD::EventInfo();
  event->setRunNumber( event_t.runNumber );
  event->setEventNumber( event_t.eventNumber );
  event->setAvgmu( event_t.avgmu);
  return true;
}


bool EventInfoConverter::serialize( std::string &key, SG::EventContext &ctx, TTree *tree)
{
  m_events_t.clear();

  MSG_INFO( "Create and link all branches..." );
  tree->Branch( ("EventInfoContainer_"+key).c_str(), &m_events_t      );

  MSG_INFO("Serialize EventInfo...");
  SG::ReadHandle<xAOD::EventInfoContainer> container( key, ctx );

  if( !container.isValid() )
  {
    MSG_ERROR("It's not possible to read the xAOD::EventInfoContainer from this Context using this key " << key );
    return false;
  }

  for (const auto event : **container.ptr() ){
    xAOD::EventInfo_t event_t;
    convert( event, event_t );
    m_events_t.push_back(event_t);
  }
  return true;
}


bool EventInfoConverter::deserialize( std::string &key , int &evt, TTree* tree, SG::EventContext &ctx)
{
  std::vector<xAOD::EventInfo_t> events_t;
 
  MSG_DEBUG( "Link all branches..." );
  
  InitBranch( tree, (key).c_str() , &events_t     );
  tree->GetEntry( evt );
  SG::WriteHandle<xAOD::EventInfoContainer> container(key, ctx);
  container.record( std::unique_ptr<xAOD::EventInfoContainer>(new xAOD::EventInfoContainer()));

  for( auto& event_t : events_t)
  {
    xAOD::EventInfo  *event=nullptr;
    convert(event_t, event);
    container->push_back(event);
  }

  return true;
}


template <class T>
bool EventInfoConverter::InitBranch(TTree* fChain, std::string branch_name, T* param) const
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


