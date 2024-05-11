__all__ = ["CaloRingsMaker", "CaloRingsMakerCfg", "CaloFwdRingsMakerCfg"]


from GaugiKernel import Cpp
from GaugiKernel.macros import *
from CaloCell.CaloDefs import CaloSampling
import ROOT
import numpy as np
pi = np.pi


class CaloRingsMaker( Cpp ):
  
  def __init__( self, name,
                InputClusterKey  : str, 
                OutputRingerKey  : str, 
                DeltaEtaRings    : list,
                DeltaPhiRings    : list,
                NRings           : list,
                LayerRings       : list,
                OutputLevel      : int=0, 
                HistogramPath    : str="Expert/Rings"
              ):

    Cpp.__init__(self, ROOT.CaloRingsMaker(name) )
 
    self.setProperty( "OutputRingerKey"    , OutputRingerKey  )
    self.setProperty( "InputClusterKey"    , InputClusterKey  )
    self.setProperty( "DeltaEtaRings"      , DeltaEtaRings    )
    self.setProperty( "DeltaPhiRings"      , DeltaPhiRings    )
    self.setProperty( "NRings"             , NRings           )
    self.setProperty( "LayerRings"         , LayerRings       )
    self.setProperty( "HistogramPath"      , HistogramPath    )
    self.setProperty( "OutputLevel"        , OutputLevel      )



def CaloRingsMakerCfg( name             : str, 
                       InputClusterKey  : str, 
                       OutputRingerKey  : str, 
                       OutputLevel      : int=0, 
                       HistogramPath    : str="Expert/Rings"
                       ):

  rings   = CaloRingsMaker(   "CaloRingsMaker",
                              InputClusterKey    = InputClusterKey,  
                              OutputRingerKey    = OutputRingerKey,
                              DeltaEtaRings      = [0.025,0.00325, 0.025, 0.050, 0.1, 0.1, 0.2 ],
                              DeltaPhiRings      = [pi/32, pi/32, pi/128, pi/128, pi/128, pi/32, pi/32, pi/32],
                              NRings             = [8, 64, 8, 8, 4, 4, 4],
                              LayerRings         = [
                                [CaloSampling.PSB, CaloSampling.PSE],
                                [CaloSampling.EMB1, CaloSampling.EMEC1],
                                [CaloSampling.EMB2, CaloSampling.EMEC2],
                                [CaloSampling.EMB3, CaloSampling.EMEC3],
                                [CaloSampling.HEC1, CaloSampling.TileCal1, CaloSampling.TileExt1],
                                [CaloSampling.HEC2, CaloSampling.TileCal2, CaloSampling.TileExt2],
                                [CaloSampling.HEC3, CaloSampling.TileCal3, CaloSampling.TileExt3],
                              ],
                              HistogramPath = HistogramPath,
                              OutputLevel   = OutputLevel)
  return rings

def CaloFwdRingsMakerCfg( name             : str, 
                          InputClusterKey  : str, 
                          OutputRingerKey  : str, 
                          OutputLevel       : int=0, 
                          HistogramPath    : str="Expert/Rings"
                          ):

  rings   = CaloRingsMaker(   "CaloRingsMaker",
                              InputClusterKey    = InputClusterKey,  
                              OutputRingerKey    = OutputRingerKey,
                              DeltaEtaRings = [0.1, 0.1, 0.1, 0.2, 0.2, 0.2],
                              DeltaPhiRings = [pi/32, pi/32, pi/32, pi/16, pi/16, pi/16],
                              NRings        = [4, 4, 4, 2, 2, 2],
                              LayerRings = [
                                [CaloSampling.EMEC1],
                                [CaloSampling.EMEC2],
                                [CaloSampling.EMEC3],
                                [CaloSampling.HEC1],
                                [CaloSampling.HEC2],
                                [CaloSampling.HEC3],
                              ],
                              HistogramPath = HistogramPath,
                              OutputLevel   = OutputLevel)
  return rings




