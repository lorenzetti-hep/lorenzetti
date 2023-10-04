#ifndef EventSeedConverter_h
#define EventSeedConverter_h

/** simulator includes **/
#include "EventInfo/EventSeedContainer.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/MsgStream.h"


namespace xAOD{

    struct EventSeed_t{
        int id;
        float e;
        float et;
        float eta;
        float phi;
    };

 
    class EventSeedConverter: public MsgService{

        public:
            EventSeedConverter():IMsgService(){;};
            ~EventSeedConverter()=default;

            std::string key(){return m_key;};
             bool serialize(  std::string &, SG::EventContext &/*ctx*/, TTree *);
             bool deserialize( std::string &, int &, TTree *, SG::EventContext &/*ctx*/);

        private:
            // convert a class object into a struct
            bool convert(const EventSeed *truth, EventSeed_t &truth_t ) const;
            bool convert(const EventSeed_t & , EventSeed *&) const;
            template <class T> bool InitBranch(TTree* fChain, std::string branch_name, T* param) const;

            std::vector<xAOD::EventSeed_t>  m_seeds_t;

            std::string m_key;
    };

}


#endif


