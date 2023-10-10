__all__ = ["ConstrainedOptimalFilter"]


from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class ConstrainedOptimalFilter(Logger):

  
  def __init__( self, name,
                OutputLevel       : int=0, 
                NSamples          : float=0, 
                PulsePath         : str="",
                Threshold         : float=0,
                SamplingRate      : float=0,
                StartSamplingBC   : float=0,
              ):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager, ConstrainedOptimalFilter
    self.__core = ConstrainedOptimalFilter(name)
    self.setProperty( "OutputLevel"     , OutputLevel       ) 
    self.setProperty( "NSamples"        , NSamples          ) 
    self.setProperty( "PulsePath"       , PulsePath         )
    self.setProperty( "Threshold"       , Threshold         )
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





