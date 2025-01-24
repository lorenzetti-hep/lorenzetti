__all__ = ["CaloRingsMaker"]

import ROOT

from typing import List
from GaugiKernel import Cpp
from GaugiKernel.macros import *

class CaloRingsMerge( Cpp ):
  
  def __init__( self, name,
                CollectionKeys   : List[str],
                OutputRingerKey  : str, 
                OutputLevel      : int=0
              ):

    Cpp.__init__(self, ROOT.CaloRingsMerge(name) )
 
    self.setProperty( "OutputRingerKey"    , OutputRingerKey  )
    self.setProperty( "CollectionKeys"     , CollectionKeys   )
    self.setProperty( "OutputLevel"        , OutputLevel      )