
__all__ = []

from G4Kernel.DetectorConstruction import *
from CaloCell.CaloDefs import Detector, CaloSampling
from CaloCellBuilder import Calorimeter
import numpy as np
import os

m   = 1000
dm  = 100
cm  = 10
mm  = 1
pi  = np.pi
MeV = 1


ecal_barrel_start = 0*m
ecal_barrel_end   = 3.4*m
ecal_barrel_z     = (ecal_barrel_start + ecal_barrel_end) * 2

endcap_start = 3704.*mm
gap_between_extended_barrel = 68.*cm

tile_barrel_start = 0*m
tile_barrel_end = (endcap_start - gap_between_extended_barrel)
tile_barrel_z = (tile_barrel_start + tile_barrel_end) * 2

def createDMVolumes():



    nlayers=2; absorber=7.7*cm; gap=10*cm; rsize=nlayers*(absorber+gap)
    dm_pv =  PhysicalVolume( Name               = "DeadMaterial", 
                             Plates             = Plates.Horizontal, # Logical type
                             AbsorberMaterial   = "G4_Al", # absorber
                             GapMaterial        = "Vacuum", # gap
                             NofLayers          = nlayers, # layers
                             AbsorberThickness  = absorber, # abso
                             GapThickness       = gap, # gap
                             RMin               = 110.6*cm, #crack_em_pv.RMax, #228.3*cm, # radio min,
                             RMax               = 110.6*cm + rsize, # radio max 
                             ZSize              = ecal_barrel_z,
                             X=0,Y=0,Z=0,
                             Visualization = True,
                             Color         = 'gray'
                             )


    nlayers=1; absorber=10*cm; gap=3*mm; rsize=nlayers*(absorber+gap)
    ecal_boundary_pv =   PhysicalVolume( Name               = "ECalBoundary", 
                                         Plates             = Plates.Horizontal, # Logical type
                                         AbsorberMaterial   = "G4_Al", # absorber
                                         GapMaterial        = "Vacuum", # gap
                                         NofLayers          = nlayers, # layers
                                         AbsorberThickness  = absorber, # abso
                                         GapThickness       = gap, # gap
                                         RMin               = 198*cm, #crack_em_pv.RMax, #228.3*cm, # radio min,
                                         RMax               = 198*cm + rsize, # radio max 
                                         ZSize              = ecal_barrel_z,
                                         X=0,Y=0,Z=0,
                                         Visualization = True,
                                         Color         = 'gray'
                                         )

    nlayers=1; absorber=10*cm; gap=3*mm; rsize=nlayers*(absorber+gap)
    tilecal_boundary_pv =   PhysicalVolume( Name               = "TileCalBoundary", 
                                            Plates             = Plates.Horizontal, # Logical type
                                            AbsorberMaterial   = "G4_Al", # absorber
                                            GapMaterial        = "Vacuum", # gap
                                            NofLayers          = nlayers, # layers
                                            AbsorberThickness  = absorber, # abso
                                            GapThickness       = gap, # gap
                                            RMin               = 218*cm, #crack_em_pv.RMax, #228.3*cm, # radio min,
                                            RMax               = 218*cm + rsize, # radio max 
                                            ZSize              = tile_barrel_z,
                                            X=0,Y=0,Z=0,
                                            Visualization = True,
                                            Color         = 'gray'
                                            )

    return [dm_pv, ecal_boundary_pv, tilecal_boundary_pv]
