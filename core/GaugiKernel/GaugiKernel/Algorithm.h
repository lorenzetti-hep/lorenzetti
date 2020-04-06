/*
  Copyright (C) 2020-2020
*/

#ifndef Algorithm_h
#define Algorithm_h

/*
 * @file Algorithm.h
 * @brief Designed to be the base class of all reconstruction algorithms
 * @author joao victor pinto (jodafons@cern.ch)
 */

#include "GaugiKernel/IAlgorithm.h"
#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"


/// Gaugi namespace
namespace Gaugi{

  /* 
   * @class Algorithm
   * @brief Designed to be the base class of all reconstruction algorithms
   *        used into the calorimeter simulator core.  The main advantage
   *        to use Algorithms is the support to main services as MsgStream,
   *        Property and StoreGate. The MsgStream is used for log output,
   *        Properties can be created into the class constructor and externa
   *        lised to python config and the StoreGate, usually used to store
   *        histograms, can be retrieved and time during the execution.
   */
  class Algorithm  : public IAlgorithm, 
                     public MsgService, 
                     public PropertyService
  {
    public:

      /*! Constructor */
      Algorithm( std::string &name );
      
      /*! Destructor */
      virtual ~Algorithm() {};
      
      /*! Return the name of the algorithm */
      const std::string& name() const;

      /*! set the store gate service */
      void setStoreGateSvc( SG::StoreGate * );
    
    protected:
      
      /*! get the monitoring tool */
      SG::StoreGate* getStoreGateSvc() const;
  
      /*! Level message */
      int m_outputLevel;

    private:
     
      /*! monitoring tool */
      SG::StoreGate *m_store;  

      
  };

}/// namespace
#endif
