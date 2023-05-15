
#include "Pythia8Gen.h"
#include "Pythia8Plugins/HepMC3.h"
#include "HepMC3/WriterRootTree.h"
#include "TMath.h"


using namespace Pythia8;
using namespace generator;

Pythia8Gen::Pythia8Gen(): 
  IMsgService("Pythia8"),
  IGenerator()
  
{
  declareProperty( "File"       , m_file=""       );
  declareProperty( "Seed"       , m_seed=0        );
  declareProperty( "EventNumber", m_eventNumber=0 );
}


StatusCode Pythia8Gen::initialize()
{
  MSG_INFO("Initialize Pythia8 generator with seed " << m_seed << "...");
  std::stringstream cmdseed; cmdseed << "Random:seed = " << m_seed;
  // Read in commands from external file.

  if (m_file==""){
    // Key requirement: switch off ProcessLevel, and thereby also PartonLevel.
    // Switch off automatic event listing in favour of manual.
    m_gun.readString("ProcessLevel:all = off");
    m_gun.readString("Next:numberShowInfo = 1");
    m_gun.readString("Next:numberShowProcess = 1");
    m_gun.readString("Next:numberShowEvent = 1");
  }else{
    MSG_INFO("Reading configuration from: " << m_file);
    m_gun.readFile( m_file );
  }

  m_gun.readString("Random:setSeed = on");
  m_gun.readString(cmdseed.str());
  
  m_nAbort = m_gun.mode("Main:timesAllowErrors");
  // Initialization for main (LHC) event
  m_gun.init();
  m_nEvent = 0;
  return StatusCode::SUCCESS;
}


StatusCode Pythia8Gen::execute(HepMC3::GenEvent &evt){

  MSG_INFO("Pythia8::next()...");
  if (!m_gun.next()) {
    if (m_iAbort++>m_nAbort){
      MSG_ERROR("Event generation aborted prematurely, owing to error in main event!" );
      throw AbortPrematurely();
    }
  }

  float weight = m_gun.info.mergingWeight();
  float evtweight = m_gun.info.weight();
  weight *= evtweight;

  // Do not print zero-weight events.
  if ( weight == 0. ) {
    return StatusCode::FAILURE;
  }

  m_nEvent+=1;

  HepMC3::Pythia8ToHepMC3 converter;


  MSG_DEBUG("Pythia8 to HepMC3");
  converter.fill_next_event(m_gun.event, &evt);

  evt.set_event_number(m_eventNumber + m_nEvent);

  return StatusCode::SUCCESS;
}



StatusCode Pythia8Gen::finalize()
{
  MSG_INFO( "Finalize the Pythia8 Event." );
  m_gun.stat();
  return StatusCode::SUCCESS;
}



float Pythia8Gen::random_flat(){
  return m_gun.rndm.flat();
}

float Pythia8Gen::random_gauss(){
  return m_gun.rndm.gauss();
}

void Pythia8Gen::clear(){
  m_gun.event.reset();
}


