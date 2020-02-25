



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
alg.setCard( basepath+"/core/data/detector.card" )



castor = RunCastor( "output.root", 1 )
castor.push_back( alg )
castor.run(macro)




