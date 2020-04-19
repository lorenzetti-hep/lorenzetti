
#from CaloRingerBuilder import *
from Gaugi import GeV
from CaloRec import *
from CaloRingerBuilder import CaloRingerBuilder
from G4Kernel import *
import os
import numpy as np
pi = np.pi

acc = ComponentAccumulator("ComponentAccumulator", RunVis=False, NumberOfThreads = 1, OutputFile = 'test')



from PythiaGenerator import EventReader
gun = EventReader( "PythiaGenerator",
                   EventKey   = recordable("EventInfo"),
                   FileName   = "pythia_zee.root",
                   #FileName   = "pythia_jf17.root",
                   OutputLevel = 6,
                   )
gun.merge(acc)





calorimeter = CaloCellBuilder("CaloCellBuilder", 
                              HistogramPath = "Expert/CaloCells",
                              OutputLevel   = 6)

calorimeter.merge(acc)



cluster = CaloClusterMaker( "CaloClusterMaker",
                            CellsKey        = recordable("TruthCells"),
                            EventKey        = recordable("EventInfo"),
                            ClusterKey      = recordable("TruthClusters"),
                            TruthKey        = recordable("Truth"),
                            EtaWindow       = 0.4,
                            PhiWindow       = 0.4,
                            HistogramPath   = "Expert/TruthClusters",
                            OutputLevel     = 6,
                            )
acc+= cluster



#ringer = CaloRingerBuilder( "CaloRingerBuilder",
#                            RingerKey     = recordable("Rings"),
#                            ClusterKey    = recordable("Clusters"),
#                            DeltaEtaRings = [0.00325, 0.025, 0.050, 0.1, 0.1, 0.2 ],
#                            DeltaPhiRings = [pi/32, pi/128, pi/128, pi/128, pi/32, pi/32, pi/32],
#                            NRings        = [64, 8, 8, 4, 4, 4],
#                            LayerRings    = [1,2,3,4,5,6],
#                            HistogramPath   = "Expert/Ringer"
#                            )
#
#acc+=ringer



truth_ringer = CaloRingerBuilder( "CaloRingerBuilder",
                            RingerKey     = recordable("TruthRings"),
                            ClusterKey    = recordable("TruthClusters"),
                            DeltaEtaRings = [0.00325, 0.025, 0.050, 0.1, 0.1, 0.2 ],
                            DeltaPhiRings = [pi/32, pi/128, pi/128, pi/128, pi/32, pi/32, pi/32],
                            NRings        = [64, 8, 8, 4, 4, 4],
                            LayerRings    = [1,2,3,4,5,6],
                            HistogramPath   = "Expert/TruthRinger",
                            OutputLevel     = 6,
                            )
acc+= truth_ringer

from CaloNtuple import CaloNtupleMaker

ntuple = CaloNtupleMaker( "CaloNtupleMaker",
                            EventKey        = recordable("EventInfo"),
                            RingerKey       = recordable("Rings"),
                            TruthRingerKey  = recordable("TruthRings"),
                            ClusterKey      = recordable("Clusters"),
                            TruthClusterKey = recordable("TruthClusters"),
                            DeltaR          = 0.15,
                            OutputLevel     = 6,
                            )
                          

#acc += ntuple



acc.run(3)



