#ifndef EventInfoConverter_h
#define EventInfoConverter_h

/** simulator includes **/
#include "EventInfo/EventInfoContainer.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/MsgStream.h"


namespace xAOD{

    struct EventInfo_t{
        float runNumber;
        float eventNumber;
        float avgmu;
    };



    class EventInfoConverter: public MsgService{

        public:
            EventInfoConverter():IMsgService(){;};
            ~EventInfoConverter()=default;

            std::string name(){return "EventInfoContainer";};
            void clear();

            bool serialize(  std::string &, SG::EventContext &/*ctx*/, TTree *);
            bool deserialize( std::string &, int &, TTree *, SG::EventContext &/*ctx*/);

        private:
            // convert a class object into a struct
            bool convert(const EventInfo *truth, EventInfo_t &truth_t ) const;
            bool convert(const EventInfo_t & , EventInfo *&) const;
            template <class T> bool InitBranch(TTree* fChain, std::string branch_name, T* param) const;

            std::vector<xAOD::EventInfo_t> m_events_t;
    };
}
#endif


