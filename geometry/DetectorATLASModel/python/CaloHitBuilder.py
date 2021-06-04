
__all__ = ["CaloHitBuilder"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
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

    from CaloHitBuilder import CaloHitMaker, CaloHitMerge

    collectionKeys = []
 
    # Get the layer cell configuration
    from DetectorATLASModel import create_ATLAS_layers 
    layers = create_ATLAS_layers()
    
    for layer_id, layer in enumerate( layers ):

      for sampling in layer:

        for seg in sampling.segments():

          alg = CaloHitMaker("CaloHitMaker", 
                              CollectionKey           = recordable( seg.CollectionKey ) + "_Hits", 
                              EventKey                = recordable( "EventInfo" ), 
                              CaloHitFile             = self.__basepath+seg.CaloCellFile, # same as cell
                              BunchIdStart            = seg.BunchIdStart,
                              BunchIdEnd              = seg.BunchIdEnd,
                              BunchDuration           = 25, #ns
                              HistogramPath           = self.__histpath + '/' + seg.name,
                              OutputLevel             = self.__outputLevel,
                              DetailedHistograms      = False, # Use True when debug with only one thread
                              )

          self.__recoAlgs.append( alg )
          collectionKeys.append( seg.CollectionKey + "_Hits" )


    
    # Merge all collection into a container and split between truth and reco
    mergeAlg = CaloHitMerge( "CaloHitMerge" , 
                              CollectionKeys  = collectionKeys,
                              HitsKey         = recordable("Hits"),
                              OutputLevel     = self.__outputLevel )

    self.__recoAlgs.append( mergeAlg )



  def merge( self, acc ):
    for reco in self.__recoAlgs:
      acc+=reco 















