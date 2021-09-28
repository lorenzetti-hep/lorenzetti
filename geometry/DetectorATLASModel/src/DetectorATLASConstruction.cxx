
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
#include "G4ProductionCuts.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4RegionStore.hh"
#include <string>
#include <sstream>

namespace{
template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}
}

G4ThreadLocal G4GlobalMagFieldMessenger* DetectorATLASConstruction::m_magFieldMessenger = nullptr;


DetectorATLASConstruction::DetectorATLASConstruction(std::string name)
 : 
  IMsgService(name), 
   G4VUserDetectorConstruction(),
   m_checkOverlaps(true)
{

  // magnetic field
  declareProperty( "UseMagneticField"           , m_useMagneticField=true     );
  // Samplings
  declareProperty( "UseBarrel"                  , m_useBarrel=true            );
  declareProperty( "UseTile"                    , m_useTile=true              );
  declareProperty( "UseTileExt"                 , m_useTileExt=true           );
  declareProperty( "UseEMEC"                    , m_useEMEC=true              );
  declareProperty( "UseHEC"                     , m_useHEC=true               );
  // Struture 
  declareProperty( "UseCrack"                   , m_useCrack=true             );
  declareProperty( "UseDeadMaterial"            , m_useDeadMaterial=true      );
  // visualization
  declareProperty( "CutOnPhi"                   , m_cutOnPhi=false            );
  
  MSG_INFO( "Using ATLAS-like detector." );
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
  nistManager->FindOrBuildMaterial("G4_Cu");
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

  //
  // TileCal
  //
  double endcap_start = 3704.*mm;
  double gap_between_extended_barrel = 68.*cm;
  double tile_barrel_z = (endcap_start - gap_between_extended_barrel)*2; 
  //
  // TileExt
  //
  const auto extended_barrel_size = 2.83*m;
  const auto extended_barrel_center_pos = endcap_start+extended_barrel_size/2;
  //
  // EndCap (EMEC)
  //
  double ps_endcap_z = 5.*mm;
  double emec_dead_material_z = 78.*mm - ps_endcap_z;
  double emec_dead_material_center = endcap_start+emec_dead_material_z/2;
  double ps_endcap_start = endcap_start+emec_dead_material_z;
  double ps_endcap_center = ps_endcap_start + ps_endcap_z/2;
  double emec_start = ps_endcap_start+ps_endcap_z;
  double emec1_zsize = 96.*mm;
  double emec2_zsize = 330.*mm;
  double emec3_zsize = 54.*mm;
  double emec1_center = emec_start + emec1_zsize/2;
  double emec2_center = emec_start + emec1_zsize + emec2_zsize/2;
  double emec3_center = emec_start + emec1_zsize + emec2_zsize + emec3_zsize/2;
  //
  // Hadronic end-cap
  //
  const double hec_start = 4262*mm;
  const double first_hec_gap = 8.5*mm;
  const double first_hec_front_plate = 12.5*mm;
  const double first_hec_plates = 25*mm;
  const double hec1_nplates = 8;
  const double hec2_nplates = 16;
  const double second_hec_gap = 8.5*mm;
  const double second_hec_front_plate = 25*mm;
  const double second_hec_plates = 50*mm;
  const double hec3_nplates = 16;
  const double first_hec_front_plate_z = first_hec_gap+first_hec_front_plate;
  const double first_hec_front_plate_center = hec_start + first_hec_front_plate_z/2;
  const double hec1_z = hec1_nplates*(first_hec_gap+first_hec_plates);
  const double hec1_center = first_hec_front_plate_center + first_hec_front_plate_z/2 + hec1_z/2;
  const double hec2_z = hec2_nplates*(first_hec_gap+first_hec_plates);
  const double hec2_center = hec1_center + hec1_z/2 + hec2_z/2;
  const double gap_between_hecs = 32*mm;
  const double second_hec_front_plate_z = second_hec_gap+second_hec_front_plate;
  const double second_hec_front_plate_center = hec2_center + hec2_z/2 + gap_between_hecs + second_hec_front_plate_z/2;
  const double hec3_z = hec3_nplates*(second_hec_gap+second_hec_plates);
  const double hec3_center = second_hec_front_plate_center + second_hec_front_plate_z/2 + hec3_z/2;
  // Support of end-cap
  const double endcap_end = 6120;
  const double endcap_center = ( endcap_end + endcap_start )/2;
  const double endcap_size = ( endcap_end - endcap_start );
  //
  // Crack Region
  //
  const double crack_material_center = ( tile_barrel_z/2 + endcap_start )/2;
  const double crack_material_size = gap_between_extended_barrel;
  const double crack_em_material_center = ( 6.8*m/2 + endcap_start )/2;
  const double crack_em_material_size = ( endcap_start - 6.8*m/2  );


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
                                 (!m_cutOnPhi)?(360*deg):(235*deg)      // phi_max
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

  






  if(m_useBarrel)
  {
    if(m_useDeadMaterial ){
      auto deadMaterial = GetRegion("DeadMaterial");
      // Create a region
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
                    deadMaterial );
    }

    auto presample = GetRegion("PS");
    auto em1       = GetRegion("EM1");
    auto em2       = GetRegion("EM2");
    auto em3       = GetRegion("EM3");
  
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
                  G4ThreeVector(0,0,0), // center
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
                  G4ThreeVector(0,0,0), // center
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
                  G4ThreeVector(0,0,0), // center
                  em3);
  }



  

  if(m_useTile)
  {
    auto had1 = GetRegion("HAD1");
    auto had2 = GetRegion("HAD2");
    auto had3 = GetRegion("HAD3");

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
                  tile_barrel_z, // z
                  G4ThreeVector(0,0,0), // center
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
                  tile_barrel_z, // z
                  G4ThreeVector(0,0,0), // center
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
                  tile_barrel_z, // z
                  G4ThreeVector(0,0,0), // center
                  had3);



    if(m_useDeadMaterial){
      // Get materials
      auto deadMaterial = GetRegion("DeadMaterial");
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
                    G4ThreeVector(0,0,0), // center
                    deadMaterial );
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
                    G4ThreeVector(0,0,0), // center
                    deadMaterial );
    }
  }




  if ( m_useTileExt ){
    
    auto had1 = GetRegion("HAD1");
    auto had2 = GetRegion("HAD2");
    auto had3 = GetRegion("HAD3");

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
                  G4ThreeVector(0,0,extended_barrel_center_pos), // center
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
                  G4ThreeVector(0,0,extended_barrel_center_pos), // center
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
                  G4ThreeVector(0,0,extended_barrel_center_pos), // center
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
                  G4ThreeVector(0,0,-extended_barrel_center_pos), // center
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
                  G4ThreeVector(0,0,-extended_barrel_center_pos), // center
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
                  G4ThreeVector(0,0,-extended_barrel_center_pos), // center
                  had3 );

  }// Extended Barrel


  if( m_useEMEC){

    auto emec1 = GetRegion("EMEC1");
    auto emec2 = GetRegion("EMEC2");
    auto emec3 = GetRegion("EMEC3");
    auto presample = GetRegion("PS");


    if(m_useDeadMaterial)
    {
      auto deadMaterial = GetRegion("DeadMaterial");
      //
      // Dead-material
      //
      CreateEndcap( worldLV,
                    "DeadMaterial",
                    G4Material::GetMaterial("Galactic"), // default
                    G4Material::GetMaterial("G4_Pb"), // absorber
                    G4Material::GetMaterial("Galactic"), // gap
                    2, // layers,
                    5.*mm,  // abso
                    31.5*mm,  // gap
                    302.*mm,  // start radio,
                    2032.*mm, // end radio,
                    emec_dead_material_z,  // z
                    G4ThreeVector(0,0,emec_dead_material_center), // center
                    deadMaterial );
      CreateEndcap( worldLV,
                    "DeadMaterial",
                    G4Material::GetMaterial("Galactic"), // default
                    G4Material::GetMaterial("G4_Pb"), // absorber
                    G4Material::GetMaterial("Galactic"), // gap
                    2, // layers,
                    5.*mm,  // abso
                    31.5*mm,  // gap
                    302.*mm,  // start radio,
                    2032.*mm, // end radio,
                    emec_dead_material_z,  // z
                    G4ThreeVector(0,0,-emec_dead_material_center), // center
                    deadMaterial );
    }

    CreateEndcap( worldLV,
                  "PS",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("Galactic"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  1, // layers,
                  0.001*mm,  // abso
                  4.99*mm,  // gap
                  1232.*mm,  // start radio,
                  1700.*mm, // end radio,
                  ps_endcap_z,  // z
                  G4ThreeVector(0,0,ps_endcap_center), // center
                  presample );
    CreateEndcap( worldLV,
                  "PS",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("Galactic"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  1, // layers,
                  0.001*mm,  // abso
                  4.99*mm,  // gap
                  1232.*mm,  // start radio,
                  1700.*mm, // end radio,
                  ps_endcap_z,  // z
                  G4ThreeVector(0,0,-ps_endcap_center), // center
                  presample );

    // Create EMEC1
    CreateEndcap( worldLV,
                  "EMEC1",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Pb"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  16, // layers,
                  2.27*mm,  // abso
                  3.73*mm,  // gap
                  302.*mm,  // start radio,
                  2032.*mm, // end radio,
                  emec1_zsize,  // z
                  G4ThreeVector(0,0,emec1_center), // center
                  emec1 );
    CreateEndcap( worldLV,
                  "EMEC1",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Pb"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  16, // layers,
                  2.27*mm,  // abso
                  3.73*mm,  // gap
                  302.*mm,  // start radio,
                  2032.*mm, // end radio,
                  emec1_zsize,  // z
                  G4ThreeVector(0,0,-emec1_center), // center
                  emec1 );

    // Create EMEC2
    CreateEndcap( worldLV,
                  "EMEC2",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Pb"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  55, // layers,
                  2.27*mm,  // abso
                  3.73*mm,  // gap
                  302.*mm,  // start radio,
                  2032.*mm, // end radio,
                  emec2_zsize,  // z
                  G4ThreeVector(0,0,emec2_center), // center
                  emec2 );
    CreateEndcap( worldLV,
                  "EMEC2",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Pb"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  55, // layers,
                  2.27*mm,  // abso
                  3.73*mm,  // gap
                  302.*mm,  // start radio,
                  2032.*mm, // end radio,
                  emec2_zsize,  // z
                  G4ThreeVector(0,0,-emec2_center), // center
                  emec2 );
    // Create EMEC3
    CreateEndcap( worldLV,
                  "EMEC3",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Pb"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  9, // layers,
                  2.41*mm,  // abso
                  3.59*mm,  // gap
                  302.*mm,  // start radio,
                  2032.*mm, // end radio,
                  emec3_zsize,  // z
                  G4ThreeVector(0,0,emec3_center), // center
                  emec3 );
    CreateEndcap( worldLV,
                  "EMEC3",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Pb"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  9, // layers,
                  2.41*mm,  // abso
                  3.59*mm,  // gap
                  302.*mm,  // start radio,
                  2032.*mm, // end radio,
                  emec3_zsize,  // z
                  G4ThreeVector(0,0,-emec3_center), // center
                  emec3 );
  }




  if(m_useHEC){

    auto hec1 = GetRegion("HEC1");
    auto hec2 = GetRegion("HEC2");
    auto hec3 = GetRegion("HEC3");

    // HEC1 first bar
    CreateEndcap( worldLV,
                  "HEC1",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  1, // layers,
                  first_hec_front_plate,  // abso
                  first_hec_gap,  // gap
                  372.*mm,  // start radio,
                  2030.*mm, // end radio,
                  first_hec_front_plate_z,  // z
                  G4ThreeVector(0,0,first_hec_front_plate_center), // center
                  hec1 );
    CreateEndcap( worldLV,
                  "HEC1",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  1, // layers,
                  first_hec_front_plate,  // abso
                  first_hec_gap,  // gap
                  372.*mm,  // start radio,
                  2030.*mm, // end radio,
                  first_hec_front_plate_z,  // z
                  G4ThreeVector(0,0,-first_hec_front_plate_center), // center
                  hec1 );
    // HEC1
    CreateEndcap( worldLV,
                  "HEC1",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  hec1_nplates, // layers,
                  first_hec_plates,  // abso
                  first_hec_gap,  // gap
                  372.*mm,  // start radio,
                  2030.*mm, // end radio,
                  hec1_z,  // z
                  G4ThreeVector(0,0,hec1_center), // center
                  hec1 );
    CreateEndcap( worldLV,
                  "HEC1",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  hec1_nplates, // layers,
                  first_hec_plates,  // abso
                  first_hec_gap,  // gap
                  372.*mm,  // start radio,
                  2030.*mm, // end radio,
                  hec1_z,  // z
                  G4ThreeVector(0,0,-hec1_center), // center
                  hec1 );
    // HEC2
    CreateEndcap( worldLV,
                  "HEC2",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  hec2_nplates, // layers,
                  first_hec_plates,  // abso
                  first_hec_gap,  // gap
                  475.*mm,  // start radio,
                  2030.*mm, // end radio,
                  hec2_z,  // z
                  G4ThreeVector(0,0,hec2_center), // center
                  hec2 );
    CreateEndcap( worldLV,
                  "HEC2",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  hec2_nplates, // layers,
                  first_hec_plates,  // abso
                  first_hec_gap,  // gap
                  475.*mm,  // start radio,
                  2030.*mm, // end radio,
                  hec2_z,  // z
                  G4ThreeVector(0,0,-hec2_center), // center
                  hec2 );
    // HEC3 first bar
    CreateEndcap( worldLV,
                  "HEC3",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  1, // layers,
                  second_hec_front_plate,  // abso
                  second_hec_gap,  // gap
                  475.*mm,  // start radio,
                  2030.*mm, // end radio,
                  second_hec_front_plate_z,  // z
                  G4ThreeVector(0,0,second_hec_front_plate_center), // center
                  hec3 );
    CreateEndcap( worldLV,
                  "HEC3",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  1, // layers,
                  second_hec_front_plate,  // abso
                  second_hec_gap,  // gap
                  475.*mm,  // start radio,
                  2030.*mm, // end radio,
                  second_hec_front_plate_z,  // z
                  G4ThreeVector(0,0,-second_hec_front_plate_center), // center
                  hec3 );
    // HEC3
    CreateEndcap( worldLV,
                  "HEC3",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  hec3_nplates, // layers,
                  second_hec_plates,  // abso
                  second_hec_gap,  // gap
                  475.*mm,  // start radio,
                  2030.*mm, // end radio,
                  hec3_z,  // z
                  G4ThreeVector(0,0,hec3_center), // center
                  hec3 );
    CreateEndcap( worldLV,
                  "HEC3",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Cu"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  hec3_nplates, // layers,
                  second_hec_plates,  // abso
                  second_hec_gap,  // gap
                  475.*mm,  // start radio,
                  2030.*mm, // end radio,
                  hec3_z,  // z
                  G4ThreeVector(0,0,-hec3_center), // center
                  hec3 );


    if(m_useDeadMaterial)
    {
      auto deadMaterial = GetRegion("DeadMaterial");

      CreateBarrel( worldLV,
                    "Hcal_Boundary",
                    G4Material::GetMaterial("Galactic"), // default
                    G4Material::GetMaterial("G4_Pb"), // absorber
                    G4Material::GetMaterial("Galactic"), // gap
                    1, // layers
                    10*cm, // abso
                    5*mm, // gap
                    2035.*mm, // start radio,
                    endcap_size ,// z
                    G4ThreeVector(0,0,endcap_center), // center
                    deadMaterial );
      CreateBarrel( worldLV,
                    "Hcal_Boundary",
                    G4Material::GetMaterial("Galactic"), // default
                    G4Material::GetMaterial("G4_Pb"), // absorber
                    G4Material::GetMaterial("Galactic"), // gap
                    1, // layers
                    10*cm, // abso
                    5*mm, // gap
                    2035.*mm, // start radio,
                    endcap_size ,// z
                    G4ThreeVector(0,0,-endcap_center), // center
                    deadMaterial );

      CreateBarrel( worldLV,
                    "Hcal_Boundary",
                    G4Material::GetMaterial("Galactic"), // default
                    G4Material::GetMaterial("G4_Pb"), // absorber
                    G4Material::GetMaterial("Galactic"), // gap
                    1, // layers
                    10*cm, // abso
                    3*mm, // gap
                    218*cm, // start radio,
                    endcap_size ,// z
                    G4ThreeVector(0,0,endcap_center), // center
                    deadMaterial );
      CreateBarrel( worldLV,
                    "Hcal_Boundary",
                    G4Material::GetMaterial("Galactic"), // default
                    G4Material::GetMaterial("G4_Pb"), // absorber
                    G4Material::GetMaterial("Galactic"), // gap
                    1, // layers
                    10*cm, // abso
                    3*mm, // gap
                    218*cm, // start radio,
                    endcap_size ,// z
                    G4ThreeVector(0,0,-endcap_center), // center
                    deadMaterial );
    }
  }// EndCap



  if(m_useCrack){

    auto crackMaterial = GetRegion("CrackDeadMaterial");
    CreateBarrel( worldLV,
                  "CrackDeadMaterial",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Al"), // absorber
                  G4Material::GetMaterial("Galactic"), // gap
                  1, // layers
                  200*cm, // abso
                  3*mm, // gap
                  228.3*cm, // start radio,
                  crack_material_size ,// z
                  G4ThreeVector(0,0,crack_material_center), // center
                  crackMaterial );
    CreateBarrel( worldLV,
                  "CrackDeadMaterial",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Al"), // absorber
                  G4Material::GetMaterial("Galactic"), // gap
                  1, // layers
                  200*cm, // abso
                  3*mm, // gap
                  228.3*cm, // start radio,
                  crack_material_size ,// z
                  G4ThreeVector(0,0,-crack_material_center), // center
                  crackMaterial );
    CreateBarrel( worldLV,
                  "CrackDeadMaterial",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Al"), // absorber
                  G4Material::GetMaterial("Galactic"), // gap
                  1, // layers
                  200*cm, // abso
                  3*mm, // gap
                  228.3*cm, // start radio,
                  crack_material_size ,// z
                  G4ThreeVector(0,0,crack_material_center), // center
                  crackMaterial );
    CreateEndcap( worldLV,
                  "CrackDeadMaterial",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Al"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  2, // layers,
                  45.*mm, // abso
                  107.*mm, // gap
                  1025*mm, // start radio,
                  2032*mm, // end radio,
                  crack_em_material_size,  // z
                  G4ThreeVector(0,0,crack_em_material_center), // center
                  crackMaterial );
    CreateEndcap( worldLV,
                  "CrackDeadMaterial",
                  G4Material::GetMaterial("Galactic"), // default
                  G4Material::GetMaterial("G4_Al"), // absorber
                  G4Material::GetMaterial("liquidArgon"), // gap
                  2, // layers,
                  45.*mm, // abso
                  107.*mm, // gap
                  1025*mm, // start radio,
                  2032*mm, // end radio,
                  crack_em_material_size,  // z
                  G4ThreeVector(0,0,-crack_em_material_center), // center
                  crackMaterial );
  }



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
                                          const G4ThreeVector &center_pos,
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
                                 (!m_cutOnPhi)?(360*deg):(235*deg)    // phi_max
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
                                 (!m_cutOnPhi)?(360*deg):(235*deg)            // phi_max
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
                                 (!m_cutOnPhi)?(360*deg):(235*deg)             // phi_max
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
                                 (!m_cutOnPhi)?(360*deg):(235*deg)          // phi_max
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


void DetectorATLASConstruction::CreateEndcap(  G4LogicalVolume *worldLV, 
                                          std::string name,  
                                          G4Material *defaultMaterial,
                                          G4Material *absorberMaterial,
                                          G4Material *gapMaterial,
                                          int nofLayers,
                                          double absoThickness,
                                          double gapThickness,
                                          double calorRmin,
                                          double calorRmax,
                                          double calorZ,
                                          const G4ThreeVector &center_pos,
                                          G4Region *region
                                          ) 
{
  if ( ! defaultMaterial || ! absorberMaterial || ! gapMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined.";
    G4Exception("DetectorATLASConstruction::DefineVolumes()", "MyCode0001", FatalException, msg);
  }

  const bool evenNofLayers = !(nofLayers % 2);
  const bool invertOrder = center_pos.z() < 0.;

  G4double layerThickness=absoThickness+gapThickness; 

  G4VSolid* calorimeterS = new G4Tubs( name,// its name
                                 calorRmin,
                                 calorRmax,
                                 calorZ/2, // Size in z
                                 0*deg,     // phi_min
                                 (!m_cutOnPhi)?(360*deg):(235*deg)    // phi_max
                                 ) ;

  G4LogicalVolume* calorLV = new G4LogicalVolume( calorimeterS,     // its solid
                                                  defaultMaterial,  // its material
                                                  name);   // its name

  //G4cout << "evenNofLayers: " << evenNofLayers << "; invertOrder: " << invertOrder << std::endl;
  //G4cout << "Creating calorimeter layer " << name << " at: " << center_pos << ". Size: " << calorZ << std::endl;

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

  for (G4int layer=-nofLayers/2; layer <= nofLayers/2; ++layer){
    if ( layer == 0 && evenNofLayers){
      continue;
    }

    G4VSolid* layerS = new G4Tubs(name+ "_Layer",// its name
                                 calorRmin,
                                 calorRmax,
                                 layerThickness/2,
                                 0*deg,
                                 (!m_cutOnPhi)?(360*deg):(235*deg)
                                 );

    G4LogicalVolume* layerLV = new G4LogicalVolume(
                                                  layerS,           // its solid
                                                  defaultMaterial,  // its material
                                                  name+"_Layer");   // its name

    const auto layerLVCenter = G4ThreeVector(0,0,
                   ((invertOrder)?-1.:+1.) * 
                   (sign(-layer)*((evenNofLayers)?0.5:0.) + (layer))
                   *layerThickness // center at with respect to the calorimeter center
                 );

    //G4cout << "Creating layer " << layer << " for: " << layerLVCenter << ". Size: " << layerThickness << std::endl;

    new G4PVPlacement(
                 0,                 // no rotation
                 layerLVCenter,     // center
                 layerLV,           // its logical volume
                 name+"_Layer",     // its name
                 calorLV,           // its mother  volume
                 false,             // no boolean operation
                 0,                 // copy number
                 m_checkOverlaps);  // checking overlaps



    G4VSolid* absorverS = new G4Tubs( name+"_Abso",// its name
                                 calorRmin,     // R min
                                 calorRmax,     // R max
                                 absoThickness/2,  // 
                                 0*deg,         // phi_min
                                 (!m_cutOnPhi)?(360*deg):(235*deg)        // phi_max
                                 ) ;

    G4LogicalVolume* absorverLV = new G4LogicalVolume(
                                                  absorverS,           // its solid
                                                  absorberMaterial,  // its material
                                                  name+"_Abso");         // its name


    new G4PVPlacement(
                 0,                 // no rotation
                 G4ThreeVector(0,0,
                   ((invertOrder)?-1.:+1) * 
                   layerThickness/2
                   + ((invertOrder)?+1.:-1)/2 * 
                    absoThickness
                 ),
                 absorverLV,        // its logical volume
                 name+"_Abso",      // its name
                 layerLV,           // its mother  volume
                 false,             // no boolean operation
                 0,                 // copy number
                 m_checkOverlaps);  // checking overlaps




    G4VSolid* gapS = new G4Tubs( name+"_Gap",// its name
                                 calorRmin,   // R max
                                 calorRmax,   // R max
                                 gapThickness/2,   // Z max,
                                 0*deg,           // phi_min
                                 (!m_cutOnPhi)?(360*deg):(235*deg)          // phi_max
        ); 

    G4LogicalVolume* gapLV = new G4LogicalVolume(
                                                  gapS,           // its solid
                                                  gapMaterial,  // its material
                                                  name+"_Gap");         // its name


    new G4PVPlacement(
                 0,                 // no rotation
                 G4ThreeVector(0,0,
                   ((invertOrder)?+1.:-1) * 
                   layerThickness/2
                   + ((invertOrder)?-1.:+1) * 
                    gapThickness/2
                 ),
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
    // Create global magnetic field messenger.
    // Uniform magnetic field is then created automatically if
    // the field value is not zero.
    G4ThreeVector fieldValue(0.0 , 0.0, 2*tesla);
    m_magFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
    m_magFieldMessenger->SetVerboseLevel(1);

    // Register the field messenger for deleting
    G4AutoDelete::Register(m_magFieldMessenger); 
  }

}



G4Region* DetectorATLASConstruction::GetRegion( std::string name ){
  auto reg = G4RegionStore::GetInstance()->GetRegion(name);
  return reg ? reg : new G4Region(name);
}