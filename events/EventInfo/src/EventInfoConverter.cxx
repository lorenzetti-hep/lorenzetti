
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


bool EventInfoConverter::serialize( std::string &key, SG::EventContext &ctx, TTree *tree ) const
{

  std::vector<xAOD::EventInfo_t> *events_t=nullptr;
  std::string branch_name = "xAOD__EventInfo";
  
  if ( tree->FindBranch(branch_name.c_str())){
    tree->SetBranchStatus((branch_name).c_str(), 1);
    tree->SetBranchAddress( (branch_name).c_str() , &events_t     );
    tree->GetBranch(branch_name.c_str() );
  }else{
    tree->Branch( (branch_name).c_str(), &events_t     );
  }

  SG::ReadHandle<xAOD::EventInfoContainer> container( key, ctx );

  if( !container.isValid() )
  {
    return false;
  }

  for (const auto event : **container.ptr() ){
    xAOD::EventInfo_t event_t;
    convert( event, event_t );
    events_t->push_back(event_t);
  }
  tree->Fill();
  return true;
}


bool EventInfoConverter::deserialize( std::string &key , int &evt, TTree* tree, SG::EventContext &ctx) const
{
  std::vector<xAOD::EventInfo_t> *events_t=nullptr;
  std::string branch_name = "xAOD__EventInfo";
  tree->SetBranchAddress(  branch_name.c_str() , &events_t );
  tree->GetEntry( evt );
  SG::WriteHandle<xAOD::EventInfoContainer> container(key, ctx);
  container.record( std::unique_ptr<xAOD::EventInfoContainer>(new xAOD::EventInfoContainer()));

  for( auto& event_t : *events_t)
  {
    xAOD::EventInfo  *event=nullptr;
    convert(event_t, event);
    container->push_back(event);
  }

  return true;
}
