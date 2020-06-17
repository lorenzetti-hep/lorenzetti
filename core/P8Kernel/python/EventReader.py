__all__ = ["EventReader"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
from G4Kernel import treatPropertyValue


class EventReader( Logger ):

  __allow_keys = [
                  "EventKey", 
                  "FileName",
                  ]


  def __init__( self, name, **kw ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzett')
    from ROOT import generator
    # Create the algorithm
    self.__core = generator.EventReader(name)

    for key, value in kw.items():
      self.setProperty( key,value )

    from ROOT import TFile, TTree
    f = TFile( self.getProperty("FileName") )
    t = f.Get("particles")
    self.__nevents = t.GetEntries()
    f.Close()



  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      setattr( self, '__' + key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)


  def merge( self, acc ):
    acc.setNumberOfEvents(self.__nevents )
    acc.core().setGenerator( self.core() )


