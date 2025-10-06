__all__ = ['ElectronBuilderCfg']

from GaugiKernel.macros import *
from EgammaBuilder import ElectronMaker



def ElectronBuilderCfg( name             : str, 
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

