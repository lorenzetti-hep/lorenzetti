#ifndef StoreGate_h 
#define StoreGate_h

#include "GaugiKernel/MsgStream.h"

/** standard libs **/
#include <string>
#include <vector>
#include <map>

/** ROOT libs **/
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

namespace SG
{
  class StoreGate: public MsgService
  {
    public:
  
      /** Constructor **/
      StoreGate( std::string &outputfile);
      
      /** Destructor **/
      ~StoreGate();     
      
      /** Create directory **/
      void mkdir(std::string );
      
      /** Move between directories **/
      void cd(std::string );
      
      /** Create 1D histogram **/
      bool add( TObject *);
      
      /** Get 1D pointer **/
      TH1F* hist1( std::string );
      
      /** Get 2D pointer **/
      TH2F* hist2( std::string );
      
      /** Get 2D pointer **/
      TTree* tree( std::string );
  
    private:

      // the current path
      std::string m_currentPath;
      // the root file
      TFile *m_file;
      // ROOT objects
      std::map<std::string, TObject *> m_objs;
  
  };
}
#endif
