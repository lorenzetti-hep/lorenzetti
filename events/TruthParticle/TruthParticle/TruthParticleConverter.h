#ifndef TruthParticleConverter_h
#define TruthParticleConverter_h

#include "TruthParticle/TruthParticle.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/MsgStream.h"


namespace xAOD{

    struct TruthParticle_t{
        int pdgid;
        int seedid;
        float e;
        float et;
        float eta;
        float phi;
        float px;
        float py;
        float pz;
        float vx; // vertex position x (prod_vx)
        float vy; // vertex position y
        float vz; // vertex position z
    };

 
    class TruthParticleConverter: public MsgService{

        public:
            TruthParticleConverter():IMsgService(){;};
            ~TruthParticleConverter()=default;
            std::string key(){return m_key;};

            bool serialize(  std::string &, SG::EventContext &/*ctx*/, TTree *);
            bool deserialize( std::string &, int &, TTree *, SG::EventContext &/*ctx*/);

        private:
            // convert a class object into a struct
            bool convert(const TruthParticle *truth, TruthParticle_t &truth_t ) const;
            bool convert(const TruthParticle_t & , TruthParticle *&) const;
        
            template <class T> bool InitBranch(TTree* fChain, std::string branch_name, T* param) const;

            std::vector<xAOD::TruthParticle_t>  m_particles_t;

            std::string m_key;
    };
}
#endif


