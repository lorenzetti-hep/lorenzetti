

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
#include <cstdlib>


RunManager::RunManager( std::string name ): 
  IMsgService( name ),
  PropertyService()
{
  
#ifdef G4MULTITHREADED
  declareProperty( "NumberOfThreads", m_nThreads=1              );
#endif
  declareProperty( "OutputFile"     , m_output="Example.root"   );
  declareProperty( "RunVis"         , m_runVis=false            );

}

RunManager::~RunManager()
{;}


void RunManager::push_back( Gaugi::Algorithm *alg )
{
  m_acc.push_back( alg );
}


void RunManager::setGenerator( PrimaryGenerator *gen )
{
  m_generator = gen;
}


void RunManager::run( int evt )
{
  std::string basepath(std::getenv("LZT_PATH"));
  int argc=1;
  char* argv[1] = {"app"};
  G4UIExecutive* ui = 0;
  if ( m_runVis ) {
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
  MSG_INFO( m_output );
  ActionInitialization* actionInitialization = new ActionInitialization(m_generator, m_acc, m_output);
  runManager->SetUserInitialization(actionInitialization);

  MSG_INFO( "Creating the vis executive...");
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  std::stringstream runCommand; runCommand << "/run/beamOn " << evt ;

  if (!m_runVis ) {
    UImanager->ApplyCommand("/run/initialize");
    UImanager->ApplyCommand("/run/printProgress 1");
    UImanager->ApplyCommand("/run/verbose 2");
    UImanager->ApplyCommand(runCommand.str());
  } else  {
    MSG_INFO("Apply my vis macro");
    UImanager->ApplyCommand("/run/initialize");
    UImanager->ApplyCommand("/run/printProgress 1");
    UImanager->ApplyCommand("/run/verbose 2");
    UImanager->ApplyCommand("/control/execute "+ basepath+"/core/G4Kernel/data/vis.mac");
    UImanager->ApplyCommand(runCommand.str());
    ui->SessionStart();
    delete ui;
  
  }

  delete runManager;
  delete visManager;
}





