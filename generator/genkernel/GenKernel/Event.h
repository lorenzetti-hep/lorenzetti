#ifndef GenKernel_Event_h
#define GenKernel_Event_h

#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "TTree.h"
#include <vector>
#include <string>

namespace generator
{

  /**
   * @brief Particle struct used to dump the info into the ntuple
   *
   */
  struct particle_t
  {
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

  class Seed
  {

  public:
    /*! Constructor */
    Seed(float eta, float phi) : m_eta(eta), m_phi(phi), m_etot(0), m_ettot(0) { ; };
    /*! Destructor */
    ~Seed() = default;
    /**
     * @brief Adds the particle into the seed
     *
     * @param p
     */
    void push_back(const particle_t p)
    {
      m_etot += p.e;
      m_ettot += p.eT;
      m_particles.push_back(p);
    };

    /**
     * @brief Get eta value
     *
     * @return float
     */
    float eta() const { return m_eta; };
    /**
     * @brief Get phi value
     *
     * @return float
     */
    float phi() const { return m_phi; };
    /**
     * @brief Set eta value
     *
     * @param eta
     */
    void setEta(float eta) { m_eta = eta; };

    /**
     * @brief Set phi value
     *
     * @param phi
     */
    void setPhi(float phi) { m_phi = phi; };

    std::vector<particle_t> &operator*() { return m_particles; };

    float etot() const { return m_etot; };

    float ettot() const { return m_ettot; };

    /**
     * @brief Pushes a new particle into the seed
     *
     * @param isMain
     * @param bc_id
     * @param pdg_id PDGID of the particle
     * @param px X axis momentum
     * @param py Y axis momentum
     * @param pz Z axis momentum
     * @param eta Eta value
     * @param phi Phi value
     * @param xProd X axis production vertex
     * @param yProd Y axis production vertex
     * @param zProd Z axis production vertex
     * @param tProd Time of production
     * @param e Energy
     * @param eT Transverse energy
     */
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
      m_etot += e;
      m_ettot += eT;
      m_particles.push_back(particle_t{isMain, bc_id, pdg_id, px, py, pz,
                                       eta, phi, xProd, yProd, zProd, tProd, e, eT});
    };

  private:
    /**
     * @brief Eta value
     *
     */
    float m_eta;
    /**
     * @brief Phi value
     *
     */
    float m_phi;
    /**
     * @brief Particles inside the seed
     *
     */
    std::vector<particle_t> m_particles;
    /**
     * @brief Total energy value
     *
     */
    float m_etot;
    /**
     * @brief Total transverse energy value
     *
     */
    float m_ettot;
  };

  class Event
  {

  public:
    /*! Constructor */
    Event() : m_avgmu(0),
              m_eventNumber(-1)
    {
      ;
    };

    /*! Destructor */
    ~Event() = default;

    /**
     * @brief Add the seed into the current event
     *
     * @param seed The seed to be inserted
     */
    void push_back(Seed seed) { m_seeds.push_back(seed); };

    /**
     * @brief Set the average mu/pileup
     *
     * @param avgmu
     */
    void setAvgmu(float avgmu) { m_avgmu = avgmu; };

    /**
     * @brief Get the average mu/pileup
     *
     * @return float
     */
    float avgmu() const { return m_avgmu; };

    /**
     * @brief Set the Event Number
     *
     * @param eventNumber
     */
    void setEventNumber(int eventNumber) { m_eventNumber = eventNumber; };

    /**
     * @brief Get the  event number
     *
     * @return int
     */
    int eventNumber() const { return m_eventNumber; };

    std::vector<Seed> &operator*() { return m_seeds; };
    Seed &operator[](unsigned idx) { return m_seeds[idx]; }

    int size() const { return m_seeds.size(); };

  protected:
    /**
     * @brief Average mu/pileup
     *
     */
    float m_avgmu;
    /**
     * @brief Event number
     *
     */
    int m_eventNumber;
    /**
     * @brief Event Seeds
     *
     */
    std::vector<Seed> m_seeds;
  };
}

#endif
