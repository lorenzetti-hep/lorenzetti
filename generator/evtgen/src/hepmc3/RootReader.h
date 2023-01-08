

#ifndef evtgen_hepmc3_RootReader_h
#define evtgen_hepmc3_RootReader_h

#include "GenKernel/exceptions.h"
#include "GenKernel/IGenerator.h"
#include "GenKernel/Event.h"
#include "HepMC3/ReaderRootTree.h"
#include <random>



namespace generator{

  class RootReader  : public IGenerator
  {
    public:
      /*! Constructor */
      RootReader();
      /*! Destructor */
      virtual ~RootReader();

      virtual StatusCode initialize() override;
      virtual StatusCode execute( HepMC3::GenEvent&) override;
      virtual StatusCode finalize() override;


      virtual float random_flat() override;
      virtual float random_gauss() override;

    protected:

      std::string m_file;
      /*! Level message */
      int m_outputLevel;
      /*! HepMC root reader */
      HepMC3::ReaderRootTree *m_data;


      float m_seed;
      /*! mersenne_twister_engine */
      std::mt19937 m_rng;
      std::uniform_real_distribution<float> m_flat;
      std::normal_distribution<float> m_gauss;



  };

}
#endif
