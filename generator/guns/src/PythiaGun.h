

#ifndef guns_PythiaGun_h
#define guns_PythiaGun_h

#include "GenKernel/exceptions.h"
#include "GenKernel/IGenerator.h"
#include "GenKernel/Event.h"
#include "Pythia8/Pythia.h"


namespace generator{

  class PythiaGun  : public IGenerator
  {
    public:
      /*! Constructor */
      PythiaGun();
      /*! Destructor */
      virtual ~PythiaGun() = default;

      virtual StatusCode initialize() override;
      virtual StatusCode execute( HepMC3::GenEvent&) override;
      virtual StatusCode finalize() override;


      virtual float random_flat() override;
      virtual float random_gauss() override;

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
      mutable Pythia8::Pythia m_gun;

  };

}
#endif
