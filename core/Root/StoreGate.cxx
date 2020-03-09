
#include "core/StoreGate.h"
#include "core/macros.h"
#include "TROOT.h"
#include <iostream>

using namespace SG;

StoreGate::StoreGate( std::string name): m_currentPath("")
{
  // This must be used for multithreading root reader 
  ROOT::EnableThreadSafety();


  //MSG_INFO( "Creating histogram file with name: " << name );
  //m_file = new TFile(name.c_str(), "new");
  m_file = new TFile(name.c_str(), "recreate");
}

StoreGate::~StoreGate()
{
  //MSG_INFO( "Writing histogram file..." );
  m_file->Write();


  /** Delete all TH1F histograms **/
  for( const auto &p : m_h1_book){
    //p.second->Write();
    delete p.second;
  }
  /** Delete all TH2F histograms **/
  for( const auto &p : m_h2_book){
    //p.second->Write();
    delete p.second;
  }
  m_file->Close();

}


void StoreGate::mkdir( std::string path ){
  m_file->mkdir(path.c_str());
  m_currentPath = path;
}


void StoreGate::cd( std::string path ){
  m_currentPath = path;
}


bool StoreGate::AddHistogram( std::string key, std::string s, int xbin, float xmin, float xmax )
{
  m_file->cd(m_currentPath.c_str());
  std::string path = m_currentPath+"/"+key;
  if ( !(m_h1_book.find(path) == m_h1_book.end())  || !(m_h2_book.find(path) == m_h2_book.end())){
    //MSG_WARNING( "The histogram with path " << path << " exist into the histogram book." );
    return false;
  }
  std::cout << "Add " << path << std::endl; 
  m_h1_book[path] = new TH1F(key.c_str(),s.c_str(),xbin,xmin,xmax);
  return true;
}


bool StoreGate::AddHistogram( std::string key, std::string s, int xbin, float xmin, float xmax, int ybin, float ymin, float ymax )
{
  m_file->cd(m_currentPath.c_str());
  std::string path = m_currentPath+"/"+key;
  if ( !(m_h1_book.find(path) == m_h1_book.end())  || !(m_h2_book.find(path) == m_h2_book.end())){
    //MSG_WARNING( "The histogram with path " << path << " exist into the histogram book." );
    return false;
  }
  std::cout << "Add " << path << std::endl; 
  m_h2_book[path] = new TH2F(key.c_str(),s.c_str(),xbin,xmin,xmax,ybin,ymin,ymax);
  return true;
}


TH1F * StoreGate::hist1( std::string key )
{
  std::cout << key << std::endl;
  return m_h1_book[key];
}


TH2F * StoreGate::hist2( std::string key )
{
  return m_h2_book[key];
}




