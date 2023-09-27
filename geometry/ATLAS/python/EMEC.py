
__all__ = ["getLArEMECCfg"]

from GaugiKernel.constants import *
from G4Kernel.DetectorConstruction import *
from G4Kernel.DetectorConstruction import SensitiveCaloVolume as SensitiveVolume
from CaloCell.CaloDefs import Detector, CaloSampling
from CaloCellBuilder import Calorimeter
import numpy as np
import os




def getLArEMECCfg(left_side=False):

    sign = -1 if left_side else 1
    side_name = 'B' if left_side else 'A'
    basepath = os.environ['LZT_PATH']+'/geometry/ATLAS/data'

    endcap_start    = 3704.*mm
    ps_endcap_start = endcap_start + 31*mm


    #
    # PSE
    #
    pse_pv      =  PhysicalVolume( Name               = "LAr::PSE::"+side_name, 
                                   Plates             = Plates.Vertical, # Logical type
                                   AbsorberMaterial   = "Vacuum", # absorber
                                   GapMaterial        = "liquidArgon", # gap
                                   NofLayers          = 1, # 16s
                                   AbsorberThickness  = 0.01*mm, # abso
                                   GapThickness       = 4.99*mm, # gap
                                   RMin               = 1232*mm, # radio min,
                                   RMax               = 1700*mm, # radio max 
                                   ZSize              = 1*(0.01*mm + 4.99*mm) ,
                                   X=0,Y=0,Z=sign*(ps_endcap_start + 0.5*(1*(0.01*mm + 4.99*mm))), # x,y,z 
                                   Visualization = True,
                                   Color         = 'orange'
                                )


    pde_sv = SensitiveVolume( pse_pv, DeltaEta = 0.025  , DeltaPhi = pi/32  )


    # Configure the electronic frontend and the detector parameters
    pse_det  = Calorimeter( pde_sv, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                        CollectionKey   = "Collection_PSE_"+ side_name, # collection key
                        Detector        = Detector.TTEM, # detector type
                        Sampling        = CaloSampling.PSE, # sampling type
                        Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                        Noise           = 26*MeV, # electronic noise
                        Samples         = 5, # how many samples
                        OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                        OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                        
                      )



    #
    # EMEC1
    #
    nlayers = 16; absorber=2.27*mm; gap=3.73*mm; zsize=nlayers*(absorber+gap)
    emec1_pv   =  PhysicalVolume( Name               = "LAr::EMEC1::"+side_name, 
                                  Plates             = Plates.Vertical, # Logical type
                                  AbsorberMaterial   = "G4_Pb", # absorber
                                  GapMaterial        = "liquidArgon", # gap
                                  NofLayers          = nlayers, # 16s (max 16)
                                  AbsorberThickness  = absorber, # abso
                                  GapThickness       = gap, # gap
                                  RMin               = 302*mm,# radio min,
                                  RMax               = 2042*mm,# radio max 
                                  ZSize              = zsize,
                                  X=0, Y=0, Z= (pse_pv.ZMin - zsize/2) if left_side else (pse_pv.ZMax + zsize/2),
                                  Visualization = True,
                                  Color         = 'aquamarine'
                              ) 

    emec1_sv0 = SensitiveVolume( emec1_pv, EtaMax = sign*1.80                           , Segment = 0, DeltaEta = 0.00325, DeltaPhi = pi/32 )
    emec1_sv1 = SensitiveVolume( emec1_pv, EtaMin = emec1_sv0.EtaMax, EtaMax = sign*2.00, Segment = 1, DeltaEta = 0.025  , DeltaPhi = pi/32 )
    emec1_sv2 = SensitiveVolume( emec1_pv, EtaMin = emec1_sv1.EtaMax, EtaMax = sign*2.37, Segment = 2, DeltaEta = 0.006  , DeltaPhi = pi/32 )
    emec1_sv3 = SensitiveVolume( emec1_pv, EtaMin = emec1_sv2.EtaMax                    , Segment = 3, DeltaEta = 0.1    , DeltaPhi = pi/32 )

    # Configure the electronic frontend and the detector parameters
    emec1_det0  = Calorimeter( emec1_sv0, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey   = "Collection_EMEC1_0" + side_name, # collection key
                              Detector        = Detector.TTEM, # detector type
                              Sampling        = CaloSampling.EMEC1, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 26*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )
    # Configure the electronic frontend and the detector parameters
    emec1_det1  = Calorimeter( emec1_sv1, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey   = "Collection_EMEC1_1"+ side_name, # collection key
                              Detector        = Detector.TTEM, # detector type
                              Sampling        = CaloSampling.EMEC1, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 26*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )
    # Configure the electronic frontend and the detector parameters
    emec1_det2  = Calorimeter( emec1_sv2, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey   = "Collection_EMEC1_2"+ side_name, # collection key
                              Detector        = Detector.TTEM, # detector type
                              Sampling        = CaloSampling.EMEC1, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 26*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )
    # Configure the electronic frontend and the detector parameters
    emec1_det3  = Calorimeter( emec1_sv3, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey   = "Collection_EMEC1_3"+ side_name, # collection key
                              Detector        = Detector.TTEM, # detector type
                              Sampling        = CaloSampling.EMEC1, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 26*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )


    #
    # EMEC2
    #
    nlayers=55; absorber=2.27*mm; gap=3.73*mm; zsize=nlayers*(absorber+gap)
    emec2_pv   =  PhysicalVolume( Name               = "LAr::EMEC2::"+side_name, 
                                  Plates             = Plates.Vertical, # Logical type
                                  AbsorberMaterial   = "G4_Pb", # absorber
                                  GapMaterial        = "liquidArgon", # gap
                                  NofLayers          = nlayers, # 16s (max 16)
                                  AbsorberThickness  = absorber, # abso
                                  GapThickness       = gap, # gap
                                  RMin               = 302*mm,# radio min,
                                  RMax               = 2042*mm,# radio max 
                                  ZSize              = zsize ,
                                  X=0,Y=0,Z=(emec1_pv.ZMin - zsize/2) if left_side else (emec1_pv.ZMax + zsize/2), # x,y,z 
                                  Visualization = True,
                                  Color         = 'cornflowerblue'
                              ) 
 
    emec2_sv0 = SensitiveVolume( emec2_pv, EtaMax = sign*2.50                    , Segment = 0, DeltaEta = 0.025, DeltaPhi = pi/128 )
    emec2_sv1 = SensitiveVolume( emec2_pv, EtaMin = emec2_sv0.EtaMax             , Segment = 1, DeltaEta = 0.1  , DeltaPhi = pi/32  )


    # Configure the electronic frontend and the detector parameters
    emec2_det0  = Calorimeter( emec2_sv0, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey   = "Collection_EMEC2_0" + side_name, # collection key
                              Detector        = Detector.TTEM, # detector type
                              Sampling        = CaloSampling.EMEC2, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 60*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )
    # Configure the electronic frontend and the detector parameters
    emec2_det1  = Calorimeter( emec2_sv1, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey   = "Collection_EMEC2_1"+ side_name, # collection key
                              Detector        = Detector.TTEM, # detector type
                              Sampling        = CaloSampling.EMEC2, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 60*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )





    #
    # EMEC3
    #
    nlayers=9; absorber=2.41*mm; gap=3.59*mm; zsize=nlayers*(absorber+gap)
    emec3_pv   =  PhysicalVolume( Name               = "LAr::EMEC3::"+side_name, 
                                  Plates             = Plates.Vertical, # Logical type
                                  AbsorberMaterial   = "G4_Pb", # absorber
                                  GapMaterial        = "liquidArgon", # gap
                                  NofLayers          = nlayers, # 16s (max 16)
                                  AbsorberThickness  = absorber, # abso
                                  GapThickness       = gap, # gap
                                  RMin               = 302*mm,# radio min,
                                  RMax               = 2042*mm,# radio max 
                                  ZSize              = zsize ,
                                  X=0,Y=0,Z=(emec2_pv.ZMin - zsize/2) if left_side else (emec2_pv.ZMax + zsize/2), # x,y,z 
                                  Visualization = True,
                                  Color         = 'cyan'
                              ) 
 
    emec3_sv0 = SensitiveVolume( emec3_pv, EtaMax = sign*2.50                    , Segment = 0, DeltaEta = 0.050 , DeltaPhi = pi/128 )
    emec3_sv1 = SensitiveVolume( emec3_pv, EtaMin = emec3_sv0.EtaMax             , Segment = 1, DeltaEta = 0.1   , DeltaPhi = pi/32 )


    # Configure the electronic frontend and the detector parameters
    emec3_det0  = Calorimeter( emec3_sv0, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey   = "Collection_EMEC3_0" + side_name, # collection key
                              Detector        = Detector.TTEM, # detector type
                              Sampling        = CaloSampling.EMEC3, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 40*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )
    # Configure the electronic frontend and the detector parameters
    emec3_det1  = Calorimeter( emec3_sv1, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                              CollectionKey   = "Collection_EMEC3_1"+ side_name, # collection key
                              Detector        = Detector.TTEM, # detector type
                              Sampling        = CaloSampling.EMEC3, # sampling type
                              Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                              Noise           = 40*MeV, # electronic noise
                              Samples         = 5, # how many samples
                              OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                              OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                            )

    pse   = [pse_det]
    emec1 = [emec1_det0, emec1_det1, emec1_det2, emec1_det3]
    emec2 = [emec2_det0, emec2_det1]
    emec3 = [emec3_det0, emec3_det1]

    return [pse,emec1,emec2,emec3]







