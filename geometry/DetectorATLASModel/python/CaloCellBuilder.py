
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
        'OFWeights'       : [ -0.0720, 0.2191, 0.6351, 0.3738, 0.0762],
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
        'OFWeights'       : [ -0.1108, 0.2146, 0.6378, 0.3856, 0.0360],
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
        'OFWeights'       : [ -0.0125, 0.2379, 0.6097, 0.3863, 0.1183],
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
        'OFWeights'       : [ -0.3683, -0.3389, 0.1549, 0.8386, 0.2539, -0.2149, -0.3253],
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
        'OFWeights'       : [ -0.3808, -0.3594, 0.1807, 0.8108, 0.2796, -0.1997, -0.3311],
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
        'OFWeights'       : [ -0.3892, -0.3554, 0.1847, 0.8053, 0.2893, -0.2156,-0.3191] ,
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















