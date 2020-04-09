__all__ = ["CaloCellMaker"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
from RecCommon import treatPropertyValue


class CaloCellMaker( Logger ):

  __allow_keys = ["CollectionKey", "OutputLevel", "CaloCellFile", "HistogramPath"]

  def __init__( self, name, **kw ): 

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CaloCellMaker
    # Create the algorithm
    self.__core = CaloCellMaker(name)
    self.Tools = []

    for key, value in kw.items():
      self.setProperty( key, value )


  def core(self):
    # Attach all tools before return the core
    for tool in self.Tools:
      self.__core.push_back(tool.core())
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      setattr( self, '__' + key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object", key)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object", key)



  def __add__( self, tool ):
    self.Tools += tool
    return self
  




