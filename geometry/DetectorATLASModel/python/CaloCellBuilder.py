
__all__ = ["CaloCellBuilder"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
from G4Kernel import treatPropertyValue, recordable
import os


#
# Calo cell builder
#
class CaloCellBuilder( Logger ):


  # basepath
  __basepath = os.environ['LZT_PATH']+'/geometry/DetectorATLASModel/data/'


  def __init__( self, name, HistogramPath = "Expert", OutputLevel=1):

    Logger.__init__(self)
    self.__recoAlgs = []
    self.__histpath = HistogramPath
    self.__outputLevel = OutputLevel
    self.configure()



  #
  # Configure 
  #
  def configure(self):

    from CaloRecBuilder import CaloCellMaker, CaloCellMerge, PulseGenerator, OptimalFilter
    from DetectorATLASModel import create_ATLAS_layers 
    collectionKeys = []

    layers = create_ATLAS_layers()

    for layer_id, layer in enumerate( layers ):

      for sample in layer:

        for seg in sample.segmentations:

          pulse = PulseGenerator( "PulseGenerator", 
                                  NSamples        = seg.NSamples, 
                                  ShaperFile      = self.__basepath+seg.ShaperFile,
                                  OutputLevel     = self.__outputLevel,
                                  SamplingRate    = 25.0,
                                  Pedestal        = 0.0,
                                  DeformationMean = 0.0, 
                                  DeformationStd  = 0.0,
                                  NoiseMean       = 0.0,
                                  NoiseStd        = seg.EletronicNoise,
                                  StartSamplingBC = seg.StartSamplingBC, 
                                )
          of = OptimalFilter("OptimalFilter",
                              Weights  = seg.OFWeights,
                              OutputLevel=self.__outputLevel)

          alg = CaloCellMaker("CaloCellMaker", 
                              CollectionKey           = recordable( seg.CollectionKey ), 
                              EventKey                = recordable( "EventInfo" ), 
                              CaloCellFile            = self.__basepath+seg.CaloCellFile, 
                              BunchIdStart            = seg.BunchIdStart,
                              BunchIdEnd              = seg.BunchIdEnd,
                              BunchDuration           = 25,
                              NumberOfSamplesPerBunch = 1,
                              HistogramPath           = self.__histpath + '/' + seg.name,
                              OutputLevel             = self.__outputLevel,
                              Layer                   = layer_id,
                              Section                 = 0 if layer_id < 4 else 1, # ECal = 0,1,2,3 and HCal = 4,5,6
                              DetailedHistograms      = False)
          alg.Tools = [pulse, of]
          self.__recoAlgs.append( alg )
          collectionKeys.append( seg.CollectionKey )


    
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















