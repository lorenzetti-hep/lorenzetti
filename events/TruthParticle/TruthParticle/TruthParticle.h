#ifndef TruthParticle_h
#define TruthParticle_h

/** simulator includes **/
#include "CaloCell/CaloCell.h"
#include "CaloCluster/CaloCluster.h"
#include "core/macros.h"

/** geant 4 includes **/
#include "globals.hh"

/** standard includes **/
#include <vector>

// Event Object Data
namespace xAOD{
  
  class TruthParticle
  {  
    public:

      /*! Contructor */
      TruthParticle();
      /*! Contructor */
      TruthParticle( float eta, float phi, int pdgid );
      /*! Destructor */
      ~TruthParticle();
     
      /** The eta center of the cluster given by the simulation (RoI) **/ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /** The phi center of the cluster given by the simulation (RoI) **/ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /** set/get transverse energy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );
      /** set/get total energy in all layer **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_etot, setEtot, etot );
      /** set/get max energy (first eletromagnetic layer) **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_emaxs1, setEmaxs1, emaxs1 );
      /** set/get max energy (second eletromagnetic layer) **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_emaxs2, setEmaxs2, emaxs2 );
      /**  get second maximum energy in sampling 1 (strip layer) **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_e2tsts1, setE2tsts1, e2tsts1 );
      /** set/get eratio **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_eratio, setEratio, eratio );
      /** set/get reta **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_reta, setReta, reta );
      /** set/get rphi **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_rphi, setRphi, rphi );
      /** set/get f1 **/ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_f1, setF1, f1 );
      /** set/get f1 **/ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_f2, setF2, f2 );
      /** set/get f3 **/ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_f3, setF3, f3 );
      /** set/get rhad **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_rhad, setRhad, rhad );
      /** set/get particle identification **/
      PRIMITIVE_SETTER_AND_GETTER( int, m_pdgid, setPdgid, pdgid );


      /** Add the calorimeter cell into the RoI container **/
      void push_back( const xAOD::CaloCell * );
      /** Return the number of cells inside of this cluster **/
      size_t size();
      /** release all memory holded by the cell container into this RoI **/
      void clear();
      /** Get all cells **/
      std::vector<const xAOD::CaloCell*> allCells();
      /** Print cluster information **/
      void Print();


      void setCaloCluster( const xAOD::CaloCluster *clus ){ m_caloCluster=clus; };
      const xAOD::CaloCluster* caloCluster() const { return m_caloCluster; };

    private:

      int m_pdgid;
      float m_et;
      float m_etot;
      float m_eta;
      float m_phi;
      float m_emaxs1;
      float m_emaxs2;
      float m_e2tsts1;
      float m_reta;
      float m_rphi;
      float m_rhad;
      float m_eratio;
      float m_f1;
      float m_f2;
      float m_f3;
   
      const xAOD::CaloCluster *m_caloCluster;

      /* All calo cells into the roi window */
      std::vector<const xAOD::CaloCell*> m_container;
  };
}
#endif
