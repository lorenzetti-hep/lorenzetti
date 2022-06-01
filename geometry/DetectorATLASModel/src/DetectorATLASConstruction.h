#ifndef DetectorATLASConstruction_h
#define DetectorATLASConstruction_h

#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Property.h"
#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4Region.hh"
#include "globals.hh"
#include "G4Cache.hh"


class G4VPhysicalVolume;

class G4GlobalMagFieldMessenger;

class DetectorATLASConstruction : public G4VUserDetectorConstruction, public MsgService, public Gaugi::PropertyService
{
  public:
    DetectorATLASConstruction(std::string);
    virtual ~DetectorATLASConstruction();
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    // get methods
    const G4VPhysicalVolume* GetAbsorberPV() const;
    const G4VPhysicalVolume* GetGapPV() const;
     
  private:

    // methods
    void DefineMaterials();
    
    G4VPhysicalVolume* DefineVolumes();


    void CreateBarrel(  G4LogicalVolume *worldLV, 
                        std::string name,  
                        G4Material *defaltMaterial,
                        G4Material *absorberMaterial,
                        G4Material *gapMaterial,
                        int nofLayers,
                        double absoThickness,
                        double gapThickness,
                        double calorRmin,
                        double calorZ,
                        const G4ThreeVector &center_pos,
                        G4Region* region);

    void CreateEndcap(  G4LogicalVolume *worldLV, 
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
                      );

    G4Region* GetRegion( std::string name );


    bool m_checkOverlaps; // option to activate checking of volumes overlaps
    bool m_useMagneticField;
    bool m_cutOnPhi;

    bool m_useBarrel;
    bool m_useTile;
    bool m_useTileExt;
    bool m_useEMEC;
    bool m_useHEC;

    bool m_useCrack;
    bool m_useDeadMaterial;


    static G4ThreadLocal G4GlobalMagFieldMessenger*  m_magFieldMessenger;
};




#endif

