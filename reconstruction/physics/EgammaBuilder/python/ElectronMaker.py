__all__ = ["ElectronMaker","ElectronMakerCfg"]

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
        
        
    


def ElectronMakerCfg( name             : str, 
                      InputClusterKey  : str,
                      OutputElectronKey: str,
                      OutputLevel      : int=0, 
                    ):

  from .CaloCutMaps import CutBasedIsEM
  hypo   = ElectronMaker( name,
                          InputClusterKey    = InputClusterKey, 
                          OutputElectronKey  = OutputElectronKey,
                          OutputLevel        = OutputLevel,
                          # central cuts
                          TightCuts          = CutBasedIsEM().getCuts('tight'),
                          MediumCuts         = CutBasedIsEM().getCuts('medium'),
                          LooseCuts          = CutBasedIsEM().getCuts('loose'),
                          VLooseCuts         = CutBasedIsEM().getCuts('vloose'),
                          # forward cuts
                          SecondLambdaCuts     = [4500,4500,2800],
                          LateralMomCuts       = [0.69,0.64,0.64],
                          LongMomCuts          = [0.55,0.29,0.24],
                          FracMaxCuts          = [0.22,0.23,0.39],
                          SecondRCuts          = [3900,3300,3000],
                          LambdaCenterCuts     = [255,255,250])
  return hypo

