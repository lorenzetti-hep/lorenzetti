__all__ = ["EventTape"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from ROOT import generator

class EventTape( Cpp ):

  def __init__( self, name           : str, 
                      OutputFile     : str,
                      RunNumber      : int=0,
                      NumberOfEvents : int=1,
                      OutputLevel    : int=0 ): 

    Cpp.__init__(self, generator.EventTape())   
    self.setProperty("RunNumber"      , RunNumber       )
    self.setProperty("NumberOfEvents" , NumberOfEvents  )
    self.setProperty("OutputFile"     , OutputFile      )
    self.setProperty("OutputLevel"    , OutputLevel     )


  def run( self, evt):

    if type(evt) is int:
      self.setProperty( "NumberOfEvents", evt )
    else:
      MSG_FATAL(self, "input argument type not supported. Should be int value")

    self._core.initialize()
    self._core.execute()
    self._core.finalize()


  def __add__(self, tool):
    self.push_back(tool)
    return self


  def push_back( self, tool ):
    self._core.push_back( tool.core() )


