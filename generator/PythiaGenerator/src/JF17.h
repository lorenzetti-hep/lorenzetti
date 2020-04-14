#ifndef JF17_h
#define JF17_h

#include "Pythia8/Pythia.h"
#include "EventGenerator.h"


class JF17 : public EventGenerator
{
  public:
    JF17();
    ~JF17()=default;

  private:
    virtual StatusCode generate(Pythia8::Event&, std::vector<Pythia8::Particle*>&, std::vector<std::vector<Pythia8::Particle*>> &) override;

    float m_etaWindow;
    float m_phiWindow;
};
#endif
