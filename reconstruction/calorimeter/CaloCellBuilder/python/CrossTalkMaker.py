__all__ = ["CrossTalkMaker"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue
from CaloCellBuilder import CaloFlags

class CrossTalkMaker( Logger ):

  def __init__( self, name, 
                InputCollectionKey  : str="Cells",
                OutputCollectionKey : str="XTCells",
                MinEnergy           : float=CaloFlags.XTMinEnergy,
                XTAmpCapacitive     : float=CaloFlags.XTAmpCapacitive,
                XTAmpInductive      : float=CaloFlags.XTAmpInductive,
                XTAmpResistive      : float=CaloFlags.XTAmpResistive,
                HistogramPath       : str="/CrossTalkMaker",
                OutputLevel         : int=0,
                ):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CrossTalkMaker as core
    # Create the algorithm
    self.__core = CrossTalkMaker(name)
    self.Tools = []
    self.setProperty( "InputCollectionKey"    , InputCollectionKey  )
    self.setProperty( "OutputCollectionKey"   , OutputCollectionKey )
    self.setProperty( "MinEnergy"             , MinEnergy           )
    self.setProperty( "XTAmpCapacitive"       , XTAmpCapacitive     )
    self.setProperty( "XTAmpInductive"        , XTAmpInductive      )
    self.setProperty( "XTAmpResistive"        , XTAmpResistive      )
    self.setProperty( "HistogramPath"         , HistogramPath       )
    self.setProperty( "OutputLevel"           , OutputLevel         )

 

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
