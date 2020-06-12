#ifndef Seed_h
#define Seed_h

#include "Pythia8/Pythia.h"
#include "../EventGenerator.h"


class Seed : public Physics
{
  public:
    Seed();
    ~Seed()=default;

    virtual StatusCode initialize() override;
    virtual StatusCode run( std::vector<xAOD::seed_t>&, std::vector<std::vector<Pythia8::Particle*>> &) override;
    virtual StatusCode finalize() override;

  private:

    int m_outputLevel;
    float m_eta;
    float m_phi;
};
#endif
