

from PythiaGenerator import Zee
from Gaugi import GeV


generator = Zee( "Zee",
                 OutputFile   = 'pythia_zee',
                 MinbiasFile  = 'minbias_zee_config.cmnd',
                 MainFile     = 'zee_config.cmnd',
                 EtaMax       = 1.4,
                 MinPt        = 15*GeV,
                 PileupAvg    = 40,
                 Select       = 2,
                 BunchIdStart = -8,
                 BunchIdEnd   = 7,
                 )

generator.run()



