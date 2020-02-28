
#include "core/EventContext.h"

EventContext::EventContext():
  m_step(nullptr),
  m_caloClusterCont(nullptr),
  m_collection(nullptr),
  m_eventInfo(nullptr)
{}


EventContext::~EventContext()
{}

void EventContext::attach( const G4Step *step )
{
  m_step = step;
}


void EventContext::attach( const xAOD::CaloClusterContainer *cont , std::string key )
{
  m_caloCluster[key] = cont;
}







void EventContext::attach( xAOD::CaloCellCollection *collection )
{
  m_collection = collection;
}


void EventContext::attach( xAOD::EventInfo *evt )
{
  m_eventInfo = evt;
}



void EventContext::retrieve( const G4Step *&step )
{
  step = m_step;
}


void EventContext::retrieve( xAOD::CaloClusterContainer *&cont)
{
  cont = m_caloClusterCont;
}



void EventContext::retrieve( xAOD::CaloCellCollection *&collection )
{
  collection = m_collection;
}



void EventContext::retrieve( xAOD::EventInfo *&evt )
{
  evt = m_eventInfo;
}




