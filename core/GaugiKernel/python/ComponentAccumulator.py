
__all__ = ["ComponentAccumulator"]

from Gaugi import Logger


class ComponentAccumulator( Logger ):

  __allow_keys = []

  def __init__( self, name , output, **kw):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import Gaugi as GK # GaugiKernel
    self.__core = GK.ComponentAccumulator( name, output )
    self.__numberOfEvents = 10000
    for key, value in kw.items():
      if key in self.__allow_keys:
        setattr( self, '__' + key , value )
        self.__core.setProperty( key, value )
      else:
        MSG_FATAL( self, "Property with name %s is not allow for %s object", key , self.__class__.__name__)


  def run( self, evt=-1 ):
    self.__core.initialize()
    self.__core.run(evt)
    self.__core.finalize()


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      self.core().setProperty( key, value )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)


  def __add__( self, algs ):
    if type(algs) is not list:
      algs =[algs]
    for alg in algs:
      self.__core.push_back( alg.core() )
    return self

  
  def core(self):
    return self.__core


