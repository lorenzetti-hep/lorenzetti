#ifndef CaloCell_h
#define CaloCell_h

/** simulator includes **/
#include "CaloCell/enumeration.h"
#include "CaloCell/CaloDetDescriptor.h"
#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"


namespace xAOD{
  
  class CaloCell: public Gaugi::EDM
  {  
    public:

      /** Constructor **/
      CaloCell()=default;
      
      /** Contructor **/
      CaloCell( float eta, float phi, float deta, float dphi);

      /** Destructor **/
      ~CaloCell()=default;
      /*! Cell eta center */
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! Cell phi center */
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! Cell delta eta */
      PRIMITIVE_SETTER_AND_GETTER( float, m_deta, setDeltaEta , deltaEta);
      /*! Cell delta phi */
      PRIMITIVE_SETTER_AND_GETTER( float, m_dphi, setDeltaPhi, deltaPhi );
      /*! Cell energy **/
      PRIMITIVE_SETTER_AND_GETTER( float, m_e, setE, e );
      /*! Tranverse energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );


      /*! Get the associated Raw information */ 
      const xAOD::CaloDetDescriptor* descriptor() const;
      /*! Set the associated Raw information */ 
      void setDescriptor( const xAOD::CaloDetDescriptor* );


    private:
 
      /*! eta center */
      float m_eta;
      /*! phi center */
      float m_phi;
      /*! delta eta */
      float m_deta;
      /*! delta phi */
      float m_dphi;
      /*! Energy */
      float m_e;
      /*! Transverse energy*/
      float m_et;

      /*! Associated raw information */
      const xAOD::CaloDetDescriptor *m_descriptor;
  };

}
#endif
