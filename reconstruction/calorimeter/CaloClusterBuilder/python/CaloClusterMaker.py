__all__ = ["CaloClusterMaker"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from CaloClusterBuilder import CaloClusterFlags as flags
import ROOT

class CaloClusterMaker( Cpp ):


  def __init__( self, name,
                InputCellsKey    : str, 
                InputSeedsKey    : str,
                OutputClusterKey : str, 
                EtaWindow        : float=flags.EtaWindow, 
                PhiWindow        : float=flags.PhiWindow,
                MinCenterEnergy  : float=flags.MinCenterEnergy,
                OutputLevel      : str=0, 
                HistogramPath    : str="Expert/Clusters",
              ):

    Cpp.__init__(self, ROOT.CaloClusterMaker(name) )

    self.setProperty( "InputCellsKey"        , InputCellsKey        ) 
    self.setProperty( "InputSeedsKey"        , InputSeedsKey        )
    self.setProperty( "OutputClusterKey"     , OutputClusterKey     ) 
    self.setProperty( "EtaWindow"            , EtaWindow            ) 
    self.setProperty( "PhiWindow"            , PhiWindow            )
    self.setProperty( "MinCenterEnergy"      , MinCenterEnergy      )
    self.setProperty( "OutputLevel"          , OutputLevel          ) 
    self.setProperty( "HistogramPath"        , HistogramPath        )


