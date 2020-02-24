
#include "core/geant/RunData.h"
#include "core/geant/RunAction.h"
#include "core/geant/Analysis.h"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction()
 : G4UserRunAction(),
{;}


RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}


G4Run* RunAction::GenerateRun()
{
  return new RunData();
}


void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{;}


void RunAction::EndOfRunAction(const G4Run* /*run*/)
{;}

