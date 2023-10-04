
__all__ = ["getLArBarrelCfg"]


#
# ATLAS Detector construction
#
from GaugiKernel.constants import *
from G4Kernel.DetectorConstruction import *
from G4Kernel.DetectorConstruction import SensitiveCaloVolume as SensitiveVolume
from CaloCell.CaloDefs import Detector, CaloSampling
from CaloCellBuilder import Calorimeter
import numpy as np
import os




def getLArBarrelCfg():

  basepath = os.environ['LZT_PATH']+'/geometry/ATLAS/data'

  # ECal
  ecal_barrel_start = 0*m
  ecal_barrel_end   = 3.4*m
  ecal_barrel_z     = (ecal_barrel_start + ecal_barrel_end) * 2


  psb_pv =  PhysicalVolume( Name               = "LAr::PSB",
                            Plates             = Plates.Horizontal, # Logical type
                            AbsorberMaterial   = "Vacuum", # absorber
                            GapMaterial        = "liquidArgon", # gap
                            NofLayers          = 1, # layers
                            AbsorberThickness  =  0.01*mm, # abso
                            GapThickness       = 1.1*cm, # gap
                            RMin               = 146*cm, # radio min,
                            RMax               = 146*cm + 1*(0.01*mm + 1.1*cm), # radio max 
                            ZSize              = ecal_barrel_z ,# z (3.4 left and 3.4 right)
                            X=0,Y=0,Z=0, # x,y,z (center in 0,0,0)
                            # Visualization
                            Visualization = True,
                            Color         = 'orange'
                          )

  emb1_pv = PhysicalVolume( Name               = "LAr::EMB1", 
                            Plates             = Plates.Horizontal, # Logical type
                            AbsorberMaterial   = "G4_Pb", # absorber
                            GapMaterial        = "liquidArgon", # gap
                            NofLayers          = 16, # layers
                            AbsorberThickness  = 1.51*mm, # abso
                            GapThickness       = 4.49*mm, # gap
                            RMin               = 150*cm, # radio min,
                            RMax               = 150*cm + 16*(1.51*mm + 4.49*mm), # radio max 
                            ZSize              = ecal_barrel_z ,# z (3.4 left and 3.4 right)
                            X=0,Y=0,Z=0, # x,y,z (center in 0,0,0)
                            Visualization = True,
                            Color         = 'aquamarine'
                          )

  emb2_pv = PhysicalVolume( Name               = "LAr::EMB2", 
                            Plates             = Plates.Horizontal, # Logical type
                            AbsorberMaterial   = "G4_Pb", # absorber
                            GapMaterial        = "liquidArgon", # gap
                            NofLayers          = 55, # layers
                            AbsorberThickness  = 1.7*mm, # abso
                            GapThickness       = 4.3*mm, # gap
                            RMin               = emb1_pv.RMax, # radio min,
                            RMax               = emb1_pv.RMax + 55*(1.7*mm + 4.3*mm), # radio max 
                            ZSize              = ecal_barrel_z ,# z (3.4 left and 3.4 right)
                            X=0,Y=0,Z=0, # x,y,z (center in 0,0,0)
                            Visualization = True,
                            Color         = 'cornflowerblue'
                          )

  emb3_pv = PhysicalVolume( Name               = "LAr::EMB3", 
                            Plates             = Plates.Horizontal, # Logical type
                            AbsorberMaterial   = "G4_Pb", # absorber
                            GapMaterial        = "liquidArgon", # gap
                            NofLayers          = 9, # layers
                            AbsorberThickness  = 1.7*mm, # abso
                            GapThickness       = 4.3*mm, # gap
                            RMin               = emb2_pv.RMax, # radio min,
                            RMax               = emb2_pv.RMax + 9*(1.7*mm + 4.3*mm), # radio max 
                            ZSize              = ecal_barrel_z ,# z (3.4 left and 3.4 right)
                            X=0,Y=0,Z=0, # x,y,z (center in 0,0,0)
                            Visualization = True,
                            Color         = 'cyan'
                          )



  psb_sv  = SensitiveVolume( psb_pv , DeltaEta = 0.025  , DeltaPhi = pi/32  )
  emb1_sv = SensitiveVolume( emb1_pv, DeltaEta = 0.00325, DeltaPhi = pi/32  )
  emb2_sv = SensitiveVolume( emb2_pv, DeltaEta = 0.025  , DeltaPhi = pi/128 )
  emb3_sv = SensitiveVolume( emb3_pv, DeltaEta = 0.050  , DeltaPhi = pi/128 )



  # Configure the electronic frontend and the detector parameters
  psb_det  = Calorimeter( psb_sv, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                          CollectionKey   = "Collection_PSB", # collection key
                          Detector        = Detector.TTEM, # detector type
                          Sampling        = CaloSampling.PSB, # sampling type
                          Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                          Noise           = 90*MeV, # electronic noise
                          Samples         = 5, # how many samples
                          OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                          OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                        )
  # Configure the electronic frontend and the detector parameters
  emb1_det = Calorimeter( emb1_sv, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                          CollectionKey   = "Collection_EMB1", # collection key
                          Detector        = Detector.TTEM, # detector type
                          Sampling        = CaloSampling.EMB1, # sampling type
                          Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                          Noise           = 26*MeV, # electronic noise
                          Samples         = 5, # how many samples
                          OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                          OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                        )
  # Configure the electronic frontend and the detector parameters
  emb2_det = Calorimeter( emb2_sv, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                          CollectionKey   = "Collection_EMB2", # collection key
                          Detector        = Detector.TTEM, # detector type
                          Sampling        = CaloSampling.EMB2, # sampling type
                          Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                          Noise           = 60*MeV, # electronic noise
                          Samples         = 5, # how many samples
                          OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                          OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                        )
  # Configure the electronic frontend and the detector parameters
  emb3_det = Calorimeter( emb3_sv, -21, 3, -2, # sensitive volume, bunch start, bunch end, sampling start,
                          CollectionKey   = "Collection_EMB3", # collection key
                          Detector        = Detector.TTEM, # detector type
                          Sampling        = CaloSampling.EMB3, # sampling type
                          Shaper          = basepath + "/pulseLar.dat", # pulse shaper
                          Noise           = 40*MeV, # electronic noise
                          Samples         = 5, # how many samples
                          OFWeightsEnergy = [-0.0000853580,    0.265132,    0.594162,     0.389505,     0.124353], # optimal filter parameters for energy estimation
                          OFWeightsTime   = [-0.0000853580,   -12.870312690734863, -27.39136505126953, 8.075883865356445, 13.768877029418945] # EMB1 ATLAS sample to test (fake first number to add to 5) 
                        )


  return [psb_det, emb1_det, emb2_det, emb3_det]


