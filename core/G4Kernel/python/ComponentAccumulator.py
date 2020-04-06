
__all__ = ["ComponentAccumulator"]

from Gaugi import Logger


class ComponentAccumulator( Logger ):

  __allow_keys = ["NumberOfThreads", "OutputFile", "VisMacro"]

  def __init__( self, name , **kw):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager
    self.__core = RunManager(name)

    for key, value in kw.items():
      if key in self.__allow_keys:
        setattr( self, '__' + key , value )
        self.__core.setProperty( key, value )
      else:
        MSG_ERROR( self, "Property with name %s is not allow for PulseGenerator object")


  def run( self, macro="" ):
    self.__core.run(macro)


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


  def __add__( self, algs ):
    print(type(algs))
    if type(algs) is not list:
      algs =[algs]
    print (algs )
    for alg in algs:
      self.__core.push_back( alg.core() )
    return self

  


