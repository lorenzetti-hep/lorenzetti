#ifndef CaloHitConverter_h
#define CaloHitConverter_h

/** simulator includes **/
#include "CaloHit/CaloHitContainer.h"

namespace xAOD{

    typedef std::map<const CaloHit *, int> hit_links_t;


    struct CaloHit_t{

        int sampling;
        int detector;
        float eta;
        float phi;
        float deta;
        float dphi;
        int bcid_start;
        int bcid_end;
        float bc_duration;
        std::vector<float> edep;
        std::vector<float> tof; // 
        unsigned long int hash;
    };


    class CaloHitConverter: public MsgService{

        public:
        
            CaloHitConverter( std::string seedKey, bool onlyRoi, 
                              float etaWindow, float phiWindow):IMsgService(),m_seedKey(seedKey), m_onlyRoi(onlyRoi),
                                                                m_etaWindow(etaWindow),m_phiWindow(phiWindow){;};

            ~CaloHitConverter()=default;

            std::string key(){return m_key;};

            bool serialize(  std::string &, SG::EventContext &/*ctx*/,  TTree *);
            bool deserialize( std::string &, int &, TTree *, SG::EventContext &/*ctx*/);

        private:

            // convert a class object into a struct
            bool convert(const CaloHit *truth, CaloHit_t &truth_t ) const;
            bool convert(const CaloHit_t & , CaloHit *&) const;
            template <class T> bool InitBranch(TTree* fChain, std::string branch_name, T* param) const;

            std::string m_seedKey;
            bool m_onlyRoi;
            float m_etaWindow;
            float m_phiWindow;

            std::vector<xAOD::CaloHit_t> m_hits_t;

            std::string m_key;
    };

}
#endif