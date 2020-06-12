#ifndef JF17_h
#define JF17_h

#include "Pythia8/Pythia.h"
#include "../EventGenerator.h"

class JF17 : public Physics
{
  public:

    JF17();
    ~JF17()=default;

    virtual StatusCode initialize() override;
    virtual StatusCode run( std::vector<xAOD::seed_t>&, std::vector<std::vector<Pythia8::Particle*>> &) override;
    virtual StatusCode finalize() override;

  private:

    int m_abort;
    int m_nAbort;
    int m_seed;
    int m_select;
    int m_outputLevel;
    float m_minPt;
    float m_etaMax;
    float m_etaWindow;
    float m_phiWindow;
 
    std::string m_mainFile;
    
    Pythia8::Pythia m_pythia;
};
#endif
