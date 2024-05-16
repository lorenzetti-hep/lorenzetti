#ifndef Electron_h
#define Electron_h

#include "CaloCluster/CaloCluster.h"
#include "CaloCell/CaloCell.h"
#include "GaugiKernel/EDM.h"
#include "GaugiKernel/macros.h"
#include <cmath>

namespace xAOD{
  
  class Electron: public Gaugi::EDM
  {  
    public:
      
      /*! Contructor */
      Electron();
      /*! Contructor */
      /*! Destructor */
      ~Electron()=default;
     
      /*! The eta center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta, setEta, eta );
      /*! The phi center of the cluster given by the simulation (RoI) */ 
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi, setPhi, phi );
      /*! set/get energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_e, setE, e );
      /*! set/get transverse energy */
      PRIMITIVE_SETTER_AND_GETTER( float, m_et, setEt, et );

      void setDecisions (std::vector<bool> decisions ){ for (auto dec : decisions) m_decisions.push_back(dec); }
      std::vector<bool> getDecisions() const {return m_decisions;}
      
      void setCaloCluster( const xAOD::CaloCluster *clus ){ m_caloCluster=clus; };
      const xAOD::CaloCluster* caloCluster() const { return m_caloCluster; };

    private:
      float m_e;
      float m_et;
      float m_eta;
      float m_phi;

      std::vector<bool> m_decisions;

      const xAOD::CaloCluster *m_caloCluster;
  };
}
#endif