
from CaloRec import *
from G4Kernel import ComponentAccumulator
import ROOT
import os



acc = ComponentAccumulator("ComponentAccumulator", VisMacro = "init_vis.mac", NumberOfThreads = 1)


calorimeter = CaloCellBuilder("CaloCellBuilder")

calorimeter.merge(acc)


cluster = CaloClusterMaker( "CaloClusterMaker",
                            TruthCellsKey   = recordable("TruthCells"),
                            CellsKey        = recordable("Cells"),
                            EventKey        = recordable("EventInfo"),
                            ClusterKey      = recordable("Clusters"),
                            TruthClusterKey = recordable("TruthClusters"),
                            TruthKey        = recordable("Truth"),
                            EnergyThreshold = 3*GeV, 
                            EtaWindow       = 0.4,
                            PhiWindow       = 0.4,
                            DeltaR          = 0.15,
                            ForceTruthMatch = False,
                            HistogramPath   = "/Expert"
                            )
acc+= cluster



ringer = CaloRingerBuilder( "CaloRingerBuilder",
                            RingerKey     = recordable("Rings"),
                            ClusterKey    = recordable("Clusters"),
                            DeltaEtaRings = [0.00325, 0.025, 0.050, 0.1, 0.1, 0.2 ],
                            DeltaPhiRings = [pi/32, pi/128, pi/128, pi/128, pi/32, pi/32, pi/32],
                            NRings        = [64, 8, 8, 4, 4, 4],
                            LayerRings    = [CaloSample.EM1, CaloSample.EM2, CaloSample.EM3, CaloSample.HAD1, CaloSample.HAD2, CaloSample.HAD3],
                            )

acc+=ringer



truth_ringer = CaloRingerBuilder( "CaloRingerBuilder",
                            RingerKey     = recordable("TruthRings"),
                            ClusterKey    = recordable("TruthClusters"),
                            DeltaEtaRings = [0.00325, 0.025, 0.050, 0.1, 0.1, 0.2 ],
                            DeltaPhiRings = [pi/32, pi/128, pi/128, pi/128, pi/32, pi/32, pi/32],
                            NRings        = [64, 8, 8, 4, 4, 4],
                            LayerRings    = [CaloSample.EM1, CaloSample.EM2, CaloSample.EM3, CaloSample.HAD1, CaloSample.HAD2, CaloSample.HAD3],
                            )


acc+= truth_ringer




#acc.run('run_gun.mac')
acc.run()



