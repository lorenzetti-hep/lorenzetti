#ifndef FieldSetup_h
#define FieldSetup_h 

#include "GaugiKernel/MsgStream.h"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"

class G4FieldManager;
class G4ChordFinder;
class G4Mag_UsualEqRhs;
class G4MagIntegratorStepper;
class G4VIntegrationDriver;



class FieldSetup: public MsgService
{
  public:
    FieldSetup(G4ThreeVector, G4int  stepperNum = -1000, G4bool useFSALstepper= false );
  
    FieldSetup();
    
    FieldSetup( FieldSetup & ) = delete; 
  
    virtual ~FieldSetup();
  
    void SetStepperType( G4int i ) { m_stepperType = i; CreateStepperAndChordFinder(); }
  
    void SetStepper();
  
    void SetMinStep(G4double s) { m_minStep = s; }
  
    void InitialiseAll();
  
    void CreateStepperAndChordFinder();
  
    void CreateFSALStepperAndChordFinder(); 
  
    void SetFieldValue(G4ThreeVector fieldVector);
    
    void SetFieldZValue(G4double fieldValue);

    G4ThreeVector GetConstantFieldValue();
  
    void   SetUseFSALstepper(G4bool val= true) { m_useFSALstepper = val; }
    
    G4bool GetUseFSALstepper()                 { return m_useFSALstepper; }
     
  protected:
     // Implementation methods
    G4VIntegrationDriver* CreateFSALStepperAndDriver();
  
     // Find the global Field Manager
    G4FieldManager*          GetGlobalFieldManager();
     
  
    G4FieldManager*          m_fieldManager = nullptr;
    G4ChordFinder*           m_chordFinder = nullptr;
    G4Mag_UsualEqRhs*        m_equation = nullptr;
    G4MagneticField*         m_magneticField = nullptr;
    G4MagIntegratorStepper*  m_stepper = nullptr;
    G4bool                   m_useFSALstepper = false;
    G4VIntegrationDriver*    m_driver =  nullptr;
    G4int                    m_stepperType = -1;
    G4double                 m_minStep = -1.0;

};

#endif
