__all__ = ["RootStreamESDMaker"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from RootStreamBuilder import RootStreamESDFlags as flags
import ROOT

class RootStreamESDMaker( Cpp ):

  def __init__( self, name,
                InputEventKey    : str,
                InputTruthKey    : str,
                InputCellsKey    : str,
                InputSeedsKey    : str,
                OutputEventKey   : str=None,
                OutputTruthKey   : str=None,
                OutputCellsKey   : str=None,
                OutputSeedsKey   : str=None,
                OutputLevel      : int=0, 
                NtupleName       : str="CollectionTree",
                EtaWindow        : float=flags.EtaWindow,
                PhiWindow        : float=flags.PhiWindow,
              ): 
    
    Cpp.__init__(self, ROOT.RootStreamESDMaker(name))
    self.setProperty( "InputEventKey"   , InputEventKey   )
    self.setProperty( "InputTruthKey"   , InputTruthKey   )
    self.setProperty( "InputCellsKey"   , InputCellsKey   )
    self.setProperty( "InputSeedsKey"   , InputSeedsKey   )
    self.setProperty( "OutputEventKey"  , OutputEventKey if OutputEventKey else InputEventKey )
    self.setProperty( "OutputTruthKey"  , OutputTruthKey if OutputTruthKey else InputTruthKey )
    self.setProperty( "OutputCellsKey"  , OutputCellsKey if OutputCellsKey else InputCellsKey )
    self.setProperty( "OutputSeedsKey"  , OutputSeedsKey if OutputSeedsKey else InputSeedsKey )
    self.setProperty( "OutputLevel"     , OutputLevel     ) 
    self.setProperty( "NtupleName"      , NtupleName      )
    self.setProperty( "EtaWindow"       , EtaWindow       )
    self.setProperty( "PhiWindow"       , PhiWindow       )

