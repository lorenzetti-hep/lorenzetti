#!/usr/bin/env python3

from GaugiKernel.constants import *
from GaugiKernel           import LoggingLevel, Logger
from G4Kernel              import *
from CaloCell.CaloDefs     import CaloSampling
from RootStreamBuilder     import recordable
import argparse
import sys,os,gc,traceback


mainLogger = Logger.getModuleLogger("job")
parser = argparse.ArgumentParser(description = '', add_help = False)
parser = argparse.ArgumentParser()


parser.add_argument('-i','--inputFile', action='store', dest='inputFile', required = True,
                    help = "The event input file generated by the Pythia event generator.")

parser.add_argument('-o','--outputFile', action='store', dest='outputFile', required = True,
                    help = "The reconstructed event file generated by lzt/geant4 framework.")

parser.add_argument('-nt','--numberOfThreads', action='store', dest='numberOfThreads', required = False, type=int, default=1,
                    help = "The number of threads")

parser.add_argument('--evt','--numberOfEvents', action='store', dest='numberOfEvents', required = False, type=int, default=None,
                    help = "The number of events to apply the reconstruction.")

parser.add_argument('--enableMagneticField', action='store_true', dest='enableMagneticField',required = False, 
                    help = "Enable the magnetic field.")

parser.add_argument('-t','--timeout', action='store', dest='timeout', required = False, type=int, default=120,
                    help = "Event timeout in minutes")

parser.add_argument('-l', '--outputLevel', action='store', dest='outputLevel', required = False, type=str, default='INFO',
                    help = "The output level messenger.")

parser.add_argument('-c','--command', action='store', dest='command', required = False, default="''",
                    help = "The preexec command")

if len(sys.argv)==1:
  parser.print_help()
  sys.exit(1)

args = parser.parse_args()

outputLevel = LoggingLevel.toC(args.outputLevel)

try:
  
  exec(args.command)

  from ATLAS import ATLASConstruction as ATLAS
  # Build the ATLAS detector
  detector = ATLAS( UseMagneticField = args.enableMagneticField )

  acc = ComponentAccumulator("ComponentAccumulator", detector,
                              NumberOfThreads = args.numberOfThreads,
                              OutputFile      = args.outputFile,
                              Timeout         = args.timeout * MINUTES )
  
  gun = EventReader( "EventReader", args.inputFile,
                     # outputs
                     OutputEventKey   = recordable("Events"   ),
                     OutputTruthKey   = recordable("Particles"),
                     OutputSeedKey    = recordable("Seeds"    ),
                     )

  from CaloCellBuilder import CaloHitBuilder
  calorimeter = CaloHitBuilder("CaloHitBuilder",
                                HistogramPath = "Expert/Hits",
                                OutputLevel   = outputLevel,
                                InputEventKey = recordable("Events"),
                                OutputHitsKey = recordable("Hits")
                                )
  gun.merge(acc)
  calorimeter.merge(acc)

  
  from RootStreamBuilder import RootStreamHITMaker, recordable
  HIT = RootStreamHITMaker( "RootStreamHITMaker",
                             OutputLevel     = outputLevel,
                             # input from context
                             InputHitsKey    = recordable("Hits"),
                             InputEventKey   = recordable("Events"),
                             InputTruthKey   = recordable("Particles"),
                             InputSeedsKey   = recordable("Seeds"),
                             )
  acc += HIT
  acc.run(args.numberOfEvents)

  sys.exit(0)
  
except  Exception as e:
  traceback.print_exc()
  mainLogger.error(e)
  sys.exit(1)
