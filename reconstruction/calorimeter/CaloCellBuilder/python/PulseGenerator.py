
__all__ = ["PulseGenerator"]

from GaugiKernel import Cpp, LoggingLevel
from GaugiKernel.macros import *
import ROOT

class PulseGenerator( Cpp ):


  def __init__( self, name      : str,
                OutputLevel     : int=LoggingLevel.toC('INFO'), 
                NSamples        : int=0, 
                ShaperFile      : str="",
                Pedestal        : float=0,
                DeformationMean : float=0,
                DeformationStd  : float=0,
                NoiseMean       : float=0,
                NoiseStd        : float=0,
                SamplingRate    : float=0,
                StartSamplingBC : float=0
              ):
                
    Cpp.__init__(self, ROOT.PulseGenerator(name) )
    self.setProperty( "OutputLevel"     , OutputLevel       )
    self.setProperty( "NSamples"        , NSamples          ) 
    self.setProperty( "ShaperFile"      , ShaperFile        )
    self.setProperty( "Pedestal"        , Pedestal          )
    self.setProperty( "DeformationMean" , DeformationMean   )
    self.setProperty( "DeformationStd"  , DeformationStd    )
    self.setProperty( "NoiseMean"       , NoiseMean         )
    self.setProperty( "NoiseStd"        , NoiseStd          )
    self.setProperty( "SamplingRate"    , SamplingRate      )
    self.setProperty( "StartSamplingBC" , StartSamplingBC   )


 
     

