

__all__ = ["CaloHitMaker"]


from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue



class CaloHitMaker( Logger ):


  def __init__( self, name, 
                OutputCollectionKey  : str    = "",
                EtaBins              : list   = [],
                PhiBins              : list   = [],
                RMin                 : float  = ,
                RMax                 : float  = ,
                ZMin                 : float  = ,
                ZMax                 : float  = ,
                Sampling             : int    = ,
                Segment              : int    = ,
                Detector             : int    = ,
                BunchIdStart         : int    = ,
                BunchIdEnd           : int    = ,
                BunchDuration        : float  = ,
                OutputLevel          : int    = ,
                DetailedHistograms   : bool   = ,
                HistogramPath        : str    = ,
                SamplingNoiseStd     : float  = ,
              ):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CaloHitMaker
    # Create the algorithm
    self.Tools = []

    self.__core = CaloHitMaker(name)
    self.setProperty("OutputCollectionKey"    ,OutputCollectionKey  )
    self.setProperty("EtaBins"                ,EtaBins              )
    self.setProperty("PhiBins"                ,PhiBins              )
    self.setProperty("RMin"                   ,RMin                 )
    self.setProperty("RMax"                   ,RMax                 )
    self.setProperty("ZMin"                   ,ZMin                 )
    self.setProperty("ZMax"                   ,ZMax                 )
    self.setProperty("Sampling"               ,Sampling             )
    self.setProperty("Segment"                ,Segment              )
    self.setProperty("Detector"               ,Detector             )
    self.setProperty("BunchIdStart"           ,BunchIdStart         )
    self.setProperty("BunchIdEnd"             ,BunchIdEnd           )
    self.setProperty("BunchDuration"          ,BunchDuration        )
    self.setProperty("OutputLevel"            ,OutputLevel          )
    self.setProperty("DetailedHistograms"     ,DetailedHistograms   )
    self.setProperty("HistogramPath"          ,HistogramPath        )
    self.setProperty("SamplingNoiseStd"       ,SamplingNoiseStd     )


  def core(self):
    # Attach all tools before return the core
    for tool in self.Tools:
      self.__core.push_back(tool.core())
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__core.hasProperty(key):
      setattr( self, key , value )
      self.__core.setProperty( key, treatPropertyValue(value) )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if hasattr(self, key):
      return getattr( self, key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)


  def __add__( self, tool ):
    self.Tools += tool
    return self
  

