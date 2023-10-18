__all__ = ["CrossTalkMaker"]

from GaugiKernel import Cpp, LoggingLevel
from GaugiKernel.macros import *
from CaloCellBuilder import CaloFlags as flags
import ROOT

class CrossTalkMaker( Cpp ):

  def __init__( self, name          : str, 
                InputCollectionKey  : str="Cells",
                OutputCollectionKey : str="XTCells",
                MinEnergy           : float=flags.XTMinEnergy,
                XTAmpCapacitive     : float=flags.XTAmpCapacitive,
                XTAmpInductive      : float=flags.XTAmpInductive,
                XTAmpResistive      : float=flags.XTAmpResistive,
                HistogramPath       : str="/CrossTalkMaker",
                OutputLevel         : int=LoggingLevel.toC('INFO'),
                ):

    Cpp.__init__(self, ROOT.CrossTalkMaker(name) )
    self.Tools = []
    self.setProperty( "OutputLevel"           , OutputLevel         )
    self.setProperty( "InputCollectionKey"    , InputCollectionKey  )
    self.setProperty( "OutputCollectionKey"   , OutputCollectionKey )
    self.setProperty( "MinEnergy"             , MinEnergy           )
    self.setProperty( "XTAmpCapacitive"       , XTAmpCapacitive     )
    self.setProperty( "XTAmpInductive"        , XTAmpInductive      )
    self.setProperty( "XTAmpResistive"        , XTAmpResistive      )
    self.setProperty( "HistogramPath"         , HistogramPath       )

 
  def core(self):
    # Attach all tools before return the core
    for tool in self.Tools:
      self._core.push_back(tool.core())
    return self._core


  def __add__( self, tool ):
    self.Tools += tool
    return self
