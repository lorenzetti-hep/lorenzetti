
__all__ = ["ComponentAccumulator"]

from Gaugi import Logger


class ComponentAccumulator( Logger ):

  __allow_keys = ["NumberOfThreads", "OutputFile", "RunVis"]

  def __init__( self, name , detector, **kw):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzett')
    from ROOT import RunManager
    self.__core = RunManager(name)
    self.__core.setDetectorConstruction( detector.core() )
    self.__numberOfEvents = 10000
    for key, value in kw.items():
      if key in self.__allow_keys:
        setattr( self, '__' + key , value )
        self.__core.setProperty( key, value )
      else:
        MSG_FATAL( self, "Property with name %s is not allow for %s object", key , self.__class__.__name__)


  def run( self, evt=None ):
    if evt is None:
      evt = self.__numberOfEvents
    elif evt > self.__numberOfEvents:
      evt = self.__numberOfEvents
    self.__core.run(evt)


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


  def setNumberOfEvents( self, evt ):
    self.__numberOfEvents = evt

