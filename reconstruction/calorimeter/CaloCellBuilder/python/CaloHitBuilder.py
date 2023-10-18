
__all__ = ["CaloHitBuilder"]

from GaugiKernel import Logger, LoggingLevel
from GaugiKernel.macros import *
from CaloCellBuilder import CaloHitMaker
from CaloCellBuilder import CaloHitMerge



#
# Calo cell builder
#
class CaloHitBuilder( Logger ):


  def __init__( self, name, 
                InputEventKey  : str,
                OutputHitsKey  : str,
                HistogramPath  : str="Expert", 
                OutputLevel    : int=LoggingLevel.toC('INFO'),
              ):

    Logger.__init__(self)
    self.__recoAlgs = []
    self.HistogramPath = HistogramPath
    self.OutputLevel   = OutputLevel
    self.OutputHitsKey = OutputHitsKey
    self.OutputCollectionKeys = []


  def configure(self):

    MSG_INFO(self, "Configure CaloHitBuilder.")
    

    for samp in self.__detector.samplings:

          MSG_INFO(self, "Create new CaloHitMaker and dump all hits into %s collection", samp.CollectionKey)
          alg = CaloHitMaker("CaloHitMaker", samp,
                              OutputCollectionKey     = samp.CollectionKey, 
                              SamplingNoiseStd        = samp.Noise, # TOF selection
                              HistogramPath           = self.HistogramPath + '/' + samp.name(),
                              OutputLevel             = self.OutputLevel,
                              DetailedHistograms      = False, # Use True when debug with only one thread
                              )
                              
          self.__recoAlgs.append( alg )
          self.OutputCollectionKeys.append( samp.CollectionKey )


    MSG_INFO(self, "Create CaloHitMerge and dump all hit colelctions into %s container", "Hits")
    # Merge all collection into a container and split between truth and reco
    mergeAlg = CaloHitMerge( "CaloHitMerge" , 
                              InputCollectionKeys  = self.OutputCollectionKeys,
                              OutputHitsKey        = self.OutputHitsKey,
                              OutputLevel          = self.OutputLevel )
    self.__recoAlgs.append( mergeAlg )

  

  def merge( self, acc ):

    self.__detector = acc.detector()
    self.configure()
    for reco in self.__recoAlgs:
      acc+=reco 













