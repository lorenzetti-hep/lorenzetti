
#include "DetectorATLASConstruction.h"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include <string>
#include <sstream>




DetectorATLASConstruction::DetectorATLASConstruction(std::string name)
 : 
  IMsgService(name), 
   G4VUserDetectorConstruction(),
   m_checkOverlaps(true)
{
  declareProperty( "UseMagneticField", m_useMagneticField=true );
  MSG_INFO( "DetectorContruction was created" );
}


DetectorATLASConstruction::~DetectorATLASConstruction()
{;}


G4VPhysicalVolume* DetectorATLASConstruction::Construct()
{
  // Define materials
  DefineMaterials();
  // Define volumes
  return DefineVolumes();
}


void DetectorATLASConstruction::DefineMaterials()
{
  // Lead material defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
  nistManager->FindOrBuildMaterial("G4_Fe");
  nistManager->FindOrBuildMaterial("G4_Al");
  nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  nistManager->FindOrBuildMaterial("PLASTIC SCINTILLATOR");
  G4Element* elH = new G4Element("Hydrogen", "H", 1., 1.0079 * g/mole);
  G4Element* elC = new G4Element("Carbon", "C", 6., 12.011 * g/mole);

  //density = 1.032*g/cm3;
  //G4Material* scinti = new G4Material(name="PLASTIC SCINTILLATOR", density, nel=2);
  //scinti->AddElement(elC, 9);
  //scinti->AddElement(elH, 10);
  G4Material* scinti = new G4Material("PLASTIC SCINTILLATOR", 1.032 * g/cm3, 2); // Organic plastic scintillator, polystyrene
  scinti->AddElement(elH, 8.5*perCent);
  scinti->AddElement(elC, 91.5*perCent);
  //G4Material* scinti = new G4Material("Polystyrene", density= 1.032*g/cm3, ncomponents=2);
  //scinti->AddElement(C, natoms=1);
  //scinti->AddElement(H, natoms=1);

  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;
  G4double density;
  // The argon by NIST Manager is a gas with a different density
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}


G4VPhysicalVolume* DetectorATLASConstruction::DefineVolumes()
{


  // Get materials
  G4Material* defaultMaterial  = G4Material::GetMaterial("Galactic");


 

  //
  // World
  //
  G4VSolid* worldS = new G4Tubs( "World",     // its name
                                 0,           // R min
                                 5*m,         // R max
                                 10*m,        // Z max
                                 0*deg,       // phi_min
                                 360*deg      // phi_max
                                 );

  G4LogicalVolume* worldLV = new G4LogicalVolume(
                                 worldS,           // its solid
                                 defaultMaterial,  // its material
                                 "World");         // its name

  G4VPhysicalVolume* worldPV = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 m_checkOverlaps);  // checking overlaps




  // Create a region
  G4Region* deadMatBeforeCal = new G4Region("DeadMatBeforeECal");
  //
  // Barrel
  //
  CreateBarrel( worldLV,
                "DeadMaterial",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Al"), // absorber
                G4Material::GetMaterial("Galactic"), // gap
                2, // ATLAS-like
                7.7*cm, // abso
                10*cm, // gap
                110.*cm, // start radio,
                6.8*m ,// z
                G4ThreeVector(0,0,0),
                deadMatBeforeCal );

  G4Region* presample = new G4Region("PS");

  // Create Pre-sampler calorimeter
  CreateBarrel( worldLV,
                "PS",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("Galactic"), // absorber
                G4Material::GetMaterial("liquidArgon"), // gap
                1, // layers
                0.01*mm, // abso
                1.1*cm, // gap
                146.*cm, // start radio,
                6.8*m ,// z
                G4ThreeVector(0,0,0),
                presample );

  G4Region* em1 = new G4Region("EM1");
  G4Region* em2 = new G4Region("EM2");
  G4Region* em3 = new G4Region("EM3");
  //G4Region* ecal= new G4Region("ECal");

  // Create Eletromagnetic calorimeter
  CreateBarrel( worldLV,
                "EM1",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Pb"), // absorber
                G4Material::GetMaterial("liquidArgon"), // gap
                16, // layers
                1.51*mm, // abso
                4.49*mm, // gap
                150.*cm, // start radio,
                6.8*m ,// z
                G4ThreeVector(0,0,0),
                em1);

  CreateBarrel( worldLV,
                "EM2",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Pb"), // absorber
                G4Material::GetMaterial("liquidArgon"), // gap
                55, // layers
                1.7*mm, // abso
                4.3*mm, // gap
                150.*cm + 9.6*cm, // start radio,
                6.8*m ,// z
                G4ThreeVector(0,0,0),
                em2);

  CreateBarrel( worldLV,
                "EM3",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Pb"), // absorber
                G4Material::GetMaterial("liquidArgon"), // gap
                9, // layers
                1.7*mm, // abso
                4.3*mm, // gap
                150.*cm + 9.6*cm + 33*cm, // start radio,
                6.8*m ,// z
                G4ThreeVector(0,0,0),
                em3);

  G4Region* deadMaterialBeforeHCal = new G4Region("DeadMaterialBeforeHCal");
  CreateBarrel( worldLV,
                "ECal_Boundary",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Pb"), // absorber
                G4Material::GetMaterial("Galactic"), // gap
                1, // layers
                10*cm, // abso
                3*mm, // gap
                198.3*cm, // start radio,
                6.8*m ,// z
                G4ThreeVector(0,0,0),
                deadMaterialBeforeHCal );

  CreateBarrel( worldLV,
                "Hcal_Boundary",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Pb"), // absorber
                G4Material::GetMaterial("Galactic"), // gap
                1, // layers
                10*cm, // abso
                3*mm, // gap
                218*cm, // start radio,
                6.8*m ,// z
                G4ThreeVector(0,0,0),
                deadMaterialBeforeHCal );


  G4Region* had1 = new G4Region("HAD1");
  G4Region* had2 = new G4Region("HAD2");
  G4Region* had3 = new G4Region("HAD3");

  // Create Hadronic calorimeter
  CreateBarrel( worldLV,
                "HAD1",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Fe"), // absorber
                G4Material::GetMaterial("PLASTIC SCINTILLATOR"), // gap
                4, // layers,
                6.0*cm, // abso
                4.0*cm, // gap
                228.3*cm, // start radio,
                6.8*m, // z
                G4ThreeVector(0,0,0),
                had1);
  CreateBarrel( worldLV,
                "HAD2",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Fe"), // absorber
                G4Material::GetMaterial("PLASTIC SCINTILLATOR"), // gap
                11, // layers,
                6.2*cm, // abso
                3.8*cm, // gap
                228.3*cm + 40*cm, // start radio,
                6.8*m, // z
                G4ThreeVector(0,0,0),
                had2);
  CreateBarrel( worldLV,
                "HAD3",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Fe"), // absorber
                G4Material::GetMaterial("PLASTIC SCINTILLATOR"), // gap
                5, // layers,
                6.2*cm, // abso
                3.8*cm, // gap
                228.3*cm + 40*cm + 110*cm, // start radio,
                6.8*m, // z
                G4ThreeVector(0,0,0),
                had3);

	const auto extended_barrel_size = 2.83*m;
  const auto extended_barrel_pos = 5.495*m;

  // Extended barrel
  CreateBarrel( worldLV,
                "HAD1_Extended",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Fe"), // absorber
                G4Material::GetMaterial("PLASTIC SCINTILLATOR"), // gap
                4, // layers,
                6.0*cm, // abso
                4.0*cm, // gap
                228.3*cm, // start radio,
                extended_barrel_size, // z
                G4ThreeVector(0,0,extended_barrel_pos),
                had1 );
  CreateBarrel( worldLV,
                "HAD2_Extended",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Fe"), // absorber
                G4Material::GetMaterial("PLASTIC SCINTILLATOR"), // gap
                11, // layers,
                6.2*cm, // abso
                3.8*cm, // gap
                228.3*cm + 40*cm, // start radio,
                extended_barrel_size, // z
                G4ThreeVector(0,0,extended_barrel_pos),
                had2 );
  CreateBarrel( worldLV,
                "HAD3_Extended",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Fe"), // absorber
                G4Material::GetMaterial("PLASTIC SCINTILLATOR"), // gap
                5, // layers,
                6.2*cm, // abso
                3.8*cm, // gap
                228.3*cm + 40*cm + 110*cm, // start radio,
                extended_barrel_size, // z
                G4ThreeVector(0,0,extended_barrel_pos),
                had3 );
  CreateBarrel( worldLV,
                "HAD1_Extended",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Fe"), // absorber
                G4Material::GetMaterial("PLASTIC SCINTILLATOR"), // gap
                4, // layers,
                6.0*cm, // abso
                4.0*cm, // gap
                228.3*cm, // start radio,
                extended_barrel_size, // z
                G4ThreeVector(0,0,-extended_barrel_pos),
                had1 );
  CreateBarrel( worldLV,
                "HAD2_Extended",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Fe"), // absorber
                G4Material::GetMaterial("PLASTIC SCINTILLATOR"), // gap
                11, // layers,
                6.2*cm, // abso
                3.8*cm, // gap
                228.3*cm + 40*cm, // start radio,
                extended_barrel_size, // z
                G4ThreeVector(0,0,-extended_barrel_pos),
                had2 );
  CreateBarrel( worldLV,
                "HAD3_Extended",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Fe"), // absorber
                G4Material::GetMaterial("PLASTIC SCINTILLATOR"), // gap
                5, // layers,
                6.2*cm, // abso
                3.8*cm, // gap
                228.3*cm + 40*cm + 110*cm, // start radio,
                extended_barrel_size, // z
                G4ThreeVector(0,0,-extended_barrel_pos),
                had3 );



  /*
  
  //
  // EndCap
  //
  // Create Eletromagnetic calorimeter
  CreateBarrel( worldLV,
                "ECal_EndCap_B",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Pb"), // absorber
                G4Material::GetMaterial("liquidArgon"), // gap
                200, // layers,
                2.*mm, // abso
                4.*mm, // gap
                28.*cm, // start radio,
                100.*cm, // z
                G4ThreeVector(0,0,3.*m + 100.*cm));
  // Create Hadronic calorimeter
  CreateBarrel( worldLV,
                "HCal_EndCap_B",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_CESIUM_IODIDE"), // absorber
                G4Material::GetMaterial("G4_Fe"), // gap
                5, // layers,
                10.*cm, // abso
                10.*cm, // gap
                148*cm, // start radio,
                1.*m, //z
                G4ThreeVector(0,0,3.*m + 100.*cm));



  // Create Eletromagnetic calorimeter
  CreateBarrel( worldLV,
                "ECal_EndCap_A",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Pb"), // absorber
                G4Material::GetMaterial("liquidArgon"), // gap
                200, // layers,
                2.*mm, // abso
                4.*mm, // gap
                28.*cm, // start radio,
                100.*cm, // z
                G4ThreeVector(0,0, -1*(3.*m + 100.*cm)));
  // Create Hadronic calorimeter
  CreateBarrel( worldLV,
                "HCal_EndCap_A",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_CESIUM_IODIDE"), // absorber
                G4Material::GetMaterial("G4_Fe"), // gap
                5, // layers,
                10.*cm, // abso
                10.*cm, // gap
                148*cm, // start radio,
                1.*m, //z
                G4ThreeVector(0,0,-1*(3.*m + 100.*cm)));

  */

  return worldPV;
}

void DetectorATLASConstruction::CreateBarrel(  G4LogicalVolume *worldLV, 
                                          std::string name,  
                                          G4Material *defaultMaterial,
                                          G4Material *absorberMaterial,
                                          G4Material *gapMaterial,
                                          int nofLayers,
                                          double absoThickness,
                                          double gapThickness,
                                          double calorRmin,
                                          double calorZ,
                                          G4ThreeVector center_pos,
                                          G4Region *region
                                          ) 

{
  if ( ! defaultMaterial || ! absorberMaterial || ! gapMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorATLASConstruction::DefineVolumes()", "MyCode0001", FatalException, msg);
  }


  G4double layerThickness=absoThickness+gapThickness; 

  G4VSolid* calorimeterS = new G4Tubs( name,// its name
                                 calorRmin, // R min 1700mm
                                 calorRmin+ nofLayers*layerThickness, // R max 48cm+1700mm
                                 calorZ/2,    // Z max, +250cm
                                 0*deg,     // phi_min
                                 360*deg    // phi_max
                                 ) ;




  G4LogicalVolume* calorLV = new G4LogicalVolume( calorimeterS,     // its solid
                                                  defaultMaterial,  // its material
                                                  name);   // its name
  new G4PVPlacement(
                 0,                 // no rotation
                 center_pos,        // at (0,0,0)
                 calorLV,           // its logical volume
                 name,              // its name
                 worldLV,           // its mother  volume
                 false,             // no boolean operation
                 0,                 // copy number
                 m_checkOverlaps);  // checking overlaps
  
  region->AddRootLogicalVolume(calorLV);



  for (G4int layer=0; layer < nofLayers; ++layer){

    G4VSolid* layerS = new G4Tubs(name+ "_Layer",// its name
                                 calorRmin + layer*layerThickness,        // R min 1700mm
                                 calorRmin + (layer+1)*layerThickness,   // R max 48cm+1700mm
                                 calorZ/2,            // Z max, +250cm
                                 0*deg,             // phi_min
                                 360*deg            // phi_max
                                 ) ;

    G4LogicalVolume* layerLV = new G4LogicalVolume(
                                                  layerS,           // its solid
                                                  defaultMaterial,  // its material
                                                  name+"_Layer");   // its name


    new G4PVPlacement(
                 0,                 // no rotation
                 G4ThreeVector(0,0,0),
                 //center_pos,        // at (0,0,0)
                 layerLV,           // its logical volume
                 name+"_Layer",     // its name
                 calorLV,           // its mother  volume
                 false,             // no boolean operation
                 0,                 // copy number
                 m_checkOverlaps);  // checking overlaps
    


    G4VSolid* absorverS = new G4Tubs( name+"_Abso",// its name
                                 calorRmin + layer*(absoThickness + gapThickness) ,     // R min 1700mm
                                 calorRmin + layer*(absoThickness + gapThickness) + absoThickness,   // R max 48cm+1700mm
                                 calorZ/2,          // Z max, +250cm
                                 0*deg,                 // phi_min
                                 360*deg             // phi_max
                                 ) ;

    G4LogicalVolume* absorverLV = new G4LogicalVolume(
                                                  absorverS,           // its solid
                                                  absorberMaterial,  // its material
                                                  name+"_Abso");         // its name


    new G4PVPlacement(
                 0,                 // no rotation
                 G4ThreeVector(0,0,0),
                 //center_pos,        // at (0,0,0)
                 absorverLV,        // its logical volume
                 name+"_Abso",      // its name
                 layerLV,           // its mother  volume
                 false,             // no boolean operation
                 0,                 // copy number
                 m_checkOverlaps);  // checking overlaps




    G4VSolid* gapS = new G4Tubs( name+"_Gap",// its name
                                 calorRmin + layer*(absoThickness + gapThickness) + absoThickness,   // R max 48cm+1700mm
                                 calorRmin + (layer+1)*(absoThickness + gapThickness),   // R max 48cm+1700mm
                                 calorZ/2,          // Z max, +250cm
                                 0*deg,           // phi_min
                                 360*deg          // phi_max
        ); 

    G4LogicalVolume* gapLV = new G4LogicalVolume(
                                                  gapS,           // its solid
                                                  gapMaterial,  // its material
                                                  name+"_Gap");         // its name


    new G4PVPlacement(
                 0,                 // no rotation
                 G4ThreeVector(0,0,0),
                 //center_pos,        // at (0,0,0)
                 gapLV,             // its logical volume
                 name+"_Gap",       // its name
                 layerLV,           // its mother  volume
                 false,             // no boolean operation
                 0,                 // copy number
                 m_checkOverlaps);  // checking overlaps


  }// Loop over calorimeter layers


}







void DetectorATLASConstruction::ConstructSDandField(){

  if (m_useMagneticField){
    MSG_INFO("Set magnetic field")
    FieldSetup* fieldSetup = new FieldSetup(G4ThreeVector( 0.0 ,0.0, 2.0*tesla ), 745, false );
    G4AutoDelete::Register(fieldSetup);
    m_fieldSetup.Put(fieldSetup);
  }

}

