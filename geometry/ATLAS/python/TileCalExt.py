

__all__ = []


#
# ATLAS Detector construction
#

from G4Kernel.DetectorConstruction import *
from CaloCell.CaloDefs import Detector, CaloSampling
from CaloCellBuilder import Calorimeter
import numpy as np
import os

basepath = os.environ['LZT_PATH']+'/geometry/ATLAS/data'

m   = 1000
dm  = 100
cm  = 10
mm  = 1
pi  = np.pi
MeV = 1

# TileExt Barrel
endcap_start = 3.704*m
extended_barrel_start = endcap_start
extended_barrel_zsize = 2.83*m


def createTileCalExt(left_side=False):


    sign = -1 if left_side else 1
    side_name = '_' + ('B' if left_side else 'A')


    nlayers=4; absorber=6.0*cm; gap=4.0*cm; rsize=nlayers*(absorber+gap)
    tilecalExt1_pv =  PhysicalVolume( Name               = "TileCalExt1"+side_name, 
                                      Plates             = Plates.Horizontal, # Logical type
                                      AbsorberMaterial   = "G4_Fe", # absorber
                                      GapMaterial        = "PLASTIC SCINTILLATOR", # gap
                                      NofLayers          = nlayers, # layers
                                      AbsorberThickness  = absorber, # abso
                                      GapThickness       = gap, # gap
                                      RMin               = 228.3*cm, # radio min,
                                      RMax               = 228.3*cm + rsize, # radio max 
                                      ZSize              = extended_barrel_zsize  ,
                                      X=0,Y=0,Z=sign*(extended_barrel_start + 0.5*extended_barrel_zsize), # x,y,z 
                                      Visualization = True,
                                      Color         = 'salmon',
                                    )
    nlayers=11; absorber=6.2*cm; gap=3.8*cm; rsize=nlayers*(absorber+gap)
    tilecalExt2_pv =  PhysicalVolume( Name               = "TileCalExt2"+side_name, 
                                      Plates             = Plates.Horizontal, # Logical type
                                      AbsorberMaterial   = "G4_Fe", # absorber
                                      GapMaterial        = "PLASTIC SCINTILLATOR", # gap
                                      NofLayers          = 11, # layers
                                      AbsorberThickness  = 6.2*cm, # abso
                                      GapThickness       = 3.8*cm, # gap
                                      RMin               = tilecalExt1_pv.RMax, # radio min,
                                      RMax               = tilecalExt1_pv.RMax + rsize, # radio max 
                                      ZSize              = extended_barrel_zsize ,
                                      X=0,Y=0,Z=sign*(extended_barrel_start + 0.5*extended_barrel_zsize), # x,y,z 
                                      Visualization = True,
                                      Color         = 'violetred'
                                    )

    nlayers=5; absorber=6.2*cm; gap=3.8*cm; rsize=nlayers*(absorber+gap)
    tilecalExt3_pv =  PhysicalVolume( Name               = "TileCalExt3"+side_name, 
                                      Plates             = Plates.Horizontal, # Logical type
                                      AbsorberMaterial   = "G4_Fe", # absorber
                                      GapMaterial        = "PLASTIC SCINTILLATOR", # gap
                                      NofLayers          = 5, # layers
                                      AbsorberThickness  = 6.2*cm, # abso
                                      GapThickness       = 3.8*cm, # gap
                                      RMin               = tilecalExt2_pv.RMax, # radio min,
                                      RMax               = tilecalExt2_pv.RMax + rsize, # radio max 
                                      ZSize              = extended_barrel_zsize ,
                                      X=0,Y=0,Z=sign*(extended_barrel_start + 0.5*extended_barrel_zsize), # x,y,z 
                                      Visualization = True,
                                      Color         = 'orangered'
                                    )


    tilecalExt1_sv = SensitiveVolume( tilecalExt1_pv, DeltaEta = 0.1  , DeltaPhi = pi/32  )
    tilecalExt2_sv = SensitiveVolume( tilecalExt2_pv, DeltaEta = 0.1  , DeltaPhi = pi/32  )
    tilecalExt3_sv = SensitiveVolume( tilecalExt3_pv, DeltaEta = 0.2  , DeltaPhi = pi/32  )


    # Configure the electronic frontend and the detector parameters
    tilecalExt1_det = Calorimeter( tilecalExt1_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                   CollectionKey = "Collection_TileExt1"+side_name, # collection key
                                   Detector      = Detector.TILE, # detector type
                                   Sampling      = CaloSampling.TileExt1, # sampling type
                                   Shaper        = basepath + "/pulseTile.dat", # pulse shaper
                                   Noise         = 20*MeV, # electronic noise
                                   Samples       = 7, # how many samples
                                   OFWeights     = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                                 )

    # Configure the electronic frontend and the detector parameters
    tilecalExt2_det = Calorimeter( tilecalExt2_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                   CollectionKey = "Collection_TileExt2"+side_name, # collection key
                                   Detector      = Detector.TILE, # detector type
                                   Sampling      = CaloSampling.TileExt2, # sampling type
                                   Shaper        = basepath + "/pulseTile.dat", # pulse shaper
                                   Noise         = 20*MeV, # electronic noise
                                   Samples       = 7, # how many samples
                                   OFWeights     = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                                 )

    # Configure the electronic frontend and the detector parameters
    tilecalExt3_det = Calorimeter( tilecalExt3_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                   CollectionKey = "Collection_TileExt3"+side_name, # collection key
                                   Detector      = Detector.TILE, # detector type
                                   Sampling      = CaloSampling.TileExt3, # sampling type
                                   Shaper        = basepath + "/pulseTile.dat", # pulse shaper
                                   Noise         = 20*MeV, # electronic noise
                                   Samples       = 7, # how many samples
                                   OFWeights     = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                                 )
    
    return [tilecalExt1_det, tilecalExt2_det, tilecalExt3_det]

