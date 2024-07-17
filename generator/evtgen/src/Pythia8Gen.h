

#ifndef guns_Pythia8Gen_h
#define guns_Pythia8Gen_h

#include "GenKernel/exceptions.h"
#include "GenKernel/IGenerator.h"
#include "GenKernel/Event.h"
#include "Pythia8/Pythia.h"

namespace generator
{

  class Pythia8Gen : public IGenerator
  {
  public:
    /*! Constructor */
    Pythia8Gen();
    /*! Destructor */
    virtual ~Pythia8Gen() = default;

    virtual StatusCode initialize() override;
    virtual StatusCode execute(HepMC3::GenEvent &) override;
    virtual StatusCode finalize() override;

    /**
     * @brief Returns a random float between 0 and 1
     * 
     * @return float 
     */
    virtual float random_flat() override;
    /**
     * @brief Returns a random float following a standard normal distribution
     * 
     * @return float 
     */
    virtual float random_gauss() override;
    /**
     * @brief Clear the event
     * 
     */
    virtual void clear() override;

    /**
     * @brief Returns a pointer to current pythia event
     * 
     * @return Pythia8::Event* 
     */
    Pythia8::Event *event() { return &m_gun.event; };
    /**
     * @brief Returns a pointer to the underlying pythia generator
     * 
     * @return Pythia8::Pythia* 
     */
    Pythia8::Pythia *core() { return &m_gun; };

  protected:
    std::string m_file;
    /**
     * @brief Pythia output level
     *
     */
    int m_outputLevel;
    /**
     * @brief Pythia seed
     *
     */
    int m_seed;
    /**
     * @brief Max number of errors
     *
     */
    int m_nAbort;
    /**
     * @brief Current number of errors
     *
     */
    int m_iAbort;

    /**
     * @brief Starting event number
     *
     */
    int m_eventNumber;
    /**
     * @brief Number of events
     *
     */
    int m_nEvent;

  private:
    /**
     * @brief The underlying Pythia8 generator
     *
     */
    Pythia8::Pythia m_gun;
  };

}
#endif
