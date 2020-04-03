
#include "GaugiKernel/StoreGate.h"
#include "TROOT.h"

using namespace SG;


StoreGate::StoreGate( std::string &outputfile): 
  IMsgService("StoreGate"),
  m_currentPath("")
{
  // This must be used for multithreading root reader 
  ROOT::EnableThreadSafety();
  m_file = new TFile(outputfile.c_str(), "recreate");
}



StoreGate::~StoreGate()
{
  m_file->Write();
  for( const auto &o : m_objs){
    delete o.second;
  }
  m_file->Close();
}


void StoreGate::mkdir( std::string &path ){
  m_file->mkdir(path.c_str());
  m_currentPath = path;
}


void StoreGate::cd( std::string &path ){
  m_currentPath = path;
}



bool StoreGate::add( TObject* obj){  
	std::string feature(obj->GetName());
  std::string fullpath = m_currentPath + "/" + feature;
	if( m_objs.find( fullpath ) != m_objs.end() ) 
  {
    MSG_WARNING("It's not possible to attach the histogram with name " << feature << " into this path " << m_currentPath);
		return false;
  }
  m_objs[fullpath] = obj;
  return true;
}


TH1F* StoreGate::hist1( std::string &path )
{
	if( m_objs.find( path ) != m_objs.end() )  
    MSG_FATAL("It's not possible to retrieve the 1D histogram with abspath " << path << " from the StoreGate");
  return ((TH1F*)m_objs[path]);
}


TH2F* StoreGate::hist2( std::string &path )
{
	if( m_objs.find( path ) != m_objs.end() )  
    MSG_FATAL("It's not possible to retrieve the 2D histogram with abspath " << path << " from the StoreGate");
  return ((TH2F*)m_objs[path]);
}


TTree* StoreGate::tree( std::string &path )
{
	if( m_objs.find( path ) != m_objs.end() )  
    MSG_FATAL("It's not possible to retrieve the TTree with abspath " << path << " from the StoreGate");
  return ((TTree*)m_objs[path]);
}



