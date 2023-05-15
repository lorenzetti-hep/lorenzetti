

#ifndef guns_Pythia8Gen_h
#define guns_Pythia8Gen_h

#include "GenKernel/exceptions.h"
#include "GenKernel/IGenerator.h"
#include "GenKernel/Event.h"
#include "Pythia8/Pythia.h"


namespace generator{

  class Pythia8Gen  : public IGenerator
  {
    public:
      /*! Constructor */
      Pythia8Gen();
      /*! Destructor */
      virtual ~Pythia8Gen() = default;

      virtual StatusCode initialize() override;
      virtual StatusCode execute( HepMC3::GenEvent&) override;
      virtual StatusCode finalize() override;


      virtual float random_flat() override;
      virtual float random_gauss() override;

      virtual void clear() override;


      Pythia8::Pythia * core(){return &m_gun;};
      

    protected:

      std::string m_file;
      /*! Level message */
      int m_outputLevel;
      /*! Pythia seed */
      int m_seed;
      /* Max number of errors */
      int m_nAbort;
      /*! Number of errors */
      int m_iAbort;

      /*! Event number control */
      int m_eventNumber;
      int m_nEvent;

    private:

      /*! The pythia event generator */
      Pythia8::Pythia m_gun;

  };

}
#endif
