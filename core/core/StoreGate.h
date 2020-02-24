#ifndef StoreGate_h 
#define StoreGate_h

/** standard libs **/
#include <string>
#include <vector>
#include <map>

/** ROOT libs **/
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

namespace SG
{
  class StoreGate
  {
    public:
  
      /** Constructor **/
      StoreGate( std::string="histo.root");
      /** Destructor **/
      ~StoreGate();     
      /** Create directory **/
      void mkdir(std::string);
      /** Move between directories **/
      void cd(std::string);
      /** Create 1D histogram **/
      void AddHistogram( std::string, std::string, int xbin, float xmin, float xmax );
      /** Create 2D histogram **/
      void AddHistogram( std::string, std::string, int xbin, float xmin, float xmax, int ybin, float ymin, float ymax );
      /** Get 1D pointer **/
      TH1F* hist1( std::string );
      /** Get 2D pointer **/
      TH2F* hist2( std::string );
  
    private:
      // the current path
      std::string m_currentPath;
      // the root file
      TFile *m_file;
      // book of 1D histograms
      std::map<std::string, TH1F *> m_h1_book;
      // book of 2D histograms
      std::map<std::string, TH2F *> m_h2_book;
  
  };
}
#endif

