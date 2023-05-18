
__all__ = []

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



# TileCal 
endcap_start = 3704.*mm
gap_between_extended_barrel = 68.*cm
tile_barrel_start = 0*m
tile_barrel_end = (endcap_start - gap_between_extended_barrel)
tile_barrel_z = (tile_barrel_start + tile_barrel_end) * 2


def createTileCal():

    tilecal1_pv =  PhysicalVolume( Name               = "TileCal1", 
                                   Plates             = Plates.Horizontal, # Logical type
                                   AbsorberMaterial   = "G4_Fe", # absorber
                                   GapMaterial        = "PLASTIC SCINTILLATOR", # gap
                                   NofLayers          = 4, # layers
                                   AbsorberThickness  = 6.0*cm, # abso
                                   GapThickness       = 4.0*cm, # gap
                                   RMin               = 228.3*cm, # radio min,
                                   RMax               = 228.3*cm + 4*(6.0*cm + 4.0*cm), # radio max 
                                   ZSize              = tile_barrel_z,
                                   X=0,Y=0,Z=0, # x,y,z (center in 0,0,0)
                                   Visualization = True,
                                   Color         = 'salmon'
                                   )

    tilecal2_pv =  PhysicalVolume( Name               = "TileCal2", 
                                   Plates             = Plates.Horizontal, # Logical type
                                   AbsorberMaterial   = "G4_Fe", # absorber
                                   GapMaterial        = "PLASTIC SCINTILLATOR", # gap
                                   NofLayers          = 11, # layers
                                   AbsorberThickness  = 6.2*cm, # abso
                                   GapThickness       = 3.8*cm, # gap
                                   RMin               = tilecal1_pv.RMax, # radio min,
                                   RMax               = tilecal1_pv.RMax + 11*(6.2*cm + 3.8*cm), # radio max 
                                   ZSize              = tile_barrel_z,
                                   X=0,Y=0,Z=0, # x,y,z (center in 0,0,0)
                                   Visualization = True,
                                   Color         = 'violetred'
                                   )

    tilecal3_pv =  PhysicalVolume( Name               = "TileCal3", 
                                   Plates             = Plates.Horizontal, # Logical type
                                   AbsorberMaterial   = "G4_Fe", # absorber
                                   GapMaterial        = "PLASTIC SCINTILLATOR", # gap
                                   NofLayers          = 5, # layers
                                   AbsorberThickness  = 6.2*cm, # abso
                                   GapThickness       = 3.8*cm, # gap
                                   RMin               = tilecal2_pv.RMax, # radio min,
                                   RMax               = tilecal2_pv.RMax + 5*(6.2*cm + 3.8*cm), # radio max 
                                   ZSize              = tile_barrel_z,
                                   X=0,Y=0,Z=0, # x,y,z (center in 0,0,0)
                                   Visualization = True,
                                   Color         = 'orangered'
                                   )


    tilecal1_sv = SensitiveVolume( tilecal1_pv, DeltaEta = 0.1  , DeltaPhi = pi/32 )
    tilecal2_sv = SensitiveVolume( tilecal2_pv, DeltaEta = 0.1  , DeltaPhi = pi/32 )
    tilecal3_sv = SensitiveVolume( tilecal3_pv, DeltaEta = 0.2  , DeltaPhi = pi/32 )




    # Configure the electronic frontend and the detector parameters
    tilecal1_det = Calorimeter( tilecal1_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                CollectionKey = "Collection_TileCal1", # collection key
                                Detector      = Detector.TILE, # detector type
                                Sampling      = CaloSampling.TileCal1, # sampling type
                                Shaper        = basepath + "/pulseTile.dat", # pulse shaper
                                Noise         = 20*MeV, # electronic noise
                                Samples       = 7, # how many samples
                                OFWeights     = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                              )

    # Configure the electronic frontend and the detector parameters
    tilecal2_det = Calorimeter( tilecal2_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                CollectionKey = "Collection_TileCal2", # collection key
                                Detector      = Detector.TILE, # detector type
                                Sampling      = CaloSampling.TileCal2, # sampling type
                                Shaper        = basepath + "/pulseTile.dat", # pulse shaper
                                Noise         = 20*MeV, # electronic noise
                                Samples       = 7, # how many samples
                                OFWeights     = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                              )

    # Configure the electronic frontend and the detector parameters
    tilecal3_det = Calorimeter( tilecal3_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                CollectionKey = "Collection_TileCal3", # collection key
                                Detector      = Detector.TILE, # detector type
                                Sampling      = CaloSampling.TileCal3, # sampling type
                                Shaper        = basepath + "/pulseTile.dat", # pulse shaper
                                Noise         = 20*MeV, # electronic noise
                                Samples       = 7, # how many samples
                                OFWeights     = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                              )

    return [tilecal1_det, tilecal2_det, tilecal3_det]