#ifndef CaloClusterContainer_h
#define CaloClusterContainer_h

/** simulator includes **/
#include "CaloCluster/CaloCluster.h"

/** standard includes **/
#include <vector>


namespace xAOD{
  /// Description of a calorimeter cluster
  ///
  /// This is a first stab at how a calorimeter cell could behave in the
  /// xAOD EDM.
  /// @author Joao victor da Fonseca Pinto <jodafons@cern.ch, jodafons@lps.ufrj.br>
  ///
	/// $Revision: 1 $
  /// $Date: 2020-02-01$
  ///
 
  class CaloClusterContainer{
    
    public:
      /** Constructor **/
      CaloClusterContainer();
      /** Destructor **/
      ~CaloClusterContainer();
      /** Return the number of cluster (RoI) into this container **/
      size_t size();
      /** Clear and destroy all clusters **/
      void clear(); 
      /** Add a new cluster into the container **/
      void push_back( xAOD::CaloCluster * );

    private:

      // Container vector
      std::vector< xAOD::CaloCluster* > m_container;

  };
}
#endif
