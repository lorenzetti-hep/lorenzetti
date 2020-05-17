
__all__ = ["CaloCellBuilder"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
from G4Kernel import treatPropertyValue, recordable
import os






class CaloCellBuilder( Logger ):

  # ( CollectionKey, Detector Cell File, Pulse Shaper File )
  __configs = [

      # Lar Barrel Calorimter, EM1
      { 'CollectionKey'   : "Collection_EM1",
        'CaloCellFile'    : "detector_sampling_1.dat",
        'ShaperFile'      : "pulseLar.dat",
        'NSamples'        : 5,
        'BunchIdStart'    : -24, # -800ns
        'BunchIdEnd'      : 5, # +175ns
        'OFWeights'       : [-0.0000853580,0.265132,0.594162,0.389505,0.124353],
      },
      # Lar Barrel Calorimter, EM2
      { 'CollectionKey'   : "Collection_EM2",
        'CaloCellFile'    : "detector_sampling_2.dat",
        'ShaperFile'      : "pulseLar.dat",
        'NSamples'        : 5,
        'BunchIdStart'    : -24, # -800ns
        'BunchIdEnd'      : 5, # +175ns
        'OFWeights'       : [-0.0000853580,0.265132,0.594162,0.389505,0.124353],
      },
      # Lar Barrel Calorimter, EM3
      { 'CollectionKey'   : "Collection_EM3",
        'CaloCellFile'    : "detector_sampling_3.dat",
        'ShaperFile'      : "pulseLar.dat",
        'NSamples'        : 5,
        'BunchIdStart'    : -24, # -800ns
        'BunchIdEnd'      : 5, # +175ns
        'OFWeights'       : [-0.0000853580,0.265132,0.594162,0.389505,0.124353],
      },
      # Tile Barrel Calorimter, HAD1
      { 'CollectionKey'   : "Collection_HAD1",
        'CaloCellFile'    : "detector_sampling_4.dat",
        'ShaperFile'      : "pulseTile.dat",
        'NSamples'        : 7,
        'BunchIdStart'    : -8, # -200ns
        'BunchIdEnd'      : 7, # +175ns
        'OFWeights'       : [-0.3781, -0.3572, 0.1808, 0.8125, 0.2767, -0.2056, -0.3292],
      },
      # Tile Barrel Calorimter, HAD2
      { 'CollectionKey'   : "Collection_HAD2",
        'CaloCellFile'    : "detector_sampling_5.dat",
        'ShaperFile'      : "pulseTile.dat",
        'NSamples'        : 7,
        'BunchIdStart'    : -8, # -200ns
        'BunchIdEnd'      : 7, # +175ns
        'OFWeights'       : [-0.3781, -0.3572, 0.1808, 0.8125, 0.2767, -0.2056, -0.3292],
      },
      # Tile Barrel Calorimter, HAD1
      { 'CollectionKey'   : "Collection_HAD3",
        'CaloCellFile'    : "detector_sampling_6.dat",
        'ShaperFile'      : "pulseTile.dat",
        'NSamples'        : 7,
        'BunchIdStart'    : -8, # -200ns
        'BunchIdEnd'      : 7, # +175ns
        'OFWeights'       : [-0.3781, -0.3572, 0.1808, 0.8125, 0.2767, -0.2056, -0.3292],
      },

     ] 

            

  __basepath = os.environ['LZT_PATH']+'/geometry/DetectorATLASModel/data/'


  def __init__( self, name, HistogramPath = "Expert", OutputLevel=1):

    Logger.__init__(self)
    self.__recoAlgs = []
    self.__histpath = HistogramPath
    self.__outputLevel = OutputLevel
    self.configure()




  def configure(self):


    from CaloRec import CaloCellMaker, CaloCellMerge, PulseGenerator, OptimalFilter
    for idx, config in enumerate( self.__configs ):
      pulse = PulseGenerator( "PulseGenerator", 
                              NSamples    = config['NSamples'], 
                              ShaperFile  = self.__basepath+config['ShaperFile'],
                              OutputLevel = self.__outputLevel)
      of = OptimalFilter("OptimalFilter",
                          Weights  = config['OFWeights'],
                          OutputLevel=self.__outputLevel)

      alg = CaloCellMaker("CaloCellMaker", 
                          CollectionKey           = recordable( config['CollectionKey'] ), 
                          EventKey                = recordable( "EventInfo" ), 
                          CaloCellFile            = self.__basepath+config['CaloCellFile'], 
                          BunchIdStart            = config['BunchIdStart'],
                          BunchIdEnd              = config['BunchIdEnd'],
                          BunchDuration           = 25,
                          NumberOfSamplesPerBunch = 1,
                          HistogramPath           = self.__histpath,
                          OutputLevel             = self.__outputLevel)
      alg.Tools = [pulse, of]
      self.__recoAlgs.append( alg )
  

    collectionKeys =  [ recordable(config["CollectionKey"]) for config in self.__configs ]
    
    # Merge all collection into a container and split between truth and reco
    mergeAlg = CaloCellMerge( "CaloCellMerge" , 
                              CollectionKeys  = collectionKeys,
                              CellsKey        = recordable("Cells"),
                              TruthCellsKey   = recordable("TruthCells"),
                              OutputLevel     = self.__outputLevel )

    self.__recoAlgs.append( mergeAlg )



  def merge( self, acc ):
    for reco in self.__recoAlgs:
      acc+=reco 















