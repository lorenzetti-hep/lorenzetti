__all__ = ["OptimalFilter"]


from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class OptimalFilter(Logger):
  
  def __init__( self, name,
                WeightsEnergy : list=[],
                WeightsTime   : list=[],
                OutputLevel   : int=0 
              ):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager, OptimalFilter
    self.__core = OptimalFilter(name)
    self.setProperty( "OutputLevel"   , OutputLevel   )
    self.setProperty( "WeightsEnergy" , WeightsEnergy )
    self.setProperty( "WeightsTime"   , WeightsTime   )

 
  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if key in self.core().hasProperty(key):
      setattr( self, key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if hasattr(self, key):
      return getattr( self, key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)




