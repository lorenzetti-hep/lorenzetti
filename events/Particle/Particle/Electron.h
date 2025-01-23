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

      bool isTight() const;
      bool isMedium() const;
      bool isLoose() const;
      bool isVeryLoose() const;
      void setIsTight(bool isTight);
      void setIsMedium(bool isMedium);
      void setIsLoose(bool isLoose);
      void setIsVeryLoose(bool isVeryLoose);
      bool isForward() const { return std::abs(m_eta) > 2.5; };

    private:
      float m_e;
      float m_et;
      float m_eta;
      float m_phi;

      std::vector<bool> m_isEM;

      const xAOD::CaloCluster *m_caloCluster;
  };
}
#endif