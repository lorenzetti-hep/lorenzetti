#ifndef ComponentAccumulator_h 
#define ComponentAccumulator_h

#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Algorithm.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/Timer.h"
#include <string>
#include <vector>
#include <ROOT/TBufferMerger.hxx>
using namespace ROOT::Experimental;

namespace Gaugi{

  class ComponentAccumulator : public MsgService,
                               public PropertyService
  {
    public:

      /** Constructor **/
      ComponentAccumulator( std::string name );

      /** Destructor **/
      virtual ~ComponentAccumulator();

      /** add tool **/
      void push_back( Gaugi::Algorithm* );


      void initialize();

      void bookHistograms( SG::EventContext *ctx ) const;

      void run( SG::EventContext *ctx, int evt ) const;

      void finalize();

    private:

      // monitoring variables
      //Timer m_timer;
      // Store gate
      //SG::StoreGate *m_store;
      // the event context
      //SG::EventContext m_ctx;
      // list of alg tools to be executed in loop
      std::vector < Gaugi::Algorithm* > m_toolHandles;
  };

}
#endif

