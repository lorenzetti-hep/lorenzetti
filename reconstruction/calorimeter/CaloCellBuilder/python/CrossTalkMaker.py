__all__ = ["CrossTalkMaker"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from CaloCellBuilder import CaloFlags as flags

class CrossTalkMaker( Logger ):

  def __init__( self, name, 
                InputCollectionKey  : str="Cells",
                OutputCollectionKey : str="XTCells",
                MinEnergy           : float=flags.XTMinEnergy,
                XTAmpCapacitive     : float=flags.XTAmpCapacitive,
                XTAmpInductive      : float=flags.XTAmpInductive,
                XTAmpResistive      : float=flags.XTAmpResistive,
                HistogramPath       : str="/CrossTalkMaker",
                OutputLevel         : int=0,
                ):

    Cpp.__init__(self, name, "ROOT.CrossTalkMaker", OutputLevel=OutputLevel)
    self.Tools = []
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
      self.__core.push_back(tool.core())
    return self.__core


  def __add__( self, tool ):
    self.Tools += tool
    return self
