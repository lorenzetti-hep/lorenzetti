__all__ = ["RootStreamESDMaker"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class RootStreamESDMaker( Logger ):

  __allow_keys = [
                  "InputEventKey",
                  "InputTruthKey",
                  "InputCellsKey",
                  "OutputEventKey",
                  "OutputTruthKey",
                  "OutputCellsKey",                 
                  "OutputLevel", 
                  "NtupleName",
                  "EtaWindow",
                  "PhiWindow",
                  ]


  def __init__( self, name, **kw ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RootStreamESDMaker
    self.__core = RootStreamESDMaker(name)

    for key, value in kw.items():
      self.setProperty( key,value )


  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      setattr( self, '__' + key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)



