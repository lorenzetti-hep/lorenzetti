

#include "G4Kernel/RunManager.h"
#include "G4Kernel/DetectorConstruction.h"
#include "G4Kernel/ActionInitialization.h"
#include "GaugiKernel/Algorithm.h"



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


RunManager::RunManager( std::string name ): 
  IMsgService( name ),
  PropertyService()
{
  
#ifdef G4MULTITHREADED
  declareProperty( "NumberOfThreads", m_nThreads=1              );
#endif
  declareProperty( "Output"         , m_output="Example.root"   );
  declareProperty( "VisMacro"       , m_visMacro="vis.mac"      );

}

RunManager::~RunManager()
{;}




void RunManager::push_back( Gaugi::Algorithm *alg )
{
  m_acc.push_back( alg );
}



void RunManager::run( std::string macro )
{
  int argc=1;
  char* argv[1] = {"app"};
  G4UIExecutive* ui = 0;
  if ( macro=="" ) {
    ui = new G4UIExecutive(argc,argv);
  }

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

  MSG_INFO( "Building the detector..." );
  DetectorConstruction* detConstruction = new DetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(physicsList);

  MSG_INFO( "Creating the action initalizer..." );
  ActionInitialization* actionInitialization = new ActionInitialization(m_acc, m_output);
  runManager->SetUserInitialization(actionInitialization);

  MSG_INFO( "Creating the vis executive...");
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();


  if ( macro != "" ) {
    G4String command = "/control/execute ";
    MSG_INFO("Apply my macro");
    UImanager->ApplyCommand(command+macro);
  } else  {
    MSG_INFO("Apply my vis macro");
    UImanager->ApplyCommand("/control/execute "+m_visMacro);
    ui->SessionStart();
    delete ui;
  
  }

  delete runManager;
  delete visManager;
}








