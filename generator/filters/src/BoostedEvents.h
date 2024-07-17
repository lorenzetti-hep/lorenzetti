#ifndef filters_BoostedEvents_h
#define filters_BoostedEvents_h

#include "GenKernel/IAlgorithm.h"
#include "GenKernel/IGenerator.h"
#include "SingleParticle.h"
#include "Pythia8/Pythia.h"

namespace generator
{
  /**
   * @brief Class for generating boosted electrons
   * For now, this class ony works with Pythia generator
   * 
   */
  class BoostedEvents : public SingleParticle
  {
  public:
    BoostedEvents(const std::string, IGenerator *);
    ~BoostedEvents();

    virtual StatusCode initialize() override;
    virtual StatusCode execute(Event &) override;
    virtual StatusCode finalize() override;

  private:
    /**
     * @brief Computes the delta R between two particles
     *
     * @param eta1 eta value from particle 1
     * @param phi1 phi value from particle 1
     * @param eta2 eta value from particle 2
     * @param phi2 phi value from particle 2
     * @return float The computed delta R
     */
    float dR(float eta1, float phi1, float eta2, float phi2) const;

    /**
     * @brief Max deltaR allowed
     * 
     */
    float m_deltaR;
    /**
     * @brief If true, considers that the introduced particle has a lifetime
     * 
     */
    bool m_hasLifetime;
    /**
     * @brief If true, considers that the introduced particle is at rest
     * 
     */
    bool m_atRest;
    /**
     * @brief PDG ID of the particle to be generated
     * 
     */
    int m_pdgid;
  };
}
#endif
