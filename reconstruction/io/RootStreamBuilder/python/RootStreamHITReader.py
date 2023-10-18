__all__ = ["RootStreamHITReader"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
import ROOT


class RootStreamHITReader( Cpp ):

  def __init__( self, name,
                OutputEventKey   : str,
                OutputTruthKey   : str,
                OutputHitsKey    : str,
                OutputSeedsKey   : str,
                InputFile        : str,
                OutputLevel      : int=0, 
                NtupleName       : str="CollectionTree",
              ): 
    
    Cpp.__init__(self, ROOT.RootStreamHITReader(name))
    self.setProperty( "OutputEventKey"  , OutputEventKey  )
    self.setProperty( "OutputTruthKey"  , OutputTruthKey  )
    self.setProperty( "OutputHitsKey"   , OutputHitsKey   )
    self.setProperty( "OutputSeedsKey"  , OutputSeedsKey  )
    self.setProperty( "OutputLevel"     , OutputLevel     ) 
    self.setProperty( "NtupleName"      , NtupleName      )
    self.setProperty( "InputFile"       , InputFile       )

    f = ROOT.TFile( self.InputFile,"read")
    t = f.Get( self.NtupleName)
    self.__entries = t.GetEntries()


  def GetEntries(self):
    return self.__entries


  def merge(self, acc):
    acc.SetReader(self)
