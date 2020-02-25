#ifndef CaloCellMaker_h
#define CaloCellMaker_h

/** Simulator includes **/
#include "core/AlgTool.h"
#include "core/EventContext.h"
#include "core/macros.h"
#include "CaloCell/CaloCell.h"
#include "CaloCell/CaloCellCollection.h"

/** Geant 4 includes **/
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

/** Standard includes **/
#include <vector>
#include <string>


class CaloCellMaker : public AlgTool
{
  public:
  
    /** Contructor **/
    CaloCellMaker( std::string & );
    /** Destructor **/
    ~CaloCellMaker();
    /** initialize the object **/
    virtual StatusCode initialize() override;
    /** Execute in step action step from geant core **/
    virtual StatusCode execute( EventContext *ctx ) override;
    /** execute before start the step action **/
    virtual StatusCode pre_execute( EventContext *ctx ) override;
    /** fill hisogram in the end **/
    virtual StatusCode fill( EventContext *ctx ) override;


  private:
    // Detector card (granularity configuration)
    std::string m_card;
    // bunch crossing id start
    float m_bc_id_start;
    // buncg crossing time duration (in ns)
    float m_bc_duration;
    // number of samples per bunch
    float m_bc_nsamples;
};

#endif
