

from PythiaGenerator import JF17
from Gaugi import GeV


generator =JF17( "JF17",
                 OutputFile   = 'pythia_jf17',
                 MinbiasFile  = 'data/minbias_jet_config.cmnd',
                 MainFile     = 'data/jet_config.cmnd',
                 EtaMax       = 1.4,
                 MinPt        = 17*GeV,
                 PileupAvg    = 0,
                 Select       = 2,
                 BunchIdStart = -8,
                 BunchIdEnd   = 7,
                 )

generator.run()



