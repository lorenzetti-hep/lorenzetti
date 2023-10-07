__all__ = ["PileupMerge"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class PileupMerge( Logger ):

  __allow_keys = [
                  "InputHitsKey",
                  "OutputHitsKey",
                  "InputEventKey",
                  "OutputEventKey",
                  "OutputLevel", 
                  "NtupleName",
                  "InputFile",
                  ]


  def __init__( self, name, **kw ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import PileupMerge
    self.__core = PileupMerge(name)

    for key, value in kw.items():
      self.setProperty( key,value )



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

