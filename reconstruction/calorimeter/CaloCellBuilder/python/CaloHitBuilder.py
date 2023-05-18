
__all__ = ["CaloHitBuilder"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue
from RootStreamBuilder  import recordable

import os


#
# Calo cell builder
#
class CaloHitBuilder( Logger ):


  def __init__( self, name, 
                      HistogramPath  = "Expert", 
                      OutputLevel    = 1,
                      ):

    Logger.__init__(self)
    self.__recoAlgs = []
    self.HistogramPath = HistogramPath
    self.OutputLevel = OutputLevel


  #
  # Configure 
  #
  def configure(self):

    MSG_INFO(self, "Configure CaloHitBuilder.")
    
    from CaloCellBuilder import CaloHitMaker, CaloHitMerge
    collectionKeys = []

    for samp in self.__detector.samplings:

          MSG_INFO(self, "Create new CaloHitMaker and dump all hits into %s collection", samp.CollectionKey)
          alg = CaloHitMaker("CaloHitMaker", 
                              # input key
                              EventKey                = recordable( "EventInfo" ), 
                              # output key
                              CollectionKey           = samp.CollectionKey, 
                              # Hits grid configuration
                              EtaBins                 = samp.sensitive().EtaBins,
                              PhiBins                 = samp.sensitive().PhiBins,
                              RMin                    = samp.volume().RMin,
                              RMax                    = samp.volume().RMax,
                              ZMin                    = samp.volume().ZMin,
                              ZMax                    = samp.volume().ZMax,
                              Sampling                = samp.Sampling,
                              Detector                = samp.Detector,                    
                              Segment                 = samp.sensitive().Segment,
                              # Bunch crossing configuration
                              BunchIdStart            = samp.BunchIdStart,
                              BunchIdEnd              = samp.BunchIdEnd,
                              BunchDuration           = 25, #ns
                              # monitoring configuration
                              HistogramPath           = self.HistogramPath + '/' + samp.name(),
                              OutputLevel             = self.OutputLevel,
                              DetailedHistograms      = False, # Use True when debug with only one thread
                              )
          self.__recoAlgs.append( alg )
          collectionKeys.append( samp.CollectionKey )


    MSG_INFO(self, "Create CaloHitMerge and dump all hit colelctions into %s container", "Hits")
    # Merge all collection into a container and split between truth and reco
    mergeAlg = CaloHitMerge( "CaloHitMerge" , 
                              CollectionKeys  = collectionKeys,
                              HitsKey         = recordable("Hits"),
                              OutputLevel     = self.OutputLevel )
    self.__recoAlgs.append( mergeAlg )



  def merge( self, acc ):

    self.__detector = acc.detector()
    self.configure()
    for reco in self.__recoAlgs:
      acc+=reco 













