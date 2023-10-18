__all__ = ["OptimalFilter", "ConstrainedOptimalFilter"]


from GaugiKernel import Cpp
from GaugiKernel.macros import *


class OptimalFilter( Cpp ):
  
  def __init__( self, name,
                WeightsEnergy : list=[],
                WeightsTime   : list=[],
                OutputLevel   : int=0 
              ):

    Cpp.__init__(self, name, "ROOT.OptimalFilter", OutputLevel=OutputLevel)
    self.setProperty( "WeightsEnergy" , WeightsEnergy )
    self.setProperty( "WeightsTime"   , WeightsTime   )

 

class ConstrainedOptimalFilter(Cpp):

  def __init__( self, name,
                OutputLevel       : int=0, 
                NSamples          : float=0, 
                PulsePath         : str="",
                Threshold         : float=0,
                SamplingRate      : float=0,
                StartSamplingBC   : float=0,
              ):

    Cpp.__init__(self, name, "ROOT.ConstrainedOptimalFilter", OutputLevel=OutputLevel)
    self.setProperty( "OutputLevel"     , OutputLevel       ) 
    self.setProperty( "NSamples"        , NSamples          ) 
    self.setProperty( "PulsePath"       , PulsePath         )
    self.setProperty( "Threshold"       , Threshold         )
    self.setProperty( "SamplingRate"    , SamplingRate      )
    self.setProperty( "StartSamplingBC" , StartSamplingBC   )

