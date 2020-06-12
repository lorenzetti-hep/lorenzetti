#ifndef ParticleGun_h
#define ParticleGun_h

#include "Pythia8/Pythia.h"
#include "../EventGenerator.h"


class ParticleGun : public Physics
{
  public:
    ParticleGun();
    ~ParticleGun()=default;

    virtual StatusCode initialize() override;
    virtual StatusCode run( std::vector<xAOD::seed_t>&, std::vector<std::vector<Pythia8::Particle*>> &) override;
    virtual StatusCode finalize() override;

  private:

    void fillParticle( Pythia8::Event& event,int id, double emin, double emax, double thetaIn, double phiIn,
                       Pythia8::ParticleData& pdt, bool atRest, bool hasLifetime); 


    int m_abort;
    int m_nAbort;
    int m_seed;
    int m_outputLevel;

    float m_theta;
    float m_eta;
    float m_phi;
    float m_energyMin;
    float m_energyMax;
    bool m_hasLifetime;
    bool m_atRest;
    int m_pdgid;
    int m_numberOfParticles;
   
    Pythia8::Pythia m_pythia;
};
#endif
