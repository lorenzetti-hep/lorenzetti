__all__ = ["OptimalFilter"]


from Gaugi import Logger
from Gaugi.messenger.macros import *
from G4Kernel import treatPropertyValue


class OptimalFilter(Logger):

  __allow_keys = ["OutputLevel"]
  
  def __init__( self, name, **kw ):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager, OptimalFilter
    self.__core = OptimalFilter(name)
    for key, value in kw.items():
      self.setProperty( key, value )


  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      setattr( self, '__' + key , value )
      self.core().setProperty( key, value )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object", key)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object", key)

 



