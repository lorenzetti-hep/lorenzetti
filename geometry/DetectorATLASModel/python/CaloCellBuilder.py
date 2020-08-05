
__all__ = ["CaloCellBuilder"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
from G4Kernel import treatPropertyValue, recordable
import os






class CaloCellBuilder( Logger ):

  # ( CollectionKey, Detector Cell File, Pulse Shaper File )
  __configs = [

      # Lar Barrel Calorimter, EM1
      { 'name'            : 'barrel_em1',
        'CollectionKey'   : "Collection_EM1",
        'CaloCellFile'    : "detector_sampling_1.dat",
        'ShaperFile'      : "pulseLar.dat",
        'BunchIdStart'    : -21, # -525ns
        'BunchIdEnd'      : 3, # +75ns
        'StartSamplingBC' : -2,
        'NSamples'        : 5,
        'OFWeights'       : [0., 0.1816,    0.6840,    0.3569,   -0.0245],
      },
      # Lar Barrel Calorimter, EM2
      { 'name'            : 'barrel_em2',
        'CollectionKey'   : "Collection_EM2",
        'CaloCellFile'    : "detector_sampling_2.dat",
        'ShaperFile'      : "pulseLar.dat",
        'BunchIdStart'    : -21, # -525ns
        'BunchIdEnd'      : 3, # +75ns
        'StartSamplingBC' : -2,
        'NSamples'        : 5,
        'OFWeights'       : [0., 0.1208,    0.7654,    0.3189,   -0.1652],
      },
      # Lar Barrel Calorimter, EM3
      { 'name'            : 'barrel_em3',
        'CollectionKey'   : "Collection_EM3",
        'CaloCellFile'    : "detector_sampling_3.dat",
        'ShaperFile'      : "pulseLar.dat",
        'BunchIdStart'    : -21, # -525ns
        'BunchIdEnd'      : 3, # +75ns
        'StartSamplingBC' : -2,
        'NSamples'        : 5,
        'OFWeights'       : [0., 0.2341,    0.6148,    0.3847,    0.1071],
      },
      # Tile Barrel Calorimter, HAD1
      { 'name'            : 'barrel_had1',
        'CollectionKey'   : "Collection_HAD1",
        'CaloCellFile'    : "detector_sampling_4.dat",
        'ShaperFile'      : "pulseTile.dat",
        'BunchIdStart'    : -6, # -150ns
        'BunchIdEnd'      : 4, # 100ns
        'StartSamplingBC' : -3,
        'NSamples'        : 7,
        'OFWeights'       : [-0.0828,   -0.3598,    0.0467,    0.9619,    0.1593,   -0.3346,   -0.3906],
      },
      # Tile Barrel Calorimter, HAD2
      { 'name'            : 'barrel_had2',
        'CollectionKey'   : "Collection_HAD2",
        'CaloCellFile'    : "detector_sampling_5.dat",
        'ShaperFile'      : "pulseTile.dat",
        'BunchIdStart'    : -6, # -150ns
        'BunchIdEnd'      : 4, # 100ns
        'StartSamplingBC' : -3,
        'NSamples'        : 7,
        'OFWeights'       : [-0.3246,   -0.3411,    0.1496,    0.8439,    0.2554,   -0.2330,   -0.3502],
      },
      # Tile Barrel Calorimter, HAD3
      { 'name'            : 'barrel_had3',
        'CollectionKey'   : "Collection_HAD3",
        'CaloCellFile'    : "detector_sampling_6.dat",
        'ShaperFile'      : "pulseTile.dat",
        'BunchIdStart'    : -6, # -150ns
        'BunchIdEnd'      : 4, # 100ns
        'StartSamplingBC' : -3,
        'NSamples'        : 7,
        'OFWeights'       : [-0.4099,   -0.3620,    0.1891,    0.8020,    0.2825,   -0.1801,   -0.3217],
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


    from CaloRecBuilder import CaloCellMaker, CaloCellMerge, PulseGenerator, OptimalFilter
    for idx, config in enumerate( self.__configs ):
      pulse = PulseGenerator( "PulseGenerator", 
                              NSamples        = config['NSamples'], 
                              ShaperFile      = self.__basepath+config['ShaperFile'],
                              OutputLevel     = self.__outputLevel,
                              SamplingRate    = 25.0,
                              Pedestal        = 0.0,
                              DeformationMean = 0.0, 
                              DeformationStd  = 0.0,
                              NoiseMean       = 0.0,
                              NoiseStd        = 20.0, # MeV
                              StartSamplingBC = config['StartSamplingBC'], 
                              #NoiseStd        = 0,
                              )
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
                          HistogramPath           = self.__histpath + '/' + config['name'],
                          OutputLevel             = self.__outputLevel,
                          DetailedHistograms      = True)
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















