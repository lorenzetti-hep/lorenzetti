
from G4Kernel import ComponentAccumulator
#from CaloRec import CaloClusterMaker, CaloCellMaker, PulseGenerator, OptimalFilter
import os
import ROOT
ROOT.gSystem.Load('liblorenzetti')

basepath = os.environ['LZT_PATH']+'/reconstruction/CaloRec/data'



acc = ComponentAccumulator("ComponentAccumulator", VisMacro = "init_vis.mac", NumberOfThreads = 4)


#from CaloRec import CaloCellBuilder
#cells = CaloCellBuilder("CaloCellBuilder")
#cells.merge(acc)

#CollectionKeys = cells.keys() 

#cluster = CaloClusterMaker( "CaloClusterMaker",
#                            CollectionKeys = CollectionKeys
#                            )
#acc+= cluster



#acc.run('run_gun.mac')
acc.run()



