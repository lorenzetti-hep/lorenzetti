
#include "G4Kernel/EventLoop.h"
#include "G4Kernel/RunAction.h"
#include "G4Kernel/Analysis.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>

RunAction::RunAction( std::vector<Gaugi::Algorithm*> acc )
 : G4UserRunAction(),
   m_acc(acc)
{;}


RunAction::~RunAction()
{ 
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


