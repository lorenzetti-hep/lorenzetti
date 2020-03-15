



import ROOT
ROOT.gSystem.Load('liblorenzetti')
from ROOT import CaloCellMaker, CaloClusterMaker, Simulator
import os
basepath = os.environ['CASTOR']

macro=""

rec = CaloCellMaker( "CaloCellMaker" )
rec.SetCalibPath( basepath+'/reco/CaloRec/data' )
clus = CaloClusterMaker( "CaloClusterMaker" )



sim = Simulator( "output.root", 1 )
sim.vis( 'init_vis.mac' )
sim.push_back( rec )
sim.push_back( clus )
sim.run(macro)




