
__all__ = ["getTileBarrelCfg","getTileExtendedCfg"]

from GaugiKernel.constants import *
from G4Kernel.DetectorConstruction import *
from G4Kernel.DetectorConstruction import SensitiveCaloVolume as SensitiveVolume
from CaloCell.CaloDefs import Detector, CaloSampling
from CaloCellBuilder import Calorimeter
import numpy as np
import os







def getTileBarrelCfg():

    basepath = os.environ['LZT_PATH']+'/geometry/ATLAS/data'

    # TileCal 
    endcap_start = 3704.*mm
    gap_between_extended_barrel = 68.*cm
    tile_barrel_start = 0*m
    tile_barrel_end = (endcap_start - gap_between_extended_barrel)
    tile_barrel_z = (tile_barrel_start + tile_barrel_end) * 2


    tilecal1_pv =  PhysicalVolume( Name               = "TILE::TileCal1", 
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

    tilecal2_pv =  PhysicalVolume( Name               = "TILE::TileCal2", 
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

    tilecal3_pv =  PhysicalVolume( Name               = "TILE::TileCal3", 
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


    #tilecal1_pv.Cuts = ProductionCuts(ElectronCut = 1, PositronCut = 1, GammaCut = 1)
    #tilecal2_pv.Cuts = ProductionCuts(ElectronCut = 1, PositronCut = 1, GammaCut = 1)
    #tilecal3_pv.Cuts = ProductionCuts(ElectronCut = 1, PositronCut = 1, GammaCut = 1)


    tilecal1_sv = SensitiveVolume( tilecal1_pv, DeltaEta = 0.1  , DeltaPhi = pi/32 )
    tilecal2_sv = SensitiveVolume( tilecal2_pv, DeltaEta = 0.1  , DeltaPhi = pi/32 )
    tilecal3_sv = SensitiveVolume( tilecal3_pv, DeltaEta = 0.2  , DeltaPhi = pi/32 )




    # Configure the electronic frontend and the detector parameters
    tilecal1_det = Calorimeter( tilecal1_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                CollectionKey   = "Collection_TileCal1", # collection key
                                Detector        = Detector.TILE, # detector type
                                Sampling        = CaloSampling.TileCal1, # sampling type
                                Shaper          = basepath + "/pulseTile.dat", # pulse shaper
                                Noise           = 20*MeV, # electronic noise
                                Samples         = 7, # how many samples
                                OFWeightsEnergy = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                                OFWeightsTime   = [0.197277, -2.336666, -18.474749, -0.247639, 14.832119, 4.627806, 1.401852], # values get from COOL, Field 1. 
                              )

    # Configure the electronic frontend and the detector parameters
    tilecal2_det = Calorimeter( tilecal2_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                CollectionKey = "Collection_TileCal2", # collection key
                                Detector      = Detector.TILE, # detector type
                                Sampling      = CaloSampling.TileCal2, # sampling type
                                Shaper        = basepath + "/pulseTile.dat", # pulse shaper
                                Noise         = 20*MeV, # electronic noise
                                Samples       = 7, # how many samples
                                OFWeightsEnergy = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                                OFWeightsTime   = [0.197277, -2.336666, -18.474749, -0.247639, 14.832119, 4.627806, 1.401852], # values get from COOL, Field 1. 
                              )

    # Configure the electronic frontend and the detector parameters
    tilecal3_det = Calorimeter( tilecal3_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                CollectionKey = "Collection_TileCal3", # collection key
                                Detector      = Detector.TILE, # detector type
                                Sampling      = CaloSampling.TileCal3, # sampling type
                                Shaper        = basepath + "/pulseTile.dat", # pulse shaper
                                Noise         = 20*MeV, # electronic noise
                                Samples       = 7, # how many samples
                                OFWeightsEnergy = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                                OFWeightsTime   = [0.197277, -2.336666, -18.474749, -0.247639, 14.832119, 4.627806, 1.401852], # values get from COOL, Field 1. 
                              )



    return [tilecal1_det, tilecal2_det, tilecal3_det]






def getTileExtendedCfg(left_side=False):


    # TileExt Barrel
    endcap_start = 3.704*m
    extended_barrel_start = endcap_start
    extended_barrel_zsize = 2.83*m
    basepath = os.environ['LZT_PATH']+'/geometry/ATLAS/data'

    sign = -1 if left_side else 1
    side_name = 'B' if left_side else 'A'


    nlayers=4; absorber=6.0*cm; gap=4.0*cm; rsize=nlayers*(absorber+gap)
    tilecalExt1_pv =  PhysicalVolume( Name               = "TILE::TileCalExt1::"+side_name, 
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
    tilecalExt2_pv =  PhysicalVolume( Name               = "TILE::TileCalExt2::"+side_name, 
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
    tilecalExt3_pv =  PhysicalVolume( Name               = "TILE::TileCalExt3::"+side_name, 
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
                                   CollectionKey   = "Collection_TileExt1_"+side_name, # collection key
                                   Detector        = Detector.TILE, # detector type
                                   Sampling        = CaloSampling.TileExt1, # sampling type
                                   Shaper          = basepath + "/pulseTile.dat", # pulse shaper
                                   Noise           = 20*MeV, # electronic noise
                                   Samples         = 7, # how many samples
                                   OFWeightsEnergy = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                                   OFWeightsTime   = [0.197277, -2.336666, -18.474749, -0.247639, 14.832119, 4.627806, 1.401852], # values get from COOL, Field 1. 
                                 )

    # Configure the electronic frontend and the detector parameters
    tilecalExt2_det = Calorimeter( tilecalExt2_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                   CollectionKey   = "Collection_TileExt2_"+side_name, # collection key
                                   Detector        = Detector.TILE, # detector type
                                   Sampling        = CaloSampling.TileExt2, # sampling type
                                   Shaper          = basepath + "/pulseTile.dat", # pulse shaper
                                   Noise           = 20*MeV, # electronic noise
                                   Samples         = 7, # how many samples
                                   OFWeightsEnergy = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                                   OFWeightsTime   = [0.197277, -2.336666, -18.474749, -0.247639, 14.832119, 4.627806, 1.401852], # values get from COOL, Field 1. 
                                 )

    # Configure the electronic frontend and the detector parameters
    tilecalExt3_det = Calorimeter( tilecalExt3_sv, -6, 4, -3, # sensitive volume, bunch start, bunch end, sampling start,
                                   CollectionKey   = "Collection_TileExt3_"+side_name, # collection key
                                   Detector        = Detector.TILE, # detector type
                                   Sampling        = CaloSampling.TileExt3, # sampling type
                                   Shaper          = basepath + "/pulseTile.dat", # pulse shaper
                                   Noise           = 20*MeV, # electronic noise
                                   Samples         = 7, # how many samples
                                   OFWeightsEnergy = [-0.3781,   -0.3572,    0.1808,    0.8125,   0.2767,    -0.2056,    -0.3292], # optimal filter parameters
                                   OFWeightsTime   = [0.197277, -2.336666, -18.474749, -0.247639, 14.832119, 4.627806, 1.401852], # values get from COOL, Field 1. 
                                 )
    
    return [tilecalExt1_det, tilecalExt2_det, tilecalExt3_det]

