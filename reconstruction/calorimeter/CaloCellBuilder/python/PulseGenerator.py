
__all__ = ["PulseGenerator"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class PulseGenerator( Logger ):


  def __init__( self, name,
                OutputLevel     : int=0, 
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
                
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager, PulseGenerator
    self.__core = PulseGenerator(name)
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


  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if key in self.core().hasProperty(key):
      setattr( self, key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if hasattr(self, key):
      return getattr( self, key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)


     

