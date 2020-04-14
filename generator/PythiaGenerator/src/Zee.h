#ifndef Zee_h
#define Zee_h

#include "Pythia8/Pythia.h"
#include "EventGenerator.h"


class Zee : public EventGenerator
{
  public:
    Zee();
    ~Zee()=default;

  private:
    virtual StatusCode generate(Pythia8::Event&, std::vector<Pythia8::Particle*>&, std::vector<std::vector<Pythia8::Particle*>> &) override;
};
#endif
