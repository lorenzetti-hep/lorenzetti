#ifndef PythiaGenerator_LLPGun_h
#define PythiaGenerator_LLPGun_h

#include "P8Kernel/Algorithm.h"

namespace generator{

  class LZT_ATLAS_Info
  {
    public:
      double rmin = 475.; // mm
      double rmax = 4283.; // mm
      double zmax1 = 6056.5; // mm
      double zmax2 = 6534.0; // mm

      double emec_max_radii = 2032.; // mm
      double detector_max_radii = 3883.; // mm

      double eta_rmax; 
      double eta_zmax1; 
      double eta_zmax2;

      LZT_ATLAS_Info();

      bool withinDetector( double r, double z, double eta );

      double tauMax();  
  };


  class LLPGun : public Algorithm
  {
    public:
      LLPGun();
      ~LLPGun();
  
      virtual StatusCode initialize() override;
      virtual StatusCode execute( generator::Event& ) override;
      virtual StatusCode finalize() override;

      void fillParticle( int id, double ee, double thetaIn, double phiIn);
      void flatTauDist(Pythia8::Event &event, Pythia8::Particle &mother, double tau);
  
    private:

      void clearParticles();
  
      int m_pdgid;
      double m_energy;
      double m_energyMin;
      double m_energyMax;

      std::string m_file;
      LZT_ATLAS_Info m_lzt_atlas_info;

      TFile *m_rootFile;
      TH2F *m_decayPositionH;
      TH2F *m_decayPositionAbsAbsH;
      TH2F *m_decayWrtEtaH;
      TH1F *m_energyH;
      TH1F *m_sub_energyH;
      TH1F *m_cos_theta_starH;
      TH1F *m_sub_cos_theta_starH;
      TH1F *m_tauH;

     
  };

}
#endif
