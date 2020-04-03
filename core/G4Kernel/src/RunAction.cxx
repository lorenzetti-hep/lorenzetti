
#include "G4Kernel/EventLoop.h"
#include "G4Kernel/RunAction.h"
#include "G4Kernel/Analysis.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"



RunAction::RunAction( std::vector<Gaugi::Algorithm*> acc, std::string output )
 : G4UserRunAction(),
   m_acc(acc)
{

  m_store = new SG::StoreGate( output );

  for ( auto toolHandle : m_acc )
  { 
    toolHandle->setStoreGateSvc( m_store );
    if ( toolHandle->initialize().isFailure() )
    {
      // raise exception here
    }
  } 

}


RunAction::~RunAction()
{

  for ( auto toolHandle : m_acc )
  { 
    if ( toolHandle->finalize().isFailure() )
    {
      // raise exception here
    }
  } 

  delete m_store;
  delete G4AnalysisManager::Instance();  
}


G4Run* RunAction::GenerateRun()
{
  return new EventLoop(m_acc);
}


void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{;}


void RunAction::EndOfRunAction(const G4Run* /*run*/)
{;}


