__all__ = ["ElectronMaker"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from typing import List
import ROOT
import numpy as np

class ElectronMaker( Cpp ):
    def __init__( self, name,
                  InputClusterKey  : str, 
                  OutputElectronKey: str,
                  OutputLevel      : str=0,
                  TightCuts        : List[float]=[],
                  MediumCuts       : List[float]=[],
                  LooseCuts        : List[float]=[],
                  VLooseCuts       : List[float]=[],
                  SecondLambdaCuts : List[float]=[], 
                  #[loose,medium,tight]
                  LateralMomCuts   : List[float]=[],
                  LongMomCuts      : List[float]=[],
                  FracMaxCuts      : List[float]=[],
                  SecondRCuts      : List[float]=[],
                  LambdaCenterCuts : List[float]=[],
    ):
        Cpp.__init__(self, ROOT.ElectronMaker(name) )
        self.setProperty( "InputClusterKey"    , InputClusterKey  )
        self.setProperty( "OutputElectronKey"  , OutputElectronKey)
        self.setProperty( "OutputLevel"        , OutputLevel      )
        self.setProperty( "TightCuts"          , TightCuts        )
        self.setProperty( "MediumCuts"         , MediumCuts       )
        self.setProperty( "LooseCuts"          , LooseCuts        )
        self.setProperty( "VLooseCuts"         , VLooseCuts       )
        self.setProperty( "SecondLambdaCuts"   , SecondLambdaCuts )
        self.setProperty( "LateralMomCuts"     , LateralMomCuts   )
        self.setProperty( "LongMomCuts"        , LongMomCuts      )
        self.setProperty( "FracMaxCuts"        , FracMaxCuts      )
        self.setProperty( "SecondRCuts"        , SecondRCuts      )
        self.setProperty( "LambdaCenterCuts"   , LambdaCenterCuts )
        
        
    

