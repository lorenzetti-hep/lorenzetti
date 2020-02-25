

#include "core/RunCastor.h"
#include "core/geant/DetectorConstruction.h"
#include "core/geant/ActionInitialization.h"
#include "core/IAlgTool.h"



#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"
#include "Randomize.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include <iostream>
#include "time.h"


RunCastor::RunCastor( std::string output , int nThreads ): 
  m_output(output),
  m_nThreads(nThreads)
{;}

RunCastor::~RunCastor()
{;}


void RunCastor::vis( std::string vis )
{
  m_visMacro=vis;
}


void RunCastor::push_back( IAlgTool *alg )
{
  m_sequence.push_back( alg );
}



void RunCastor::run( std::string macro )
{

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  //G4long seed = abs(((time(NULL) * 181) * ((getpid() - 83) * 359)) % 104729);
  G4long seed = abs(((time(NULL) * 181) * ((83) * 359)) % 104729);
  CLHEP::HepRandom::setTheSeed(seed);

  // Construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  if ( m_nThreads > 0 ) {
    runManager->SetNumberOfThreads(m_nThreads);
  }
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  DetectorConstruction* detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(physicsList);

  ActionInitialization* actionInitialization = new ActionInitialization(m_sequence, m_output);
  runManager->SetUserInitialization(actionInitialization);

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();


  if ( macro != "" ) {
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  } else  {
    UImanager->ApplyCommand("/control/execute "+m_visMacro);
  }

  delete runManager;
  delete visManager;
}








