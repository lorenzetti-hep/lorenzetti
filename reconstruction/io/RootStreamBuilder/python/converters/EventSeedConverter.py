
__all__ = ["EventInfoConverter"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class EventInfoConverter( Logger ):

  def __init__( self, key : str): 

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    self.__core = ROOT.EventInfoConverter("EventInfoConverter")
    self.setProperty( "Key", key )


  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if self.core().hasProperty(key):
      setattr( self, key , value )
      try:
        self.core().setProperty( key, treatPropertyValue(value) )
      except:
        MSG_FATAL( self, "Property with name %s has some problem during type convertion in %s object", key, self.__class__.__name__)
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if hasattr(self, key):
      return getattr( self, key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

