__all__ = ["CaloCellMaker"]

from Gaugi import Logger
from Gaugi.messenger.macros import *

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
      if key in self.__allow_keys:
        setattr( self, '__' + key , value )
        self.__core.setProperty( key, value )
      else:
        MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object")


  def core(self):
    # Attach all tools before return the core
    for tool in self.Tools:
      self.__core.push_back(tool)
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      self.core().setProperty( key, value )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object")

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object")


  




