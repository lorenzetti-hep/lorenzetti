__all__ = ["EventTape"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class EventTape( Logger ):

  __allow_keys = [
                        "RunNumber"         ,
                        "NumberOfEvents"    ,
                        "OutputFile"        , 
                        "OutputLevel"       ,
                      ]


  def __init__( self, name, **kw ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager, generator
    # Create the algorithm
    self.__core = generator.EventTape()
    for key, value in kw.items():
      self.setProperty( key,value )


  def core(self):
    return self.__core


  def run( self, evt):

    if type(evt) is int:
      self.setProperty( "NumberOfEvents", evt )
    else:
      MSG_FATAL(self, "input argument type not supported. Should be int value")

    self.core().initialize()
    self.core().execute()
    self.core().finalize()


  def __add__(self, tool):
    self.push_back(tool)
    return self


  def push_back( self, tool ):
    self.__core.push_back( tool.core() )


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

