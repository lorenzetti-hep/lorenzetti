#ifndef Truth_h
#define Truth_h

/** simulator includes **/
#include "CaloCluster/CaloCluster.h"
#include "core/macros.h"

/** geant 4 includes **/
#include "globals.hh"

/** standard includes **/
#include <vector>

// Event Object Data
namespace xAOD{
  
  /// xAOD EDM.
  /// @author Joao victor da Fonseca Pinto <jodafons@cern.ch, jodafons@lps.ufrj.br>
  ///
	/// $Revision: 1 $
  /// $Date: 2020-02-01$
  ///
  class Truth
  {  
    public:

      /*! Contructor */
      Truth();
      /*! Contructor */
      Truth( float energy, float eta, float phi, int pdgid );
      /*! Destructor */
      ~Truth();
      
      /*! Eta particle  position */
      PRIMITIVE_SETTER_AND_GETTER( float, m_eta     , setEta    , eta         );
      /*! Phi particle position */
      PRIMITIVE_SETTER_AND_GETTER( float, m_phi     , setPhi    , phi         );
      /*! Particle energy in MeV */
      PRIMITIVE_SETTER_AND_GETTER( float, m_energy  , setEnergy , energy      );
      /*! Particle identification */
      PRIMITIVE_SETTER_AND_GETTER( int  , m_pdgid   , setPdgid  , pdgid       );
      /*! Link the truth object with the associated cluster */ 
      PRIMITIVE_SETTER_AND_GETTER( const xAOD::CaloCluster*, setCaloCluster, caloCluster );

    private:

      // Energy
      float m_energy;
      // Eta
      float m_eta;
      // Phi
      float m_phi;
      // Particle identification
      int m_pdgid;
      // Cluster
      const xAOD::CaloCluster *m_cluster;
  };
}
#endif
