__all__ = ["RootStreamAODReader"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
import ROOT

class RootStreamAODReader( Cpp ):

  def __init__( self, name,
                InputFile         : str,
                OutputEventKey    : str,
                OutputTruthKey    : str,
                OutputClusterKey  : str,
                OutputRingerKey   : str,
                OutputSeedsKey    : str,
                OutputElectronKey : str,
                OutputLevel       : int=0, 
                NtupleName        : str="CollectionTree",
              ): 
    
    Cpp.__init__(self, ROOT.RootStreamAODReader(name))
    self.setProperty( "OutputEventKey"   , OutputEventKey     )
    self.setProperty( "OutputTruthKey"   , OutputTruthKey     )
    self.setProperty( "OutputClusterKey" , OutputClusterKey   )
    self.setProperty( "OutputRingerKey"  , OutputRingerKey    )
    self.setProperty( "OutputSeedsKey"   , OutputSeedsKey     )
    self.setProperty( "OutputElectronKey", OutputElectronKey  )
    self.setProperty( "OutputLevel"      , OutputLevel        ) 
    self.setProperty( "NtupleName"       , NtupleName         )
    self.setProperty( "InputFile"        , InputFile          ) 
    
    f = ROOT.TFile( self.InputFile,"read")
    t = f.Get( self.NtupleName )
    self.__entries = t.GetEntries()


  def GetEntries(self):
    return self.__entries


  def merge(self, acc):
    acc.SetReader(self)
