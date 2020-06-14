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
  
      const std::vector<particle_t>& operator*() const { return m_particles;};
  
      float etot() const {return m_etot;};
  
      float ettot() const {return m_ettot;};
  
  
  
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
  
      const std::vector<Seed>& operator*() const { return m_seeds;};
  
  
    protected:
  
      /* Average mu */
      float m_avgmu;
      /* Seeds */
      std::vector< Seed > m_seeds;
  };

}

#endif










