
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

endcap_start = 3704.*mm
gap_between_extended_barrel = 68.*cm
crack_material_size = gap_between_extended_barrel
crack_start = endcap_start - gap_between_extended_barrel
crack_em_start = 3400*mm + 35*mm/2


def createCrackVolumes(left_side=False):


    sign = -1 if left_side else 1
    side_name = 'B' if left_side else 'A'




    nlayers=2; absorber=4.3*cm; gap=10.7*cm; zsize=nlayers*(absorber+gap)
    crack_em_pv =  PhysicalVolume( Name               = "Crack_EM_"+side_name, 
                                    Plates             = Plates.Vertical, # Logical type
                                    AbsorberMaterial   = "G4_Al", # absorber
                                    GapMaterial        = "liquidArgon", # gap
                                    NofLayers          = nlayers, # layers
                                    AbsorberThickness  = absorber, # abso
                                    GapThickness       = gap, # gap
                                    RMin               = 900*mm, # radio min,
                                    RMax               = 2232*mm, # radio max 
                                    ZSize              = zsize,
                                    X=0,Y=0,Z=sign * (crack_em_start + zsize/2),
                                    Visualization = True,
                                    Color         = 'gray'
                                    )


    nlayers=1; absorber=200*cm; gap=1*mm; rsize=nlayers*(absorber+gap)
    crack_tile_pv =  PhysicalVolume( Name               = "Crack_Tile_"+side_name, 
                                     Plates             = Plates.Horizontal, # Logical type
                                     AbsorberMaterial   = "G4_Al", # absorber
                                     GapMaterial        = "Vacuum", # gap
                                     NofLayers          = nlayers, # layers
                                     AbsorberThickness  = absorber, # abso
                                     GapThickness       = gap, # gap
                                     RMin               = 228.3*cm, #crack_em_pv.RMax, #228.3*cm, # radio min,
                                     RMax               = 228.3*cm + rsize, # radio max 
                                     ZSize              = crack_material_size,
                                     X=0,Y=0,Z=sign * (crack_start + crack_material_size/2),
                                     Visualization = True,
                                     Color         = 'gray'
                                     )

    return [crack_em_pv, crack_tile_pv]
