#ifndef CaloCellMaker_h
#define CaloCellMaker_h

/** Simulator includes **/
#include "core/macros.h"
#include "core/AlgTool.h"
#include "core/EventContext.h"
#include "core/macros.h"
#include "CaloCell/CaloCell.h"
#include "CaloCell/CaloCellCollection.h"

/** Pulse generator **/
#include "TPulseGenerator.h"

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
    
    
    /** initialize the algorithm **/
    virtual StatusCode initialize() override;
    /** Execute in step action step from geant core **/
    virtual StatusCode execute( EventContext *ctx ) override;
    /** execute before start the step action **/
    virtual StatusCode pre_execute( EventContext *ctx ) override;
    /** execute after the step action **/ 
    virtual StatusCode post_execute( EventContext *ctx ) override;
    /** fill hisogram in the end **/
    virtual StatusCode fillHistograms( EventContext *ctx ) override;
    /** finalize the algorithm **/ 
    virtual StatusCode finalize() override;



    PRIMITIVE_SETTER_AND_GETTER( std::string, m_calibPath, SetCalibPath, GetCalibPath);



  private:
   
    xAOD::CaloCellCollection m_collection;

    void GeneratePulse( xAOD::CaloCell * );
    void CalculateEnergy( xAOD::CaloCell * );

    
    std::string m_inputKey;
    std::string m_outputKey;
 
    // bunch crossing id start
    int m_bc_id_start;
    // bunch crossing id end
    int m_bc_id_end;
    // number of samples per bunch
    int m_bc_nsamples;
    // bunch crossing time duration (in ns)
    float m_bc_duration;
    
    std::string m_calibPath;

    
    std::map<CaloSampling::CaloLayer, CPK::TPulseGenerator*> m_pulseGenerator;
};



#endif
