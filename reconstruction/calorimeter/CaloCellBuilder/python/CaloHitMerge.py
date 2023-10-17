__all__ = ["CaloHitMerge"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class CaloHitMerge( Logger ):

  def __init__( self, name, 
                InputCollectionKeys : list= [],
                OutputHitsKey       : str = "Hits",
                OutputLevel         : int = 10,
                ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CaloHitMerge
    # Create the algorithm
    self.__core = CaloHitMerge(name)
    self.setProperty( "InputCollectionKeys" , InputCollectionKeys )
    self.setProperty( "OutputHitsKey"       , OutputHitsKey       ) 
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













