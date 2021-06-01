#ifndef ComponentAccumulator_h 
#define ComponentAccumulator_h

#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/ComponentReader.h"
#include "GaugiKernel/Timer.h"
#include <string>
#include <vector>

namespace Gaugi{

  class ComponentAccumulator : public MsgService,
                               public PropertyService
  {
    public:

      /** Constructor **/
      ComponentAccumulator( std::string name, std::string output );

      /** Destructor **/
      virtual ~ComponentAccumulator();

      /** add tool **/
      void push_back( Gaugi::Algorithm* );

      void setReader( Gaugi::ComponentReader* ); 

      /** run events **/
      void run( int evt );

      void initialize();

      void finalize();

    private:

      /** Pre execution used in event action step **/
      void BeginOfEvent();
      /** Execute event **/
      void ExecuteEvent();
      /** Pos execution used in event action step **/
      void EndOfEvent();
      /** book all histograms here **/
      void bookHistograms();

      // monitoring variables
      Timer m_timer;

      // Store gate
      SG::StoreGate m_store;
      // the event context
      SG::EventContext m_ctx;
      // list of alg tools to be executed in loop
      std::vector < Gaugi::Algorithm* > m_toolHandles;
      // event stream reader
      Gaugi::ComponentReader *m_reader;
  };

}
#endif

