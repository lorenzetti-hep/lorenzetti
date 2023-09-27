

__all__ = ["getHECCfg"]

from GaugiKernel.constants import *
from G4Kernel.DetectorConstruction import *
from G4Kernel.DetectorConstruction import SensitiveCaloVolume as SensitiveVolume
from CaloCell.CaloDefs import Detector, CaloSampling
from CaloCellBuilder import Calorimeter
import numpy as np
import os



def getHECCfg(left_side=False):

    sign = -1 if left_side else 1
    side_name = 'B' if left_side else 'A'
    
    basepath = os.environ['LZT_PATH']+'/geometry/ATLAS/data'
    hec_start = 4262*mm

    #
    # HEC1
    #
    nlayers=9; absorber=12.5*mm; gap=8.5*mm; zsize=nlayers*(absorber+gap)
    hec1_zsize = zsize
    hec1_pv    =  PhysicalVolume(  Name               = "FCal::HEC1::"+side_name, 
                                   Plates             = Plates.Vertical, # Logical type
                                   AbsorberMaterial   = "G4_Cu", # absorber
                                   GapMaterial        = "liquidArgon", # gap
                                   NofLayers          = nlayers, # 16s (max 16)
                                   AbsorberThickness  = absorber, # abso
                                   GapThickness       = gap, # gap
                                   RMin               = 372*mm,# radio min,
                                   RMax               = 2030*mm,# radio max 
                                   ZSize              = zsize ,
                                   X=0,Y=0,Z= sign * (hec_start + zsize/2), # x,y,z 
                                   Visualization = True,
                                   Color         = 'salmon'
                                  ) 

   
    hec1_sv0 = SensitiveVolume( hec1_pv, EtaMax = sign*2.50         , Segment = 0, DeltaEta = 0.1, DeltaPhi = pi/32 )
    hec1_sv1 = SensitiveVolume( hec1_pv, EtaMin = hec1_sv0.EtaMax   , Segment = 1, DeltaEta = 0.2, DeltaPhi = pi/16 )



    # Configure the electronic frontend and the detector parameters
    hec1_det0  = Calorimeter( hec1_sv0, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey = "Collection_HEC1_0" + side_name, # collection key
                              Detector        = Detector.TTHEC, # detector type
                              Sampling        = CaloSampling.HEC1, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 250*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )
    # Configure the electronic frontend and the detector parameters
    hec1_det1  = Calorimeter( hec1_sv1, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey = "Collection_HEC1_1"+ side_name, # collection key
                              Detector        = Detector.TTHEC, # detector type
                              Sampling        = CaloSampling.HEC1, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 250*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )



    #
    # HEC2
    #
    nlayers=16; absorber=12.5*mm; gap=8.5*mm; zsize=nlayers*(absorber+gap)
    hec2_zsize = zsize
    hec2_pv  =  PhysicalVolume(  Name               = "FCal::HEC2::"+side_name, 
                                 Plates             = Plates.Vertical, # Logical type
                                 AbsorberMaterial   = "G4_Cu", # absorber
                                 GapMaterial        = "liquidArgon", # gap
                                 NofLayers          = nlayers, # 16s (max 16)
                                 AbsorberThickness  = absorber, # abso
                                 GapThickness       = gap, # gap
                                 RMin               = 475*mm,# radio min,
                                 RMax               = 2030*mm,# radio max 
                                 ZSize              = zsize ,
                                 X=0,Y=0,Z= sign * (hec_start + hec1_zsize + hec1_zsize +zsize/2), # x,y,z 
                                 Visualization = True,
                                 Color         = 'violetred'
                             ) 


    hec2_sv0 = SensitiveVolume( hec2_pv, EtaMax = sign*2.50         , Segment = 0, DeltaEta = 0.1, DeltaPhi = pi/32 )
    hec2_sv1 = SensitiveVolume( hec2_pv, EtaMin = hec2_sv0.EtaMax   , Segment = 1, DeltaEta = 0.2, DeltaPhi = pi/16 )


    # Configure the electronic frontend and the detector parameters
    hec2_det0  = Calorimeter( hec2_sv0, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey = "Collection_HEC2_0" + side_name, # collection key
                              Detector        = Detector.TTHEC, # detector type
                              Sampling        = CaloSampling.HEC2, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 400*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )
    # Configure the electronic frontend and the detector parameters
    hec2_det1  = Calorimeter( hec2_sv1, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey = "Collection_HEC2_1"+ side_name, # collection key
                              Detector        = Detector.TTHEC, # detector type
                              Sampling        = CaloSampling.HEC2, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 400*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )





    #
    # HEC3
    #
    nlayers=16; absorber=50*mm; gap=8.5*mm; zsize=nlayers*(absorber+gap)
    hec3_zsize = zsize
    hec3_pv  =  PhysicalVolume(  Name               = "FCal::HEC3::"+side_name, 
                                 Plates             = Plates.Vertical, # Logical type
                                 AbsorberMaterial   = "G4_Cu", # absorber
                                 GapMaterial        = "liquidArgon", # gap
                                 NofLayers          = nlayers, # 16s (max 16)
                                 AbsorberThickness  = absorber, # abso
                                 GapThickness       = gap, # gap
                                 RMin               = 475*mm,# radio min,
                                 RMax               = 2030*mm,# radio max 
                                 ZSize              = zsize ,
                                 X=0,Y=0,Z= sign * (hec_start + hec1_zsize + hec1_zsize + hec2_zsize + zsize/2), # x,y,z 
                                 Visualization = True,
                                 Color         = 'orangered'
                             ) 


    hec3_sv0 = SensitiveVolume( hec3_pv, EtaMax = sign*2.50         , Segment = 0, DeltaEta = 0.1, DeltaPhi = pi/32 )
    hec3_sv1 = SensitiveVolume( hec3_pv, EtaMin = hec3_sv0.EtaMax   , Segment = 1, DeltaEta = 0.2, DeltaPhi = pi/16 )

    # Configure the electronic frontend and the detector parameters
    hec3_det0  = Calorimeter( hec3_sv0, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey = "Collection_HEC3_0" + side_name, # collection key
                              Detector        = Detector.TTHEC, # detector type
                              Sampling        = CaloSampling.HEC3, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 750*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )
    # Configure the electronic frontend and the detector parameters
    hec3_det1  = Calorimeter( hec3_sv1, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey = "Collection_HEC3_1"+ side_name, # collection key
                              Detector        = Detector.TTHEC, # detector type
                              Sampling        = CaloSampling.HEC3, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 750*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )
    
    hec1 = [hec1_det0, hec1_det1]
    hec2 = [hec2_det0, hec2_det1]
    hec3 = [hec3_det0, hec3_det1]

    return [hec1, hec2, hec3]
   