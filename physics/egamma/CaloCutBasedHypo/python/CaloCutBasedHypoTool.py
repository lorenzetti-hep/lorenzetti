__all__ = ["CaloCutBasedHypoTool","CaloCutBasedHypoCfg"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from CaloCell.CaloDefs import CaloSampling
import ROOT
import numpy as np

class CaloCutBasedHypoTool( Cpp ):
    def __init__( self, name,
                  InputClusterKey  : str, 
                  OutputElectronKey: str,
                  OutputLevel      : str=0,
                  TightCuts        : list=[],
                  MediumCuts       : list=[],
                  LooseCuts        : list=[],
                  VLooseCuts       : list=[],
    ):
        Cpp.__init__(self, ROOT.CaloCutBasedHypoTool(name) )
        self.setProperty( "InputClusterKey"    , InputClusterKey  )
        self.setProperty( "OutputElectronKey"  , OutputElectronKey)
        self.setProperty( "OutputLevel"        , OutputLevel      )
        self.setProperty( "TightCuts"          , TightCuts        )
        self.setProperty( "MediumCuts"         , MediumCuts       )
        self.setProperty( "LooseCuts"          , LooseCuts        )
        self.setProperty( "VLooseCuts"         , VLooseCuts       )
    


def CaloCutBasedHypoCfg( name             : str, 
                         InputClusterKey  : str,
                         OutputElectronKey: str,
                         OutputLevel      : int=0, 
                       ):

  from .CaloCutMaps import CutBasedIsEM
  hypo   = CaloCutBasedHypoTool(   "CaloCutBasedHypoTool",
                                    InputClusterKey    = InputClusterKey, 
                                    OutputElectronKey  = OutputElectronKey,
                                    TightCuts          = CutBasedIsEM().getCuts('tight'),
                                    MediumCuts         = CutBasedIsEM().getCuts('medium'),
                                    LooseCuts          = CutBasedIsEM().getCuts('loose'),
                                    VLooseCuts         = CutBasedIsEM().getCuts('vloose'),
                                    OutputLevel        = OutputLevel)
  return hypo

