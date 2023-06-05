
__all__ = ["getCrackVolumesCfg", "getDMVolumesCfg"]

from G4Kernel.DetectorConstruction import *
from G4Kernel.DetectorConstruction import SensitiveCaloVolume as SensitiveVolume
from GaugiKernel.constants import *
from CaloCell.CaloDefs import Detector, CaloSampling
from CaloCellBuilder import Calorimeter
import numpy as np
import os



def getCrackVolumesCfg(left_side=False):


    sign = -1 if left_side else 1
    side_name = 'B' if left_side else 'A'
    
    endcap_start = 3704.*mm
    gap_between_extended_barrel = 68.*cm
    crack_material_size = gap_between_extended_barrel
    crack_start = endcap_start - gap_between_extended_barrel
    crack_em_start = 3400*mm + 35*mm/2



    nlayers=2; absorber=4.3*cm; gap=10.7*cm; zsize=nlayers*(absorber+gap)
    crack_em_pv =  PhysicalVolume( Name               = "DM::Crack::EM::"+side_name, 
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
    crack_tile_pv =  PhysicalVolume( Name               = "DM::Crack::TILE::"+side_name, 
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


    crack_em_pv.Cuts     = ProductionCuts(ElectronCut = 1, PositronCut = 1, GammaCut = 1)
    crack_tile_pv.Cuts   = ProductionCuts(ElectronCut = 1, PositronCut = 1, GammaCut = 1)

    return [crack_em_pv, crack_tile_pv]


def getDMVolumesCfg():

    ecal_barrel_start = 0*m
    ecal_barrel_end   = 3.4*m
    ecal_barrel_z     = (ecal_barrel_start + ecal_barrel_end) * 2

    endcap_start = 3704.*mm
    gap_between_extended_barrel = 68.*cm

    tile_barrel_start = 0*m
    tile_barrel_end = (endcap_start - gap_between_extended_barrel)
    tile_barrel_z = (tile_barrel_start + tile_barrel_end) * 2

    psb_rmin = 1460*mm

    nlayers=2; absorber=2*cm; gap=2*cm; rsize=nlayers*(absorber+gap)
    dm_pv =  PhysicalVolume( Name               = "DM::PS::Boundary", 
                             Plates             = Plates.Horizontal, # Logical type
                             AbsorberMaterial   = "G4_Al", # absorber
                             GapMaterial        = "Vacuum", # gap
                             NofLayers          = nlayers, # layers
                             AbsorberThickness  = absorber, # abso
                             GapThickness       = gap, # gap
                             RMin               = psb_rmin - rsize, #crack_em_pv.RMax, #228.3*cm, # radio min,
                             RMax               = psb_rmin, # radio max 
                             ZSize              = ecal_barrel_z,
                             X=0,Y=0,Z=0,
                             Visualization = True,
                             Color         = 'gray'
                             )


    nlayers=1; absorber=10*cm; gap=3*mm; rsize=nlayers*(absorber+gap)
    ecal_boundary_pv =   PhysicalVolume( Name               = "DM::LAr::Boundary", 
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
    tilecal_boundary_pv =   PhysicalVolume( Name               = "DM::TILE::Boundary", 
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


    dm_pv.Cuts                  = ProductionCuts(ElectronCut = 1, PositronCut = 1, GammaCut = 1)
    ecal_boundary_pv.Cuts       = ProductionCuts(ElectronCut = 1, PositronCut = 1, GammaCut = 1)
    tilecal_boundary_pv.Cuts    = ProductionCuts(ElectronCut = 1, PositronCut = 1, GammaCut = 1)

    return [dm_pv, ecal_boundary_pv, tilecal_boundary_pv]
