#!/usr/bin/env python3

from G4Kernel             import *
from RootStreamBuilder    import recordable
import sys,os,gc

try:

  from DetectorATLASModel import DetectorConstruction as ATLAS
  from DetectorATLASModel import CaloHitBuilder
  
  # Build the ATLAS detector
  detector = ATLAS("GenericATLASDetector", 
                   UseMagneticField = False, # Force to be false since the mag field it is not working yet
                   #UseMagneticField = True,
                   UseDeadMaterial=True, # cause
                   # PS,EM1,EM2,EM3
                   UseBarrel=True,
                   # HAD1,2,3
                   UseTile=True,
                   # HAD1,2,3 ext.
                   UseTileExt=True,
                   # EMECs
                   UseEMEC= True,
                   # HECs
                   UseHEC = True, # cause
                   # Tracking
                   UseTrack = True,
                   # crack region
                   UseCrack = True, # cause
                   CutOnPhi = True,
                   )

  acc = ComponentAccumulator("ComponentAccumulator", detector,
                              RunVis=True,
                              NumberOfThreads = 1,
                              Seed = 512,
                              OutputFile = ".output.root",
                              Timeout = 0 )
  
  gun = EventReader( "EventReader", 
                     FileName   = "", # Force empty
                     EventKey   = recordable("EventInfo"),
                     TruthKey   = recordable("Particles"),
                     )
  gun.merge(acc)
  acc.run(0)
  
  input("Press Enter to quit...")
  del acc
  os.system('rm .output.root')
  sys.exit(0)
  
except  Exception as e:
  print(e)
  sys.exit(1)
