__all__ = ["RootStreamESDReader"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
import ROOT

class RootStreamESDReader( Cpp ):

  def __init__( self, name,
                OutputEventKey   : str,
                OutputTruthKey   : str,
                OutputCellsKey   : str,
                OutputSeedsKey   : str,
                InputFile        : str,
                OutputLevel      : int=0, 
                NtupleName       : str="CollectionTree",
              ): 
    
    Cpp.__init__(self, ROOT.RootStreamESDReader(name))
    self.setProperty( "OutputEventKey"  , OutputEventKey   )
    self.setProperty( "OutputTruthKey"  , OutputTruthKey   )
    self.setProperty( "OutputCellsKey"  , OutputCellsKey   )
    self.setProperty( "OutputSeedsKey"  , OutputSeedsKey   )
    self.setProperty( "OutputLevel"     , OutputLevel      ) 
    self.setProperty( "NtupleName"      , NtupleName       )
    self.setProperty( "InputFile"       , InputFile        )

    f = ROOT.TFile( self.InputFile,"read")
    t = f.Get( self.NtupleName )
    self.__entries = t.GetEntries()


  def GetEntries(self):
    return self.__entries


  def merge(self, acc):
    acc.SetReader(self)
