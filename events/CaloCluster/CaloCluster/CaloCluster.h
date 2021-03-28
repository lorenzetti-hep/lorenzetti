#ifndef CaloCluster_h
#define CaloCluster_h


/** simulator includes **/
#include "CaloCell/CaloCell.h"
#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"

/** geant 4 includes **/
#include "globals.hh"

/** standard includes **/
#include <vector>


// Event Object Data
namespace xAOD{
  
  class CaloCluster: public Gaugi::EDM
  {  
    public:
      
      /*! Contructor */
      CaloCluster();
      /*! Contructor */
      CaloCluster( float emaxs2, float eta, float phi, float deta, float dphi );
      /*! Destructor */
      ~CaloCluster();
     
      /*! The eta center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! The phi center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! The RoI eta window */ 
			PRIMITIVE_SETTER_AND_GETTER( float, m_deta, setDeltaEta, deltaEta );
      /*! The RoI phi window */ 
			PRIMITIVE_SETTER_AND_GETTER( float, m_dphi, setDeltaPhi, deltaPhi );
      /*! set/get energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e, setE, e );
      /*! set/get transverse energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );
      /*! set/get total energy in first eletromagnetic layer */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e0, setE0, e0 );
      /*! set/get total energy in pre-sampler */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e1, setE1, e1 );
      /*! set/get total energy in second eletromagnetic layer */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e2, setE2, e2 );
      /*! set/get total energy in third eletromagnetic layer */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e3, setE3, e3 );
      /*! set/get hadronic Energy (first hadronic layer) */
      PRIMITIVE_SETTER_AND_GETTER( float, m_ehad1, setEhad1, ehad1 );
      /*! set/get hadronic Energy (second hadronic layer) */
      PRIMITIVE_SETTER_AND_GETTER( float, m_ehad2, setEhad2, ehad2 );
      /*! set/get hadronic Energy (third hadronic layer) */
      PRIMITIVE_SETTER_AND_GETTER( float, m_ehad3, setEhad3, ehad3 );
      /*! set/get total energy in all layer */
      PRIMITIVE_SETTER_AND_GETTER( float, m_etot, setEtot, etot );
      /*! set/get max energy (first eletromagnetic layer) */
      PRIMITIVE_SETTER_AND_GETTER( float, m_emaxs1, setEmaxs1, emaxs1 );
      /*! set/get max energy (second eletromagnetic layer) */
      PRIMITIVE_SETTER_AND_GETTER( float, m_emaxs2, setEmaxs2, emaxs2 );
      /*!  get second maximum energy in sampling 1 (strip layer) */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e2tsts1, setE2tsts1, e2tsts1 );
      /*! set/get Energy in a 3x7 cluster (no calibration) around hottest cell */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e237, setE237, e237 );
      /*! set/get Energy in a 3x3 cluster (no calibration) around hottest cell */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e233, setE233, e233 );
      /*! set/get Energy in a 7x7 cluster (no calibration) around hottest cell */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e277, setE277, e277 );
      /*! set/get eratio */
      PRIMITIVE_SETTER_AND_GETTER( float, m_eratio, setEratio, eratio );
      /*! set/get reta */
      PRIMITIVE_SETTER_AND_GETTER( float, m_reta, setReta, reta );
      /*! set/get rphi */
      PRIMITIVE_SETTER_AND_GETTER( float, m_rphi, setRphi, rphi );
      /*! set/get f0 */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_f0, setF0, f0 );
      /*! set/get f1 */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_f1, setF1, f1 );
      /*! set/get f2 */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_f2, setF2, f2 );
      /*! set/get f3 */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_f3, setF3, f3 );
      /*! set/get rhad */
      PRIMITIVE_SETTER_AND_GETTER( float, m_rhad, setRhad, rhad );
      /*! set/get rhad1 */
      PRIMITIVE_SETTER_AND_GETTER( float, m_rhad1, setRhad1, rhad1 );
      /*! Weta2 */
      PRIMITIVE_SETTER_AND_GETTER( float, m_weta2, setWeta2, weta2 );


      /*! Add the calorimeter cell into the RoI collection */
      void push_back( const xAOD::CaloCell * );
      /*! Return the number of cells inside of this cluster */
      size_t size();
      /*! release all memory holded by the cell collection into this RoI */
      void clear();
      /*! Get all cells **/
      const std::vector<const xAOD::CaloCell*>& cells() const;


    private:

      float m_e;
      float m_et;
      float m_e0;
      float m_e1;
      float m_e2;
      float m_e3;
      float m_ehad1;
      float m_ehad2;
      float m_ehad3;
      float m_etot;
      float m_e233;
      float m_e237;
      float m_e277;
      float m_emaxs1;
      float m_emaxs2;
      float m_e2tsts1;
      float m_reta;
      float m_rphi;
      float m_rhad;
      float m_rhad1;
      float m_eratio;
      float m_f0;
      float m_f1;
      float m_f2;
      float m_f3;
      float m_weta2;
      float m_eta;
      float m_phi;
      float m_deta;
      float m_dphi;

      
      /* All calo cells into the roi window */
      std::vector<const xAOD::CaloCell*> m_container;
  };
}
#endif
