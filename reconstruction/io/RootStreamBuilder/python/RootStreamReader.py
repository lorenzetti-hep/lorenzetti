__all__ = ["RootStreamReader"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue
from RootStreamBuilder import recordable

class RootStreamReader( Logger ):


  def __init__( self, name: str, InputFile: str, 
                      NtupleName  : str="CollectionTree",
                      OutputLevel : int=1 ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    self.__core = ROOT.RootStreamReader(name)

    # cpp configuration
    self.setProperty( "InputFile"   , InputFile   )
    self.setProperty( "NtupleName"  , NtupleName  )
    self.setProperty( "OutputLevel" , OutputLevel )

    f = ROOT.TFile( self.InputFile, "read")
    t = f.Get( self.NtupleName )
    self.__entries = t.GetEntries()
    MSG_INFO(self,f"Getting a total of {self.__entries} events into the ntuple.")

  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if self.core().hasProperty(key):
      setattr( self, key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if hasattr(self, key):
      return getattr( self, key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)


  def GetEntries(self):
    return self.__entries


  def merge(self, acc):
    acc.SetReader(self)
