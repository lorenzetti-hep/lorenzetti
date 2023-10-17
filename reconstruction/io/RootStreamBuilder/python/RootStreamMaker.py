
__all__ = ["RootStreamHITMaker","RootStreamESDMaker", "RootStreamAODMaker"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue
from RootStreamBuilder import RootStreamEDMList, RootStreamFlags


class RootStreamMaker( Logger ):

  def __init__( self, name: str, edm: str, 
                      EtaWindow   : int = RootStreamFlags.EtaWindow, 
                      PhiWindow   : int = RootStreamFlags.PhiWindow,
                      OnlyRoI     : bool= RootStreamFlags.OnlyRoI,
                      OutputLevel : int = 0,
                      ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    self.__core = ROOT.RootStreamMaker(name)
    
    self.EDMFormat = edm
    # cpp core configuration and python var set
    self.setProperty( "OnlyRoI"     , OnlyRoI     )
    self.setProperty( "EtaWindow"   , EtaWindow   )
    self.setProperty( "PhiWindow"   , PhiWindow   )
    self.setProperty( "OutputLevel" , OutputLevel )

    # cpp core configuration only
    containers = [stream[0] for stream in RootStreamEDMList if self.EDMFormat in stream[1]]
    self.setProperty("Containers", containers )


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


#
# format types
#


class RootStreamHITMaker(RootStreamMaker):
  def __init__(self, name, **kw):
    RootStreamMaker.__init__(self, name, 'HIT', **kw)

class RootStreamESDMaker(RootStreamMaker):
  def __init__(self, name, **kw):
    RootStreamMaker.__init__(self, name, 'ESD', **kw)

class RootStreamAODMaker(RootStreamMaker):
  def __init__(self, name, **kw):
    RootStreamMaker.__init__(self, name, 'AOD', **kw)


