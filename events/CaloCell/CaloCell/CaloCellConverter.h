#ifndef CaloCellConverter_h
#define CaloCellConverter_h

/** simulator includes **/
#include "CaloCell/CaloCellContainer.h"
#include "CaloCell/CaloDetDescriptor.h"

namespace xAOD{


    typedef std::map<const xAOD::CaloCell*, int> cell_links_t;

    struct CaloCell_t{
        float e;
        float tau; 
        float et;
        float eta;
        float phi;
        float deta;
        float dphi;
        int descriptor_link;
    };



    typedef std::map<const CaloDetDescriptor *, int> descriptor_links_t;


    struct CaloDetDescriptor_t{

        int sampling;
        int detector;
        float eta;
        float phi;
        float deta;
        float dphi;
        float e;
        float tau; 
        float edep;
        float tof; 
        int bcid_start;
        int bcid_end;
        float bc_duration;
        std::vector<float> pulse;
        std::vector<float> edep_per_bunch;
        std::vector<float> tof_per_bunch; 
        unsigned long int hash;
        int cell_link;
    };

    class CaloCellConverter: public MsgService{

        public:
            CaloCellConverter( std::string seedKey, float etaWindow, float phiWindow):
                               IMsgService(),m_seedKey(seedKey), m_etaWindow(etaWindow), m_phiWindow(phiWindow){;};

            ~CaloCellConverter()=default;

            std::string name(){return "CaloCellContainer";};

            bool serialize(  std::string &cellKey, SG::EventContext &/*ctx*/, TTree * );
            bool deserialize( std::string &, int &, TTree *, SG::EventContext &/*ctx*/);

        private:

            // convert a class object into a struct
            bool convert(const CaloCell *           , CaloCell_t &  , int link) const;
            bool convert(const CaloCell_t &         , CaloCell   *& ) const;
            bool convert(const CaloDetDescriptor *  , CaloDetDescriptor_t &, int link ) const;
            bool convert(const CaloDetDescriptor_t &, CaloDetDescriptor *& ) const;
            
            template <class T> bool InitBranch(TTree* fChain, std::string branch_name, T* param) const;

            std::vector<xAOD::CaloCell_t> m_cells_t;
            std::vector<xAOD::CaloDetDescriptor_t> m_descriptors_t;
            std::string m_seedKey;
            float m_etaWindow, m_phiWindow; 
    };
}
#endif


