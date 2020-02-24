
#include "core/EventContext.h"



EventContext::EventContext():
  m_step(nullptr),
  m_caloClusterCont(nullptr),
  m_collection(nullptr)
{;}

EventContext::~EventContext()
{;}

void EventContext::attach( const G4Step *step )
{
  m_step = step;
}


void EventContext::attach( xAOD::CaloClusterContainer *cont )
{
  m_caloClusterCont = cont;
}


void EventContext::attach( xAOD::CaloCellCollection *collection )
{
  m_collection = collection
}



void EventContext::retrieve( G4Step *step )
{
  step = m_step;
}


void EventContext::retrieve( xAOD::CaloClusterContainer *cont)
{
  cont = m_caloClusterCont;
}



void EventContext::retrieve( xAOD::CaloCellCollection *collection )
{
  collection = m_collection;
}




