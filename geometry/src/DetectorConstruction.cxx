
#include "geometry/DetectorConstruction.h"

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


G4ThreadLocal
G4GlobalMagFieldMessenger* DetectorConstruction::m_magFieldMessenger = 0;


DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   m_checkOverlaps(true)
{;}


DetectorConstruction::~DetectorConstruction()
{;}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();
  // Define volumes
  return DefineVolumes();
}


void DetectorConstruction::DefineMaterials()
{
  // Lead material defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
  nistManager->FindOrBuildMaterial("G4_Fe");
  nistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");

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


G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{


  // Get materials
  G4Material* defaultMaterial  = G4Material::GetMaterial("Galactic");


 

  //
  // World
  //
  G4VSolid* worldS = new G4Tubs( "World",     // its name
                                 0,           // R min
                                 5000*cm,     // R max
                                 10000*cm,    // Z max
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




  
 
  //
  // Barrel
  //
  // Create Eletromagnetic calorimeter
  CreateBarrel( worldLV,
                "ECal",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_Pb"), // absorber
                G4Material::GetMaterial("liquidArgon"), // gap
                80, // layers,
                2.*mm, // abso
                4.*mm, // gap
                1.*m, // start radio,
                6.2*m ,// z
                G4ThreeVector(0,0,0));


  // Create Hadronic calorimeter
  CreateBarrel( worldLV,
                "HCal",
                G4Material::GetMaterial("Galactic"), // default
                G4Material::GetMaterial("G4_CESIUM_IODIDE"), // absorber
                G4Material::GetMaterial("G4_Fe"), // gap
                5, // layers,
                10.*cm, // abso
                10.*cm, // gap
                1.*m + 48*cm, // start radio,
                6.2*m, // z
                G4ThreeVector(0,0,0));

  



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


void DetectorConstruction::ConstructSDandField()
{
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue = G4ThreeVector();
  m_magFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  m_magFieldMessenger->SetVerboseLevel(1);

  // Register the field messenger for deleting
  G4AutoDelete::Register(m_magFieldMessenger);
}





void DetectorConstruction::CreateBarrel(  G4LogicalVolume *worldLV, 
                                          G4String name,  
                                          G4Material *defaultMaterial,
                                          G4Material *absorberMaterial,
                                          G4Material *gapMaterial,
                                          G4int nofLayers,
                                          G4double absoThickness,
                                          G4double gapThickness,
                                          G4double calorRmin,
                                          G4double calorZ,
                                          G4ThreeVector center_pos
                                          ) 

{
  if ( ! defaultMaterial || ! absorberMaterial || ! gapMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorConstruction::DefineVolumes()", "MyCode0001", FatalException, msg);
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
                 calorLV,           // its mother  volume
                 false,             // no boolean operation
                 0,                 // copy number
                 m_checkOverlaps);  // checking overlaps


  }// Loop over calorimeter layers


}






