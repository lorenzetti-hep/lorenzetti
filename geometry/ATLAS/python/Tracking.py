
__all__ = ['getPixelBarrelCfg']

from GaugiKernel.constants import *
from SpacePoint.TrackingDefs import *
from G4Kernel.DetectorConstruction import *
import numpy as np
import os




def getPixelBarrelCfg():
    

    nlayers = 1
    absorber=0.150*mm
    gap=0.130*mm
    rsize=nlayers*(absorber+gap)

    pixel_barrel_end = 2.5*m
    pixel_barrel_size = pixel_barrel_end * 2
    pixel_barrel_layers = 3
    pixel_barrel_rmin = 10*cm
    pixel_barrel_gap_between_layers = 15*cm
    pixel_barrel_rmax = pixel_barrel_rmin + pixel_barrel_layers * (nlayers*(absorber+gap)) + (pixel_barrel_layers-1)*pixel_barrel_gap_between_layers
    pixel_endcap_layers = 3

    samplings=[]
    
    for idx in range(pixel_barrel_layers):

        rmin = pixel_barrel_rmin if idx==0 else (samplings[-1].volume().RMax + pixel_barrel_gap_between_layers)

        pv =  PhysicalVolume( Name               = "Tracking::Pixel" + str(idx), 
                              Plates             = Plates.Horizontal, # Logical type
                              AbsorberMaterial   = "G4_Si", # absorber
                              GapMaterial        = "Vacuum", # gap
                              NofLayers          = nlayers, # layers
                              AbsorberThickness  = absorber, # abso
                              GapThickness       = gap, # gap
                              RMin               = rmin, # radio min,
                              RMax               = rmin + rsize, # radio max 
                              ZSize              = pixel_barrel_size,
                              X=0,Y=0,Z=0,
                              Visualization      = True,
                              Color              = 'gray'
                            )
                              
        pv.Cuts = ProductionCuts(ElectronCut = 0.05, PositronCut = 0.05, GammaCut = 0.05)
        samplings.append(pv)

    return samplings



def getPixelEndCapCfg( left_side=False ):

    sign = -1 if left_side else 1
    side_name = 'B' if left_side else 'A'


    nlayers = 1
    absorber=0.150*mm
    gap=0.130*mm
    rsize=nlayers*(absorber+gap)

    pixel_barrel_end = 2.5*m
    pixel_barrel_size = pixel_barrel_end * 2
    pixel_barrel_layers = 3
    pixel_barrel_rmin = 10*cm
    pixel_barrel_gap_between_layers = 15*cm
    pixel_barrel_rmax = pixel_barrel_rmin + pixel_barrel_layers * (nlayers*(absorber+gap)) + (pixel_barrel_layers-1)*pixel_barrel_gap_between_layers
    pixel_endcap_layers = 3

    samplings = []
    z= pixel_barrel_end

    for pixel_idx in range(pixel_endcap_layers):

        nlayers=1; zsize=nlayers*(absorber+gap)
        pv =  PhysicalVolume( Name               = "Tracking::Pixel" + str(idx) + "::" + side_name, 
                              Plates             = Plates.Vertical, # Logical type
                              AbsorberMaterial   = "G4_Si", # absorber
                              GapMaterial        = "Vacuum", # gap
                              NofLayers          = nlayers, # layers
                              AbsorberThickness  = absorber, # abso
                              GapThickness       = gap, # gap
                              RMin               = pixel_barrel_rmin, # radio min,
                              RMax               = pixel_barrel_rmax, # radio max 
                              ZSize              = zsize,
                              X=0,Y=0,Z=sign * (z + pixel_barrel_gap_between_layers + zsize/2)  ,
                              Visualization = True,
                              Color         = 'gray'
                              )

        z = z + pixel_barrel_gap_between_layers + zsize
        pv.Cuts = ProductionCuts(ElectronCut = 0.5, PositronCut = 0.5, GammaCut = 0.5)
        samplings.append(pv)


    return samplings