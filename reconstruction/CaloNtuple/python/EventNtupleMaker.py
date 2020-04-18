__all__ = ["EventNtupleMaker"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
from G4Kernel import treatPropertyValue


class EventNtupleMaker( Logger ):

  __allow_keys = [
                  "ClusterKey", 
                  "TruthClusterKey", 
                  "EventKey", 
                  "RingerKey", 
                  "TruthRingerKey", 
                  "OutputLevel", 
                  "DeltaR", 
                  ]


  def __init__( self, name, **kw ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import EventNtupleMaker
    self.__core = EventNtupleMaker(name)

    for key, value in kw.items():
      self.setProperty( key,value )


  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      setattr( self, '__' + key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for EventNtupleMaker object", key)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for EventNtupleMaker object", key)



