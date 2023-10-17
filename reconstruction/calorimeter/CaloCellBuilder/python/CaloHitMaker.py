

__all__ = ["CaloHitMaker"]


from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue



class CaloHitMaker( Logger ):


  def __init__( self, name, sampling,
                OutputCollectionKey  : str    = "Hits",
                OutputLevel          : int    = 0,
                DetailedHistograms   : bool   = False,
                HistogramPath        : str    = "/CaloHitMaker",
                SamplingNoiseStd     : float  = 0,
              ):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CaloHitMaker
    # Create the algorithm
    self.Tools = []

    self.__core = CaloHitMaker(name)
    self.setProperty( "OutputCollectionKey"     , OutputCollectionKey         )
    self.setProperty( "EtaBins"                 , sampling.sensitive().EtaBins)
    self.setProperty( "PhiBins"                 , sampling.sensitive().PhiBins)
    self.setProperty( "RMin"                    , sampling.volume().RMin      )
    self.setProperty( "RMax"                    , sampling.volume().RMax      )
    self.setProperty( "ZMin"                    , sampling.volume().ZMin      )
    self.setProperty( "ZMax"                    , sampling.volume().ZMax      )
    self.setProperty( "Sampling"                , sampling.Sampling           )
    self.setProperty( "Segment"                 , sampling.sensitive().Segment)
    self.setProperty( "Detector"                , sampling.Detector           )
    self.setProperty( "BunchIdStart"            , sampling.BunchIdStart       )
    self.setProperty( "BunchIdEnd"              , sampling.BunchIdEnd         )
    self.setProperty( "BunchDuration"           , 25                          )
    self.setProperty( "SamplingNoiseStd"        , SamplingNoiseStd            )
    self.setProperty( "OutputLevel"             , OutputLevel                 )
    self.setProperty( "DetailedHistograms"      , DetailedHistograms          )
    self.setProperty( "HistogramPath"           , HistogramPath               )
 
    

  def core(self):
    # Attach all tools before return the core
    for tool in self.Tools:
      self.__core.push_back(tool.core())
    return self.__core


  def setProperty( self, key, value ):
    if self.__core.hasProperty(key):
      setattr( self, key , value )
      try:
        self.__core.setProperty( key, treatPropertyValue(value) )
      except:
        MSG_FATAL( self, f"Exception in property with name {key} and value: {value}")
    else:
      MSG_FATAL( self, f"Property with name {key} is not allow for this object")


  def getProperty( self, key ):
    if hasattr(self, key):
      return getattr( self, key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)


  def __add__( self, tool ):
    self.Tools += tool
    return self
  

