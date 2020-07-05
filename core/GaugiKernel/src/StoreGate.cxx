#include <boost/algorithm/string/replace.hpp>
#include "GaugiKernel/StoreGate.h"
#include "TROOT.h"
#include <sstream>
#include <algorithm>

using namespace SG;


StoreGate::StoreGate( std::string outputfile, int index): 
  IMsgService("StoreGate"),
  m_currentPath("")
{
  // This must be used for multithreading root reader 
  ROOT::EnableThreadSafety();
  // remove .root in case of the user include it
  //std::replace( outputfile.begin(), outputfile.end(), ".root", "" );
  boost::replace_all(outputfile, ".root", "");
  if (index>=0){
    std::stringstream ss; 
    ss << outputfile << "_" << index; outputfile = ss.str();
  }
  m_file = new TFile( (outputfile+".root").c_str(), "recreate");
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
  if (!m_file->GetDirectory(path)){
    m_file->mkdir(path.c_str());
    m_file->cd(path.c_str());
    m_currentPath = path;
  }
}


void StoreGate::cd( std::string path ){
  m_currentPath = path;
  m_file->cd(path.c_str());
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


TTree* StoreGate::tree( std::string name )
{
  std::string path = m_currentPath+"/"+name;
  return ((TTree*)m_objs[path]);
}



