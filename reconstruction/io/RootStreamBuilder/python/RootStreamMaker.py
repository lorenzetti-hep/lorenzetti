__all__ = ["RootStreamMaker", "recordable", "get_recordable_keys"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue
from pprint import pprint


recodable_keys = []

def recordable( key:str , container: str) -> str:
  name = (container+'_'+key)   
  if name in recodable_keys:
    raise RuntimeError(f"Key {name} repeated. please use another key.")
  recodable_keys.append(name)
  return name

def get_recordable_keys():
  return recordable_keys




class RootStreamMaker( Logger ):

  __allow_keys = [
                  "OutputLevel", 
                  "NtupleName",
                  "OnlyRoI",
                  "EtaWindow",
                  "PhiWindow",
                  ]


  def __init__( self, name, **kw ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RootStreamMaker
    self.__core = RootStreamMaker(name)

    for key, value in kw.items():
      self.setProperty( key,value )

    from RootStreamBuilder import recodable_keys
    pprint(recodable_keys)
    self.setProperty("Containers", recodable_keys)

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



