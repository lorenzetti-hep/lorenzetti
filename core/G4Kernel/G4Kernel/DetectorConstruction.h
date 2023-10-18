#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "GaugiKernel/MsgStream.h"
#include "GaugiKernel/Property.h"
#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4Region.hh"
#include "globals.hh"
#include "G4Cache.hh"


//enum Plates{
//    Horizontal       = 0,
//    Vertical         = 1,
//};

class G4VPhysicalVolume;

class G4GlobalMagFieldMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction, public MsgService, public Gaugi::PropertyService
{
  public:


    struct Volume{
      std::string name;
      int plates;
      std::string absorberMaterial;
      std::string gapMaterial;
      int nofLayers;
      double absoThickness;
      double gapThickness;
      double rMin;
      double rMax;
      double zSize;
      double x;
      double y;
      double z;

      double electronCut;
      double positronCut;
      double gammaCut;
      double photonCut;
    };

    DetectorConstruction(std::string);
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    // get methods
    const G4VPhysicalVolume* GetAbsorberPV() const;
    const G4VPhysicalVolume* GetGapPV() const;

    void AddVolume(std::string region,
                   int plates,
                   std::string absorberMaterial,
                   std::string gapMaterial,
                   int nofLayers,
                   double absoThickness,
                   double gapThickness,
                   double rMin,
                   double rMax,
                   double zSize,
                   double x,
                   double y,
                   double z,
                   // production cuts
                   double electronCut,
                   double positronCut,
                   double gammaCut,
                   double photonCut
                   );

  private:

    std::vector<Volume> m_volumes;

    // methods
    void DefineMaterials();
    
    G4VPhysicalVolume* DefineVolumes();


    void CreateHorizontalPlates(  G4LogicalVolume *worldLV, 
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

    void CreateVerticalPlates(  G4LogicalVolume *worldLV, 
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
                                G4Region *region);

    G4Region* GetRegion( std::string name );


    bool m_checkOverlaps; // option to activate checking of volumes overlaps
    bool m_useMagneticField;
    bool m_cutOnPhi;
    int m_outputLevel;

    static G4ThreadLocal G4GlobalMagFieldMessenger*  m_magFieldMessenger;
};




#endif

