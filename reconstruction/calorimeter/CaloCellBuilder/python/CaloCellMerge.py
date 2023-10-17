__all__ = ["CaloCellMerge"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class CaloCellMerge( Logger ):


  def __init__( self, name, 
                InputCollectionKeys : str="Collection" ,
                OutputCellsKey      : str="Cells",
                OutputTruthCellsKey : str="TruthCells",
                OutputLevel         : int=0 
                ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CaloCellMerge
    # Create the algorithm
    self.__core = CaloCellMerge(name)
    self.setProperty( "InputCollectionKeys" , InputCollectionKeys ) 
    self.setProperty( "OutputCellsKey"      , OutputCellsKey      ) 
    self.setProperty( "OutputTruthCellsKey" , OutputTruthCellsKey ) 
    self.setProperty( "OutputLevel"         , OutputLevel         ) 

 

  def core(self):
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










