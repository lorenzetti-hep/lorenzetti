#include "FieldSetup.h"

#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4ConstRK4.hh"
#include "G4NystromRK4.hh"
#include "G4HelixMixedStepper.hh"
#include "G4ExactHelixStepper.hh"

#include "G4BogackiShampine23.hh"
#include "G4BogackiShampine45.hh"
#include "G4DormandPrince745.hh"
#include "G4DormandPrinceRK56.hh"
#include "G4DormandPrinceRK78.hh"
#include "G4TsitourasRK45.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4VIntegrationDriver.hh"
#include "G4FSALIntegrationDriver.hh"
#include "G4RK547FEq1.hh"
#include "G4RK547FEq2.hh"
#include "G4RK547FEq3.hh"


enum EStepperNumber { kDormandPrince45 = 17, kBogackiShampine45= 45, kClassicalRK4 = 4,
                      kNystromRK4 = 13 /*soon 40*/,
                      kDormandPrince56 = 56, kBogackiShampine23= 23, kCashKarp = 8,
                      kDormandPrince78 = 78, kTsitouras45 = 145
} ;


FieldSetup::FieldSetup(G4ThreeVector fieldVector, G4int stepperNum, G4bool useFSALstepper )
 : IMsgService("FieldSetup"),
   m_magneticField(new G4UniformMagField(fieldVector)),
   m_useFSALstepper(useFSALstepper),
   m_stepperType(0),
   m_minStep(0.)
{
  MSG_INFO( " FieldSetup: magnetic field set to Uniform( " << fieldVector << " ) " );

  /*
  if( stepperNum == -1000 )
  {
     m_useFSALstepper = useFSALstepper;
     if( !useFSALstepper )
        m_stepperType = 17;   // Use Dormand Prince (7) 4/5 as default stepper
     else
        m_stepperType = 101;
  }
  else
  {
     m_useFSALstepper = ( stepperNum > 0 );
     if( stepperNum > 0 )
        m_stepperType = stepperNum;
     else
        m_stepperType = - stepperNum;        
  }
  */

  m_stepperType = stepperNum;
  m_useFSALstepper = useFSALstepper;
  MSG_INFO( "Stepper type   : " << m_stepperType );
  MSG_INFO( "UseFSALstepper : " << m_useFSALstepper );
  InitialiseAll();
}


FieldSetup::FieldSetup(): 
   IMsgService("FieldSetup"),
   m_magneticField(new G4UniformMagField(G4ThreeVector())),
   m_useFSALstepper(false),
   m_stepperType(17),   // Use Dormand Prince (7) 4/5 as default stepper
   m_minStep(0.)
{
  MSG_INFO("FieldSetup: magnetic field set to Uniform( 0.0, 0, 0 ) ");
  InitialiseAll();
}


void FieldSetup::InitialiseAll()
{
 
  m_equation = new G4Mag_UsualEqRhs(m_magneticField);
 
  //m_minStep     = 1.0*mm; // minimal step of 1 mm is default
  m_minStep     = 2.0*mm; // minimal step of 1 mm is default

  m_fieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  if( m_useFSALstepper ) {
    CreateFSALStepperAndChordFinder();
  }else{
    CreateStepperAndChordFinder();
  }

  MSG_INFO( "AKI JOAO = " << m_chordFinder->GetIntegrationDriver()->GetMaxNoSteop() );
  MSG_INFO( "Updating Field Manager..." );
  m_fieldManager->SetChordFinder( m_chordFinder );
  m_fieldManager->SetDetectorField(m_magneticField );


  MSG_INFO( "1 = " << m_fieldManager->GetMinimumEpsilonStep() );
  MSG_INFO( "2 = " << m_fieldManager->GetMaximumEpsilonStep() );

}



FieldSetup::~FieldSetup()
{
  MSG_INFO( "Delete the magnetic field! ");
  delete m_magneticField;
  delete m_chordFinder;
  delete m_stepper;
}



void FieldSetup::CreateStepperAndChordFinder()
{
  MSG_INFO( "CreateStepperAndChordFinder called... ");

  SetStepper();

  m_chordFinder = new G4ChordFinder( m_magneticField, m_minStep,m_stepper );
  m_fieldManager->SetChordFinder( m_chordFinder );
  m_fieldManager->SetDetectorField( m_magneticField );
}



void FieldSetup::CreateFSALStepperAndChordFinder()
{
  MSG_INFO( "CreateFSALStepperAndChordFinder called... " );
  m_driver = CreateFSALStepperAndDriver();
  m_chordFinder = new G4ChordFinder( m_driver );
}




void FieldSetup::SetStepper()
{


  MSG_INFO( "Setting Stepper with type: " << m_stepperType );
  switch ( m_stepperType )
  {
     //  The new default in G4 and here ( since G4 10.4 Dec 2017 )
    case 17:      
    case 457:
    case 745:
      m_stepper = new G4DormandPrince745( m_equation );
      MSG_INFO( "G4DormandPrince745 Stepper is chosen.");
      break;
     
    case 0:
      m_stepper = new G4ExplicitEuler( m_equation );
      MSG_INFO("G4ExplicitEuler is chosen.");
      break;
    case 1:
      m_stepper = new G4ImplicitEuler( m_equation );
      MSG_INFO("G4ImplicitEuler is chosen");
      break;
    case 2:
      m_stepper = new G4SimpleRunge( m_equation );
      MSG_INFO("G4SimpleRunge is chosen");
      break;
    case 3:
      m_stepper = new G4SimpleHeum( m_equation );
      MSG_INFO("G4SimpleHeum is chosen");
      break;
    case 4:
      m_stepper = new G4ClassicalRK4( m_equation );
      MSG_INFO("G4ClassicalRK4 (default) is chosen");
      break;
    case 5:
      m_stepper = new G4HelixExplicitEuler( m_equation );
      MSG_INFO("G4HelixExplicitEuler is chosen");
      break;
    case 6:
      m_stepper = new G4HelixImplicitEuler( m_equation );
      MSG_INFO("G4HelixImplicitEuler is chosen");
      break;
    case 7:
      m_stepper = new G4HelixSimpleRunge( m_equation );
      MSG_INFO("G4HelixSimpleRunge is chosen");
      break;
    case 8:
      m_stepper = new G4CashKarpRKF45( m_equation );
      MSG_INFO("G4CashKarpRKF45 is chosen");
      break;
    case 9:
      m_stepper = new G4RKG3_Stepper( m_equation );
      MSG_INFO("G4RKG3_Stepper is chosen");
      break;
    case 10: 
       m_stepper = new G4ExactHelixStepper( m_equation );   
       MSG_INFO("G4ExactHelixStepper is chosen");
       break;
    case 11: 
       m_stepper = new G4HelixMixedStepper( m_equation );  
       MSG_INFO("G4HelixMixedStepper is chosen");
       break;
    case 12: 
       m_stepper = new G4ConstRK4( m_equation ); 
       MSG_INFO("G4ConstRK4 Stepper is chosen");
       break;
    case 13:
    case 40:
      m_stepper = new G4NystromRK4( m_equation );
      MSG_INFO(" G4NystromRK4 Stepper is chosen");
      break;
    case 14:      
    case 23:
      m_stepper = new G4BogackiShampine23( m_equation );
      MSG_INFO("G4BogackiShampine23 Stepper is chosen");
      break;

      // Other optimised 4/5th order embedded drivers
    case 15:
    case 45:       
      m_stepper = new G4BogackiShampine45( m_equation );
      MSG_INFO("G4BogackiShampine45 Stepper is chosen");
      break;

    // case 145:
    case kTsitouras45:     
      m_stepper = new G4TsitourasRK45( m_equation );
      MSG_INFO("G4TsitourasRK45 Stepper is chosen");
      break;      

      // Higher order embedded drivers - for very smooth fields
    case 56:
      m_stepper = new G4DormandPrinceRK56( m_equation );
      MSG_INFO("G4DormandPrinceRK56 Stepper is chosen");
      break;
    case 78:
      m_stepper = new G4DormandPrinceRK78( m_equation );
      MSG_INFO("G4DormandPrinceRK78 Stepper is chosen");
      break;

    default:
      // m_stepper = new G4ClassicalRK4( m_equation );
      // MSG_INFO("G4ClassicalRK4 Stepper (default) is chosen");
      m_stepper = new G4DormandPrince745( m_equation );
      MSG_INFO("G4DormandPrince745 (default) Stepper is chosen");
      break;      
  }
}



G4VIntegrationDriver* FieldSetup::CreateFSALStepperAndDriver()
{
  
  MSG_INFO( "CreateFSALStepperAndDriver called...." );   

  G4VIntegrationDriver* fsalDriver = nullptr;
  
  switch ( m_stepperType )
  {
    case   1:
    case 101:
       MSG_INFO( " Stepper type '1' is G4RK547FEq1 stepper (in FSAL mode) with FSAL driver. ");
       fsalDriver = new G4FSALIntegrationDriver<G4RK547FEq1>( m_minStep, new G4RK547FEq1( m_equation ) );
       break;
     
    case   2:
    case 102:
       MSG_INFO( " Stepper type '2' is G4RK547FEq2 stepper (in FSAL mode) with FSAL driver. ");
       fsalDriver = new G4FSALIntegrationDriver<G4RK547FEq2>( m_minStep, new G4RK547FEq2( m_equation ) );
       break;
       
    case   3:
    case 103:
       MSG_INFO( " Stepper type '3' is G4RK547FEq3 stepper (in FSAL mode) with FSAL driver. ");       
       fsalDriver = new G4FSALIntegrationDriver<G4RK547FEq3>( m_minStep, new G4RK547FEq3( m_equation ) );
       break;

    default:

       MSG_INFO( "Setting default..." );
       fsalDriver = new G4FSALIntegrationDriver<G4RK547FEq1>( m_minStep, new G4RK547FEq1( m_equation ) );
       break;
  }

 C
  
  m_stepper = fsalDriver->GetStepper();
  return fsalDriver;
}









void FieldSetup::SetFieldZValue( G4double fieldStrength)
{
  SetFieldValue(G4ThreeVector(0, 0, fieldStrength));
}


void FieldSetup::SetFieldValue(G4ThreeVector fieldVector)
{
  if (m_magneticField) delete m_magneticField;
 
  if (fieldVector != G4ThreeVector(0.,0.,0.))
  {
    m_magneticField = new G4UniformMagField(fieldVector);
  }else{
    m_magneticField = 0;
  }

  GetGlobalFieldManager()->SetDetectorField(m_magneticField);

  m_equation->SetFieldObj( m_magneticField );

}


G4FieldManager* FieldSetup::GetGlobalFieldManager()
{
  return G4TransportationManager::GetTransportationManager()->GetFieldManager();
}

