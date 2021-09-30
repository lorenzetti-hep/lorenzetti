
#include "G4Kernel/RunReconstruction.h"
#include "G4Kernel/RunAction.h"
#include "G4Kernel/Analysis.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4Field.hh"
#include "G4ChordFinder.hh"

#include <iostream>

RunAction::RunAction( int numberOfThreads, int timeout, std::vector<Gaugi::Algorithm*> acc, std::string output )
 : IMsgService("RunAction"),
   G4UserRunAction(),
   m_acc(acc),
   m_output(output),
   m_numberOfThreads(numberOfThreads),
   m_timeout(timeout)
{;}


RunAction::~RunAction()
{
  MSG_INFO( "~RunAction()" );	
  delete G4AnalysisManager::Instance();  
}


G4Run* RunAction::GenerateRun()
{
  MSG_INFO("Creating the RunReconstruction...");
  return new RunReconstruction(m_numberOfThreads, m_timeout, m_acc, m_output);
}


void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  MSG_INFO( "RunAction::BeginOfRunAction" );
}


void RunAction::EndOfRunAction(const G4Run* /*run*/)
{;}


