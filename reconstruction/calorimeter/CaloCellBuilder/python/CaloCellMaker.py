

__all__ = ["CaloCellMaker"]


from GaugiKernel import Cpp
from GaugiKernel.macros import *



class CaloCellMaker( Cpp ):

  def __init__( self, name, sampling, 
                InputHitsKey        : str="Hits",
                OutputCollectionKey : str="Collection",
                OutputLevel         : int=0,
                DetailedHistograms  : bool=False,
                HistogramPath       : str="/Hists/Cells" ): 

    Cpp.__init__(self, name, "ROOT.CaloCellMaker", OutputLevel=OutputLevel)
    self.Tools = []
    self.PulseGenerator = None

    self.setProperty( "InputHitsKey"            , InputHitsKey                )
    self.setProperty( "OutputCollectionKey"     , OutputCollectionKey         )
    self.setProperty( "EtaBins"                 , sampling.sensitive().EtaBins)
    self.setProperty( "PhiBins"                 , sampling.sensitive().PhiBins)
    self.setProperty( "ZMin"                    , sampling.volume().ZMin      )
    self.setProperty( "ZMax"                    , sampling.volume().ZMax      )
    self.setProperty( "Sampling"                , sampling.Sampling           )
    self.setProperty( "Segment"                 , sampling.sensitive().Segment)
    self.setProperty( "Detector"                , sampling.Detector           )
    self.setProperty( "BunchIdStart"            , sampling.BunchIdStart       )
    self.setProperty( "BunchIdEnd"              , sampling.BunchIdEnd         )
    self.setProperty( "BunchDuration"           , 25                          )
    self.setProperty( "DetailedHistograms"      , DetailedHistograms          )
    self.setProperty( "HistogramPath"           , HistogramPath               )
 

  def core(self):
    # Attach all tools before return the core
    for tool in self.Tools:
      self.__core.push_back(tool.core())
    self.__core.setPulseGenerator(self.PulseGenerator.core())
    return self.__core


  def __add__( self, tool ):
    self.Tools += tool
    return self
  

