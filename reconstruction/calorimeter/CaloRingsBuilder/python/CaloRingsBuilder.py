__all__ = [ 'CaloRingsBuilderCfg' ]


from CaloRingsBuilder import CaloRingsMaker, CaloRingsMerge
from CaloCell.CaloDefs import CaloSampling
import numpy as np

def CaloRingsBuilderCfg( name             : str, 
                         InputClusterKey  : str, 
                         OutputRingerKey  : str, 
                         OutputLevel      : int=0, 
                         HistogramPath    : str="Expert/Rings",
                         DoSigmaCut       : bool=False,
                         SigmaCut         : float=2.0
                         ):
    pi = np.pi

    rings   = CaloRingsMaker( name,
                              InputClusterKey    = InputClusterKey,  
                              OutputRingerKey    = OutputRingerKey+"_Aux",
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
                              EtaRange      = [0.0, 2.5],
                              OutputLevel   = OutputLevel,
                              DoSigmaCut    = DoSigmaCut,
                              SigmaCut      = SigmaCut
                            )
 

    fwd_rings   = CaloRingsMaker(name+"_Fwd",
                                 InputClusterKey    = InputClusterKey,  
                                 OutputRingerKey    = OutputRingerKey+"_Fwd_Aux",
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
                                 HistogramPath = HistogramPath+"_Fwd",
                                 EtaRange      = [2.5, 4.9],
                                 OutputLevel   = OutputLevel)
  
    merge_rings = CaloRingsMerge( "CaloRingsMerge",   
                                  CollectionKeys = [rings.OutputRingerKey, fwd_rings.OutputRingerKey],
                                  OutputRingerKey = OutputRingerKey,  
                                  OutputLevel   = OutputLevel)
                                 

    return [rings, fwd_rings, merge_rings]

