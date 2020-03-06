



import ROOT
ROOT.gSystem.Load('libcastor')
from ROOT import CaloCellMaker, CaloClusterMaker,Simulator


import os
basepath = os.environ['CASTOR']
print (basepath)

#macro = 'run_gun.mac'
macro = 'run_jf17.mac'



rec = CaloCellMaker( "CaloCellMaker" )

clus = CaloClusterMaker( "CaloClusterMaker" )
print (basepath)
rec.SetCalibPath( basepath+'/reco/CaloRec/data' )



#
## Calo cell reconstuction configuration
#alg.setCard( basepath+"/core/data/detector.card" )
#alg.set_bc_id_start( -8 )
#alg.set_bc_id_end( 7 )
#alg.set_bc_nsamples( 1 )
#alg.set_bc_duration( 25 )

sim = Simulator( "output.root", 1 )
sim.push_back( rec )
sim.push_back( clus )
sim.run(macro)




