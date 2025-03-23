__all__ = ["EventTape"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from ROOT import generator
from typing import Optional, Union, List

class EventTape( Cpp ):

  def __init__( self, name           : str, 
                      OutputFile     : str,
                      RunNumber      : int=0,
                      NumberOfEvents : int=1,
                      OutputLevel    : int=0 ): 

    Cpp.__init__(self, generator.EventTape())   
    self.setProperty("RunNumber"      , RunNumber       )
    self.setProperty("OutputFile"     , OutputFile      )
    self.setProperty("OutputLevel"    , OutputLevel     )


  def run( self, evt : Optional[Union[int,List[int]]] = None ):
    
    if type(evt) is int:
      evts = list(range(evt))
    elif type(evt) is list:
      evts = [int(e) for e in evt]
    else:
      MSG_FATAL(self, "input argument type not supported. Should be int value or a list of ints")

    if len(evts)==0:
      MSG_FATAL(self, "No events to process")
  
    self.setProperty( "EventNumbers" , evt)
    self._core.initialize()
    self._core.execute()
    self._core.finalize()


  def __add__(self, tool):
    self.push_back(tool)
    return self

  def push_back( self, tool ):
    self._core.push_back( tool.core() )

  

