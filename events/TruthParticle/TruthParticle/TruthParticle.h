#ifndef TruthParticle_h
#define TruthParticle_h

#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"

namespace xAOD{
  
  class TruthParticle: public Gaugi::EDM
  {  
    public:

      /*! Contructor */
      TruthParticle();
      /*! Contructor */
      TruthParticle( float e, float et, float eta, float phi, 
                     float px, float py, float pz, int pdgid, 
                     float vx, float vy, float vz); // production vertex position
                    //  float seedEta, float seedPhi, float seedEtot, float seedEttot, // generator seed properties
                      
      /*! Destructor */
      ~TruthParticle()=default;
      /*! The eta center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! The phi center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! set/get energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e, setE, e );
      /*! set/get transverse energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );
      /*! set/get particle identification */
      PRIMITIVE_SETTER_AND_GETTER( int, m_pdgid, setPdgid, pdgid );
      
      PRIMITIVE_SETTER_AND_GETTER( float, m_px, setPx, px );
      
      PRIMITIVE_SETTER_AND_GETTER( float, m_py, setPy, py );

      PRIMITIVE_SETTER_AND_GETTER( float, m_pz, setPz, pz );

      /*! set/get vertex position */
      PRIMITIVE_SETTER_AND_GETTER( float, m_vtx_x, setVx, vx );

      PRIMITIVE_SETTER_AND_GETTER( float, m_vtx_y, setVy, vy );

      PRIMITIVE_SETTER_AND_GETTER( float, m_vtx_z, setVz, vz );

      /*! set/get main_particle seed properties*/
      // PRIMITIVE_SETTER_AND_GETTER(float, m_seed_eta, setSeedEta, seedEta);

      // PRIMITIVE_SETTER_AND_GETTER(float, m_seed_phi, setSeedPhi, seedPhi);

      // PRIMITIVE_SETTER_AND_GETTER(float, m_seed_etot, setSeedEtot, seedEtot);

      // PRIMITIVE_SETTER_AND_GETTER(float, m_seed_ettot, setSeedEttot, seedEttot);



    private:

      int m_pdgid;
      float m_e;
      float m_et;
      float m_eta;
      float m_phi;
      float m_px;
      float m_py;
      float m_pz;
      float m_vtx_x;      // vertex position
      float m_vtx_y;
      float m_vtx_z;
      // properties from Generator::Seed
      // float m_seed_etot;
      // float m_seed_ettot;
      // float m_seed_eta;
      // float m_seed_phi;
  };
}
#endif
