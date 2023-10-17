__all__ = ["RootStreamHITReader","RootStreamESDReader", "RootStreamAODReader"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue
from RootStreamBuilder import recordable
from RootStreamBuilder import RootStreamEDMList, RootStreamFlags
import ROOT

class RootStreamReader( Logger ):


  def __init__( self, name: str, 
                      edm : str, 
                      InputFile: str, 
                      OutputLevel : int=1 ): 
    
    Logger.__init__(self)
    ROOT.gSystem.Load('liblorenzetti')
    self.__core = ROOT.RootStreamReader(name)
    self.EDMFormat = edm

    # cpp configuration
    self.setProperty( "InputFile"   , InputFile   )
    self.setProperty( "OutputLevel" , OutputLevel )


    # cpp core configuration only
    containers = [stream[0] for stream in RootStreamEDMList if self.EDMFormat in stream[1]]
    self.setProperty("Containers", containers )
    self.configure()

    

  def configure(self):
    f = ROOT.TFile( self.InputFile, "read")
    t = f.Get( "CollectionTree" )
    # check number of entries of each container
    entries = []
    for container in self.Containers:
      treename = container.replace(":","_").replace("#",'_')
      MSG_INFO(self, f"Checking {treename} tree into the root file...")

      t = f.Get("CollectionTree/" + treename)
      if not t:
        MSG_FATAL(self, f"container with name {treename} not found into the collection tree.")
      entries.append(t.GetEntries())
    if len(set(entries)) > 1:
      MSG_FATAL(self, "We have containers with different sizes into the collection. abort!")
    self.__entries = entries[0]
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





#
# format types
#


class RootStreamHITReader(RootStreamReader):
  def __init__(self, name, InputFile, **kw):
    RootStreamReader.__init__(self, name, 'HIT', InputFile, **kw)

class RootStreamESDReader(RootStreamReader):
  def __init__(self, name, InputFile, **kw):
    RootStreamReader.__init__(self, name, 'ESD', InputFile, **kw)

class RootStreamAODReader(RootStreamReader):
  def __init__(self, name, InputFile, **kw):
    RootStreamReader.__init__(self, name, 'AOD', InputFile, **kw)

