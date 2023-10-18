__all__ = ["RootStreamAODMaker"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from RootStreamBuilder import RootStreamAODFlags as flags
import ROOT

class RootStreamAODMaker( Cpp ):

  def __init__( self, name,
                InputEventKey    : str,
                InputTruthKey    : str,
                InputCellsKey    : str,
                InputClusterKey  : str,
                InputRingerKey   : str,
                InputSeedsKey    : str,
                OutputEventKey   : str=None,
                OutputTruthKey   : str=None,
                OutputCellsKey   : str=None,
                OutputClusterKey : str=None,
                OutputRingerKey  : str=None,
                OutputSeedsKey   : str=None,
                OutputLevel      : int=0, 
                NtupleName       : str="CollectionTree",
                DumpCells        : bool=flags.DumpCells,
              ): 
    
    Cpp.__init__(self, ROOT.RootStreamAODMaker(name))
    self.setProperty( "InputEventKey"   , InputEventKey   )
    self.setProperty( "InputTruthKey"   , InputTruthKey   )
    self.setProperty( "InputCellsKey"   , InputCellsKey   )
    self.setProperty( "InputClusterKey" , InputClusterKey )
    self.setProperty( "InputRingerKey"  , InputRingerKey  )
    self.setProperty( "InputSeedsKey"   , InputSeedsKey   )

    self.setProperty( "OutputEventKey"  , OutputEventKey if OutputEventKey else InputEventKey       )
    self.setProperty( "OutputTruthKey"  , OutputTruthKey if OutputTruthKey else InputTruthKey       )
    self.setProperty( "OutputCellsKey"  , OutputCellsKey if OutputCellsKey else InputCellsKey       )
    self.setProperty( "OutputClusterKey", OutputClusterKey if OutputClusterKey else InputClusterKey )
    self.setProperty( "OutputRingerKey" , OutputRingerKey if OutputRingerKey else InputRingerKey    )
    self.setProperty( "OutputSeedsKey"  , OutputSeedsKey if OutputSeedsKey else InputSeedsKey       )
    
    self.setProperty( "OutputLevel"     , OutputLevel     ) 
    self.setProperty( "NtupleName"      , NtupleName      )
    self.setProperty( "DumpCells"       , DumpCells       )