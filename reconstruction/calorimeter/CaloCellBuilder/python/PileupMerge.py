__all__ = ["PileupMerge"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class PileupMerge( Logger ):

  def __init__( self, name, 
                InputFile       : str,
                InputHitsKey    : str="Hits",
                OutputHitsKey   : str="Hits_Merged",
                InputEventKey   : str="EventInfo",
                OutputEventKey  : str="EventInfo_Merged",
                OutputLevel     : int=0,
                NtupleName      : str="CollectionTree"
              ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import PileupMerge
    self.__core = PileupMerge(name)
    self.setProperty( "InputHitsKey"  , InputHitsKey   )  
    self.setProperty( "OutputHitsKey" , OutputHitsKey  )
    self.setProperty( "InputEventKey" , InputEventKey  )
    self.setProperty( "OutputEventKey", OutputEventKey )
    self.setProperty( "OutputLevel"   , OutputLevel    ) 
    self.setProperty( "NtupleName"    , NtupleName     )
    self.setProperty( "InputFile"     , InputFile      )


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

