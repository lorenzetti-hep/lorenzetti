
#include "PythiaGun.h"
#include "Pythia8Plugins/HepMC3.h"
#include "HepMC3/WriterRootTree.h"


using namespace Pythia8;
using namespace generator;

PythiaGun::PythiaGun(): 
  IGenerator("PythiaGun")
{
  declareProperty( "File"       , m_file=""       );
  declareProperty( "Seed"       , m_seed=0        );
  declareProperty( "EventNumber", m_eventNumber=0 );
}


StatusCode PythiaGun::initialize()
{
  MSG_INFO("Initialize Pythia8 generator with seed " << m_seed << "...");
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed;
  // Read in commands from external file.
  m_gun.readFile( m_file );
  m_gun.readString("Random:setSeed = on");
  m_gun.readString(cmdseed.str());
  m_nAbort = m_gun.mode("Main:timesAllowErrors");
  // Initialization for main (LHC) event
  m_gun.init();
  m_nEvent = 0;
  return StatusCode::SUCCESS;
}


StatusCode PythiaGun::execute(HepMC3::GenEvent &evt){


  if (!m_gun.next()) {
    if (m_iAbort++>m_nAbort){
      MSG_ERROR("Event generation aborted prematurely, owing to error in main event!" );
      throw AbortPrematurely();
    }
  }

  double weight = m_gun.info.mergingWeight();
  double evtweight = m_gun.info.weight();
  weight *= evtweight;

  // Do not print zero-weight events.
  if ( weight == 0. ) {
    return StatusCode::FAILURE;
  }

  m_nEvent+=1;

  HepMC3::Pythia8ToHepMC3 converter;
  converter.fill_next_event(m_gun.event, &evt, -1, &m_gun.info);

  evt.set_event_number(m_eventNumber + m_nEvent);

  return StatusCode::SUCCESS;
}



StatusCode PythiaGun::finalize()
{
  MSG_INFO( "Finalize the PythiaGun Event." );
  m_gun.stat();
  return StatusCode::SUCCESS;
}



float PythiaGun::random_flat(){
  return m_gun.rndm.flat();
}

float PythiaGun::random_gauss(){
  return m_gun.rndm.gauss();
}