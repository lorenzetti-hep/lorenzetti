

__all__ = ["CaloHitMaker"]


from GaugiKernel import Cpp
from GaugiKernel.macros import *



class CaloHitMaker( Cpp ):


  def __init__( self, name, sampling,
                OutputCollectionKey  : str    = "Hits",
                OutputLevel          : int    = 0,
                DetailedHistograms   : bool   = False,
                HistogramPath        : str    = "/CaloHitMaker",
                SamplingNoiseStd     : float  = 0,
              ):

    Cpp.__init__(self, name, "ROOT.CaloHitMaker", OutputLevel=OutputLevel)
    self.Tools = []
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
    self.setProperty( "DetailedHistograms"      , DetailedHistograms          )
    self.setProperty( "HistogramPath"           , HistogramPath               )
 

  def core(self):
    # Attach all tools before return the core
    for tool in self.Tools:
      self.__core.push_back(tool.core())
    return self.__core


  def __add__( self, tool ):
    self.Tools += tool
    return self
  

