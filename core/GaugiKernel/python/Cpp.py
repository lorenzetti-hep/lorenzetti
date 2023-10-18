__all__ = ["CaloCellMerge", "treatPropertyValue"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from GaugiKernel import list2stdvector

import importlib



def treatPropertyValue( value ):

  if (type(value) is list) and (type(value[0]) is str):
    return list2stdvector('string', value)
  elif (type(value) is list) and (type(value[0]) is int):
    return list2stdvector('int', value)
  elif (type(value) is list) and (type(value[0]) is float):
    return list2stdvector('float', value)
  elif (type(value) is list) and (type(value[0]) is bool):
    return list2stdvector('bool', value)
  # list of list with ints, should be vector<vector<int>>
  elif (type(value) is list) and (type(value[0]) is list) and (type(value[0][0]) is int) :
    from ROOT.std import vector
    vec = vector("vector<int>")()
    for v in value:
      vec.push_back( list2stdvector('int', v) )
    return vec
  else:
    return value


class Cpp( Logger ):

  def __init__( self, name : str, import_path : str, OutputLevel : int=0 ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CaloCellMerge
    cpp = importlib.import_module(import_path)
    self.__core = cpp(name)
  

  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if self.__core.hasProperty(key):
      setattr( self, key , value )
      try:
        self.__core.setProperty( key, treatPropertyValue(value) )
      except:
        MSG_FATAL( self, f"Exception in property with name {key} and value: {value}")
    else:
      MSG_FATAL( self, f"Property with name {key} is not allow for this object")

 
  def getProperty( self, key ):
    if hasattr(self, key):
      return getattr( self, key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)










