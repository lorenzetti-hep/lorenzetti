#include <boost/algorithm/string/replace.hpp>
#include "GaugiKernel/StoreGate.h"
#include "TROOT.h"
#include <sstream>
#include <algorithm>

using namespace SG;


StoreGate::StoreGate( std::string outputfile): 
  IMsgService("StoreGate"),
  m_currentPath("")
{
  // This must be used for multithreading root reader 
  ROOT::EnableThreadSafety();
  m_file = std::unique_ptr<TFile>(new TFile( outputfile.c_str(), "recreate"));
  
}



void StoreGate::save()
{
  MSG_INFO("Saving StoreGate...")
  
  for( auto&it : m_decorators)
  {
    delete it.second;
  }

  MSG_INFO( "Writing all root objects into the file and delete all decorators" ); 
  MSG_INFO( "Decorators will no be saved into the root file.")
  for( auto it : m_objs)
  {
    MSG_INFO( it.first  << " " << it.second);
    //it.second->Write();
  }

  m_file->Write();
  m_file->Close();
}


void StoreGate::mkdir( std::string path ){
  MSG_DEBUG( "Creating a directory with name " << path );
  if (!m_file->GetDirectory(path.c_str())){
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
  return ((TH1F*)m_objs[path]);
}


TH2F* StoreGate::hist2( std::string name )
{
  std::string path = m_currentPath+"/"+name;
  return ((TH2F*)m_objs[path]);
}


TH1I* StoreGate::histI( std::string name )
{
  std::string path = m_currentPath+"/"+name;
  return ((TH1I*)m_objs[path]);
}


TH2Poly* StoreGate::hist2P( std::string name )
{
  std::string path = m_currentPath+"/"+name;
  return ((TH2Poly*)m_objs[path]);
}


TTree* StoreGate::tree( std::string name )
{
  std::string path = m_currentPath+"/"+name;
  return ((TTree*)m_objs[path]);
}


void StoreGate::setLabels(TH1* histo, const std::vector<std::string>& labels) {
    if ( ! labels.empty() ){
        for ( int i = 0; i < std::min( (int)labels.size(), (int)histo->GetNbinsX() ); ++i ) {
            int bin = i+1;
            histo->GetXaxis()->SetBinLabel(bin, labels[i].c_str());
            MSG_DEBUG("setting label X" <<  labels[i] << " for bin " << bin);
        }

        for ( int i = (int)histo->GetNbinsX(); i < std::min( (int)labels.size(), (int)histo->GetNbinsX()+(int)histo->GetNbinsY() ); ++i ) {
            int bin = i+1-(int)histo->GetNbinsX();
            histo->GetYaxis()->SetBinLabel(bin, labels[i].c_str());
            MSG_DEBUG("setting label Y" <<  labels[i] << " for bin " << bin);
        }
    }
}


bool StoreGate::decorate( std::string name, TObject* obj){  
  if( m_decorators.find( name ) != m_decorators.end() ) 
  {
    MSG_WARNING("It's not possible decorate with name " << name);
		return false;
  }
  m_decorators[name] = obj;
  return true;
}


TObject* StoreGate::decorator( std::string name )
{
  return m_decorators[name];
}