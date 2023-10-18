__all__ = ["RootStreamHITMaker"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from RootStreamBuilder import RootStreamHITFlags as flags
import ROOT

class RootStreamHITMaker( Cpp ):

  def __init__( self, name,
                InputEventKey    : str,
                InputTruthKey    : str,
                InputHitsKey     : str,
                InputSeedsKey    : str,               
                OutputEventKey   : str=None,
                OutputTruthKey   : str=None,
                OutputHitsKey    : str=None,
                OutputSeedsKey   : str=None,
                OutputLevel      : int=0, 
                NtupleName       : str="CollectionTree",
                OnlyRoI          : bool=flags.OnlyRoI,
                EtaWindow        : float=flags.EtaWindow,
                PhiWindow        : float=flags.PhiWindow,
              ): 
    
    Cpp.__init__(self, ROOT.RootStreamHITMaker(name))
    
    self.setProperty( "InputEventKey"   , InputEventKey   )
    self.setProperty( "InputTruthKey"   , InputTruthKey   )
    self.setProperty( "InputHitsKey"    , InputHitsKey    )
    self.setProperty( "InputSeedsKey"   , InputSeedsKey   )

    self.setProperty( "OutputEventKey"  , OutputEventKey if OutputEventKey else InputEventKey  )
    self.setProperty( "OutputTruthKey"  , OutputTruthKey if OutputTruthKey else InputTruthKey  )
    self.setProperty( "OutputHitsKey"   , OutputHitsKey if OutputHitsKey else InputHitsKey     )
    self.setProperty( "OutputSeedsKey"  , OutputSeedsKey if OutputSeedsKey else InputSeedsKey       )

    self.setProperty( "OutputLevel"     , OutputLevel     ) 
    self.setProperty( "NtupleName"      , NtupleName      )
    self.setProperty( "OnlyRoI"         , OnlyRoI         )
    self.setProperty( "EtaWindow"       , EtaWindow       )
    self.setProperty( "PhiWindow"       , PhiWindow       )



