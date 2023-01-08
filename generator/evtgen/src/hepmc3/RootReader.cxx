#include "RootReader.h"

using namespace generator;

RootReader::RootReader(): 
  IMsgService("RootReader"),
  IGenerator(),
  m_rng(0),
  m_flat(0,1),
  m_gauss(0,1)
{
  declareProperty( "File"       , m_file="" );
  declareProperty( "Seed"       , m_seed=0  );
}


RootReader::~RootReader(){
  if(m_data){
    delete m_data;
  }
}

StatusCode RootReader::initialize()
{
  m_data = new HepMC3::ReaderRootTree(m_file);
  if (m_data->failed()){
    MSG_FATAL("It is not possible to initialize the HepMC reader. Abort!");
  }

  m_rng.seed(m_seed);

  return StatusCode::SUCCESS;
}


StatusCode RootReader::execute(HepMC3::GenEvent &evt){

  if (m_data->failed()){
    MSG_INFO("End of file. Stop the execution.");
    throw EndOfFile();
  }
  m_data->read_event(evt);
  return StatusCode::SUCCESS;
}



StatusCode RootReader::finalize()
{
  m_data->close();
  return StatusCode::SUCCESS;
}



float RootReader::random_flat(){
  return m_flat(m_rng);
}

float RootReader::random_gauss(){
  return m_gauss(m_rng);
}