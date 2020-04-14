
#include "GaugiKernel/StoreGate.h"
#include "TROOT.h"
#include <sstream>

using namespace SG;


StoreGate::StoreGate( std::string &outputfile, int index): 
  IMsgService("StoreGate"),
  m_currentPath("")
{
  // This must be used for multithreading root reader 
  ROOT::EnableThreadSafety();
  std::stringstream ss; 
  if (index<0)
    ss << outputfile << ".root";
  else
    ss << outputfile << "_" << index << ".root";
  m_file = new TFile(ss.str().c_str(), "recreate");
}



StoreGate::~StoreGate()
{
  MSG_INFO( "Writing all root objects into the file" ); 
  m_file->Write();
  /*
  for( const auto &o : m_objs){
    //o.second->Write();
    MSG_INFO( o->GetName() << " With " << o->GetEntries() << " entries." );
    delete o.second;
  }*/
  m_file->Close();
}


void StoreGate::mkdir( std::string path ){
  MSG_INFO( "Creating a directory with name " << path );
  m_file->mkdir(path.c_str());
  m_file->cd(path.c_str());
  m_currentPath = path;
}


void StoreGate::cd( std::string path ){
  m_currentPath = path;
  m_file->cd(path.c_str());
}



bool StoreGate::add( TObject* obj){  
	std::string feature(obj->GetName());
  std::string fullpath = m_currentPath + "/" + feature;
	MSG_INFO( "Attaching into " << fullpath );
  if( m_objs.find( fullpath ) != m_objs.end() ) 
  {
    MSG_WARNING("It's not possible to attach the histogram with name " << feature << " into this path " << m_currentPath);
		return false;
  }
  m_objs[fullpath] = obj;
  return true;
}


TH1F* StoreGate::hist1( std::string name )
{
  std::string path = m_currentPath+"/"+name;
	//if( m_objs.find( path ) == m_objs.end() )  
  //  MSG_FATAL("It's not possible to retrieve the 1D histogram with abspath " << path << " from the StoreGate");
  return ((TH1F*)m_objs[path]);
}


TH2F* StoreGate::hist2( std::string name )
{
	//if( m_objs.find( path ) == m_objs.end() )  
  //  MSG_FATAL("It's not possible to retrieve the 2D histogram with abspath " << path << " from the StoreGate");
  //MSG_INFO( "Getting path " << path );

  //MSG_INFO( m_objs[path] );
  std::string path = m_currentPath+"/"+name;
	//if( m_objs.find( path ) == m_objs.end() )  
  return ((TH2F*)m_objs[path]);
}


TTree* StoreGate::tree( std::string path )
{
	if( m_objs.find( path ) == m_objs.end() )  
    MSG_FATAL("It's not possible to retrieve the TTree with abspath " << path << " from the StoreGate");
  return ((TTree*)m_objs[path]);
}



