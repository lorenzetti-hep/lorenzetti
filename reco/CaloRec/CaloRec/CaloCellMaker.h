#ifndef CaloCellMaker_h
#define CaloCellMaker_h

/** Simulator includes **/
#include "core/macros.h"
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
    CaloCellMaker( std::string );
    /** Destructor **/
    ~CaloCellMaker();
    
    
    /** initialize the object **/
    virtual StatusCode initialize() override;
    /** Execute in step action step from geant core **/
    virtual StatusCode execute( EventContext *ctx ) override;
    /** execute before start the step action **/
    virtual StatusCode pre_execute( EventContext *ctx ) override;
    /** override to avoid abstract python import problem **/ 
    virtual StatusCode post_execute( EventContext * ) override{return SUCCESS;};
    /** fill hisogram in the end **/
    virtual StatusCode fill( EventContext *ctx ) override;
    /** override to avoid abstract python import problem **/ 
    virtual StatusCode finalize() override {return SUCCESS;};



    PRIMITIVE_SETTER_AND_GETTER( std::string, m_card, setCard, card );
    PRIMITIVE_SETTER_AND_GETTER( int        , m_bc_id_start   , bc_id_start, set_bc_id_start );
    PRIMITIVE_SETTER_AND_GETTER( int        , m_bc_nsamples   , bc_nsamples, set_bc_nsamples );
    PRIMITIVE_SETTER_AND_GETTER( float      , m_bc_duration   , bc_duration, set_bc_duration );


  private:
    
    // Detector card (granularity configuration)
    std::string m_card;
    // bunch crossing id start
    int m_bc_id_start;
    // number of samples per bunch
    int m_bc_nsamples;
    // buncg crossing time duration (in ns)
    float m_bc_duration;


};

#endif
