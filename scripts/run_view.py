#!/usr/bin/env python3
from Gaugi.messenger    import LoggingLevel, Logger
from Gaugi              import GeV
from CaloRingerBuilder  import CaloRingerBuilder
from CaloRec            import *
from G4Kernel           import *
import numpy as np
import argparse
import sys,os


acc = ComponentAccumulator("ComponentAccumulator", 
                            RunVis=True, 
                            NumberOfThreads = 1, 
                            OutputFile = 'test.root')


gun = ParticleGun( "ParticleGun",
                   EventKey   = recordable("EventInfo"),
                   Energy     = 10*GeV,
                   Sigma      = 1*GeV,
                   Particle   = 'e-',
                   EtaMax     = 0.8)



#calorimeter = CaloCellBuilder("CaloCellBuilder", 
#                              HistogramPath = "Expert/CaloCells",
#                              OutputLevel   = args.outputLevel)
#
#
#
#cluster = CaloClusterMaker( "TruthCaloClusterMaker",
#                            CellsKey        = recordable("TruthCells"),
#                            EventKey        = recordable("EventInfo"),
#                            ClusterKey      = recordable("TruthClusters"),
#                            TruthKey        = recordable("Truth"),
#                            EtaWindow       = 0.4,
#                            PhiWindow       = 0.4,
#                            HistogramPath   = "Expert/TruthClusters",
#                            OutputLevel     = args.outputLevel)
#
#
#
#pi = np.pi
#ringer = CaloRingerBuilder( "CaloRingerBuilder",
#                            RingerKey     = recordable("Rings"),
#                            ClusterKey    = recordable("Clusters"),
#                            DeltaEtaRings = [0.00325, 0.025, 0.050, 0.1, 0.1, 0.2 ],
#                            DeltaPhiRings = [pi/32, pi/128, pi/128, pi/128, pi/32, pi/32, pi/32],
#                            NRings        = [64, 8, 8, 4, 4, 4],
#                            LayerRings    = [1,2,3,4,5,6],
#                            HistogramPath = "Expert/Ringer",
#                            OutputLevel   = args.outputLevel)
#
#
#truth_ringer = CaloRingerBuilder( "TruthCaloRingerBuilder",
#                                  RingerKey       = recordable("TruthRings"),
#                                  ClusterKey      = recordable("TruthClusters"),
#                                  DeltaEtaRings   = [0.00325, 0.025, 0.050, 0.1, 0.1, 0.2 ],
#                                  DeltaPhiRings   = [pi/32, pi/128, pi/128, pi/128, pi/32, pi/32, pi/32],
#                                  NRings          = [64, 8, 8, 4, 4, 4],
#                                  LayerRings      = [1,2,3,4,5,6],
#                                  HistogramPath   = "Expert/TruthRinger",
#                                  OutputLevel     = args.outputLevel)
#
#
#ntuple = CaloNtupleMaker( "CaloNtupleMaker",
#                          EventKey        = recordable("EventInfo"),
#                          RingerKey       = recordable("Rings"),
#                          TruthRingerKey  = recordable("TruthRings"),
#                          ClusterKey      = recordable("Clusters"),
#                          TruthClusterKey = recordable("TruthClusters"),
#                          DeltaR          = 0.15,
#                          DumpCells       = True,
#                          OutputLevel     = args.outputLevel)
#                          
#
gun.merge(acc)
#calorimeter.merge(acc)
#acc+= cluster
#acc+= truth_ringer
##acc+=ringer
#acc += ntuple
acc.run(1)





