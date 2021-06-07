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
#include "TH1I.h"
#include "TH2F.h"
#include "TH1.h"
#include "TH2Poly.h"
#include <ROOT/TBufferMerger.hxx>
using namespace ROOT::Experimental;

namespace SG
{
  class StoreGate: public MsgService
  {
    public:
  
      /** Constructor **/
      StoreGate( std::string outputfile);

      /** Destructor **/
      ~StoreGate()=default;     
      
      void save();

      /** Create directory **/
      void mkdir(std::string );
      
      /** Move between directories **/
      void cd(std::string="" );
      
      /** Create 1D histogram **/
      bool add( TObject *);
      
      /** Get 1D pointer **/
      TH1F* hist1( std::string );
      
      /** Get 2D pointer **/
      TH2F* hist2( std::string );
     
      /** Get ID pointer **/
      TH1I* histI( std::string );

      /** Get special TH2 histogram */
      TH2Poly* hist2P( std::string );


      /** Get 2D pointer **/
      TTree* tree( std::string );
  
      void setLabels(TH1* histo, const std::vector<std::string>& labels);

      bool decorate( std::string key, TObject* );
      
      TObject* decorator(std::string);


    private:

      // the current path
      std::string m_currentPath;
      // file
      std::unique_ptr<TFile> m_file;

      // ROOT objects
      std::map<std::string, TObject *> m_objs;
      std::map<std::string, TObject *> m_decorators;

  };
}
#endif
