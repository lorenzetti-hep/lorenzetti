#ifndef P8Kernel_Cluster_h
#define P8Kernel_Cluster_h

#include <vector>


namespace generator{

  /* Particle struct used to dump the info into the ntuple */
  struct particle_t{
    int isMain;
    int bc_id;
    int pdg_id;
    float px;
    float py;
    float pz;
    float eta;
    float phi;
    float xProd;
    float yProd;
    float zProd;
    float tProd;
    float e;
    float eT;
  };
  
  
  /* Seed */
  class Seed{
  
    public:
      /*! Constructor */
      Seed(float eta, float phi):m_eta(eta), m_phi(phi), m_etot(0), m_ettot(0) {;};
      /*! Destructor */
      ~Seed()=default;
      /*! Add the particle struct into the cluster */
      void push_back( const particle_t p ){
        m_etot+=p.e;
        m_ettot+=p.eT;
        m_particles.push_back(p);
      };
  
      /*! Get eta seed position */
      float eta() const {return m_eta;};
      /*! Get phi seed position */
      float phi() const {return m_phi;};
      /*! Set eta seed position */
      void setEta(float eta){ m_eta=eta; };
      /*! Set phi seed position */
      void setPhi(float phi){ m_phi=phi; };
  
      std::vector<particle_t>& operator*() { return m_particles;};
  
      float etot() const {return m_etot;};
  
      float ettot() const {return m_ettot;};
  
 
      void emplace_back(int isMain,
                        int bc_id,
                        int pdg_id,
                        float px,
                        float py,
                        float pz,
                        float eta,
                        float phi,
                        float xProd,
                        float yProd,
                        float zProd,
                        float tProd,
                        float e,
                        float eT)
      { 
        m_etot+=e;
        m_ettot+=eT;
        m_particles.push_back( particle_t{isMain,bc_id,pdg_id,px,py,pz,eta,phi,xProd,yProd,zProd,tProd,e,eT});
      };
  
  
    private:
  
      /*! Eta */
      float m_eta;
      /*! Phi */
      float m_phi;
      /* Particles inside of this seed*/
      std::vector<particle_t> m_particles;
  
      float m_etot;
      float m_ettot;

  };
  
  
  /* Event geenrator */
  class Event{
  
    public:
      /*! Constructor */
      Event()=default;
      /*! Destructor */
      ~Event()=default;
      /*! Add seed into the current event*/
      void push_back( Seed seed ){ m_seeds.push_back( seed ); };

      /*! Set average mu */
      void setAvgmu( float avgmu ){ m_avgmu=avgmu; };
      /*! Get average mu */
      float avgmu() const { return m_avgmu; };
  
      std::vector<Seed>& operator*() { return m_seeds;};
  
 
      Seed& operator[](unsigned idx){ return m_seeds[idx]; }

    protected:
  
      /* Average mu */
      float m_avgmu;
      /* Seeds */
      std::vector< Seed > m_seeds;
  };

}

#endif










