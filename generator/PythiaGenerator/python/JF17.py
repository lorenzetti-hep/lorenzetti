__all__ = ["JF17"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
from G4Kernel import treatPropertyValue
from PythiaGenerator.utilities import default_allow_keys

class JF17( Logger ):

  __allow_keys = default_allow_keys
  __allow_keys.extend([
                  "EtaWindow"      ,
                  "PhiWindow"      ,
                ])

  def __init__( self, name, **kw ): 
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import JF17
    # Create the algorithm
    self.__core = JF17()
    for key, value in kw.items():
      self.setProperty( key,value )
    self.core().initialize()


  def run(self):
    self.core().run()
    self.finalize()


  def core(self):
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      setattr( self, '__' + key , value )
      self.core().setProperty( key, treatPropertyValue(value) )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for JF17 object", key)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_ERROR( self, "Property with name %s is not allow for JF17 object", key)



