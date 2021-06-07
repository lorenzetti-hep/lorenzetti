/*
  Copyright (C) 2020-2020
*/

#ifndef IAlgService_h
#define IAlgService_h

#include "GaugiKernel/DataHandle.h"
#include "GaugiKernel/StoreGate.h"
#include "GaugiKernel/StatusCode.h"
#include "GaugiKernel/Property.h"
#include "GaugiKernel/MsgStream.h"

#include "G4Step.hh"

/*
 * @file IAlgorithm.h
 * @brief Designed to be the interface class of all reconstruction algorithms
 * @author joao victor pinto (jodafons@cern.ch)
 */


/// namespace
namespace Gaugi{

  /* 
   * @class IAlgorithm
   * @brief Designed to be the interface class of all reconstruction algorithms
   */
  class IAlgorithm
  {  
    public:
  
    /*! Destructor */
    virtual ~IAlgorithm() {};

    /*! Create all resouces here */    
    virtual StatusCode initialize()=0;
  
    /*! Book all histograms into the current store gate */
    virtual StatusCode bookHistograms( SG::EventContext &/*sg*/ ) const=0;

    /*! This step will be executed before Geant step action */
    virtual StatusCode pre_execute( SG::EventContext & /*ctx*/ ) const=0;
  
    /*! This step will be executed during the Geant step action */
    virtual StatusCode execute( SG::EventContext &/*ctx*/, const G4Step * ) const=0;
  
    /*! This step will be executed in standalone mode (no geant) */
    virtual StatusCode execute( SG::EventContext &/*ctx*/, int /*evt*/ ) const=0;
  
    /*! This step will be executed after the Geant step action */
    virtual StatusCode post_execute( SG::EventContext & /*ctx*/ ) const=0;
  
    /*! Fill all histograms into the current store gate */
    virtual StatusCode fillHistograms( SG::EventContext & /*ctx*/ )const =0;
  
    /*! Destroy all allocated memory and close all services */
    virtual StatusCode finalize()=0;
  
  };

}// namespace
#endif
