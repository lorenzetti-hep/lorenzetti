
__all__ = ["CaloHitBuilder"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue, recordable
import os


#
# Calo cell builder
#
class CaloHitBuilder( Logger ):


  # basepath
  __basepath = os.environ['LZT_PATH']+'/geometry/DetectorATLASModel/data/'


  def __init__( self, name, 
                      HistogramPath  = "Expert", 
                      OutputLevel    = 1,
                      ):

    Logger.__init__(self)
    self.__recoAlgs = []
    self.__histpath = HistogramPath
    self.__outputLevel = OutputLevel

    # configure
    self.configure()



  #
  # Configure 
  #
  def configure(self):

    MSG_INFO(self, "Configure CaloHitBuilder.")
    
    from CaloCellBuilder import CaloHitMaker, CaloHitMerge

    collectionKeys = []
    # Get the layer cell configuration
    from DetectorATLASModel import create_ATLAS_layers 
    layers = create_ATLAS_layers(self.__basepath)

    for layer_id, layer in enumerate( layers ):
      for sampling in layer:
        for seg in sampling.segments:

          MSG_INFO(self, "Create new CaloHitMaker and dump all hits into %s collection", seg.CollectionKey)
          alg = CaloHitMaker("CaloHitMaker", 
                              # input key
                              EventKey                = recordable( "EventInfo" ), 
                              # output key
                              CollectionKey           = seg.CollectionKey, 
                              # Hits grid configuration
                              EtaBins                 = seg.EtaBins,
                              PhiBins                 = seg.PhiBins,
                              RMin                    = seg.RMin,
                              RMax                    = seg.RMax,
                              Sampling                = seg.Sampling,
                              Segment                 = seg.Segment,
                              Detector                = seg.Detector,
                              # Bunch crossing configuration
                              BunchIdStart            = seg.BunchIdStart,
                              BunchIdEnd              = seg.BunchIdEnd,
                              BunchDuration           = 25, #ns
                              # monitoring configuration
                              HistogramPath           = self.__histpath + '/' + seg.name,
                              OutputLevel             = self.__outputLevel,
                              DetailedHistograms      = False, # Use True when debug with only one thread
                              )

          self.__recoAlgs.append( alg )
          print(seg.CollectionKey)
          collectionKeys.append( seg.CollectionKey )


    MSG_INFO(self, "Create CaloHitMerge and dump all hit colelctions into %s container", "Hits")
    # Merge all collection into a container and split between truth and reco
    mergeAlg = CaloHitMerge( "CaloHitMerge" , 
                              CollectionKeys  = collectionKeys,
                              HitsKey         = recordable("Hits"),
                              OutputLevel     = self.__outputLevel )

    self.__recoAlgs.append( mergeAlg )



  def merge( self, acc ):
    for reco in self.__recoAlgs:
      acc+=reco 













