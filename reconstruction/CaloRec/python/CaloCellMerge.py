__all__ = ["CaloCellMerge"]

from Gaugi import Logger, list_to_stdvector
from Gaugi.messenger.macros import *

class CaloCellMerge( Logger ):

  __allow_keys = [
                  "CollectionKeys", 
                  "CellsKey", 
                  "TruthCellsKey", 
                  "OutputLevel", 
                  ]

  def __init__( self, name, **kw ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CaloCellMerge
    # Create the algorithm
    self.__core = CaloCellMerge(name)

    for key, value in kw.items():
      if key in self.__allow_keys:
        setattr( self, '__' + key , value )
        self.__core.setProperty( key,self.__treatValue( value ) )
      else:
        MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object")


  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      self.core().setProperty( key, self.__treatValue(value) )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object")

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object")



  def __treatValue( self, value ):

    if (type(value) is list) and (type(value[0]) is str):
      return list_to_stdvector('string', value)
    elif (type(value) is list) and (type(value[0]) is int):
      return list_to_stdvector('int', value)
    elif (type(value) is list) and (type(value[0]) is float):
      return list_to_stdvector('float', value)
    elif (type(value) is list) and (type(value[0]) is bool):
      return list_to_stdvector('bool', value)
    else:
      return value












