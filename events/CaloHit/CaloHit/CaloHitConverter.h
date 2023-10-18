#ifndef CaloHitConverter_h
#define CaloHitConverter_h

#include "CaloHit/CaloHitContainer.h"
#include "GaugiKernel/DataHandle.h"

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


    class CaloHitConverter{

        public:
        
            CaloHitConverter( std::string seedKey, bool onlyRoi, float etaWindow, float phiWindow):
                              m_seedKey(seedKey), m_onlyRoi(onlyRoi),m_etaWindow(etaWindow),m_phiWindow(phiWindow){;};

            ~CaloHitConverter()=default;


            bool serialize(  std::string &, SG::EventContext &/*ctx*/,  TTree *) const;
            bool deserialize( std::string &, int &, TTree *, SG::EventContext &/*ctx*/) const;
            bool convert(const CaloHit *truth, CaloHit_t &truth_t ) const;
            bool convert(const CaloHit_t & , CaloHit *&) const;


        private:

            std::string m_seedKey;
            bool m_onlyRoi;
            float m_etaWindow;
            float m_phiWindow;
    };

}
#endif