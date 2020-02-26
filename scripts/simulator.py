



import ROOT
ROOT.gSystem.Load('libcastor')
from ROOT import RunCastor
from ROOT import CaloCellMaker


import os
basepath = os.environ['CASTOR']
print (basepath)

#macro = 'run_gun.mac'
macro = 'run_jf17.mac'



alg = CaloCellMaker( "CaloCellMaker" )

# Calo cell reconstuction configuration
alg.setCard( basepath+"/core/data/detector.card" )
alg.set_bc_id_start( -8 )
alg.set_bc_id_end( 7 )
alg.set_bc_nsamples( 1 )
alg.set_bc_duration( 25 )




castor = RunCastor( "output.root", 1 )
castor.push_back( alg )
castor.run(macro)




