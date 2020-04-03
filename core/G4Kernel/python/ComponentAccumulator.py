
__all__ = ["ComponentAccumulator"]

from Gaugi import Logger


class ComponentAccumulator( Logger ):

  def __init__( self, name ):

    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager
    self.__core = RunManager(name)



  @property
  def NumberOfThreads( self ):
    return self.__nThreads

  @NumberOfThreads.setter
  def NumberOfThreads( self, v ):
    self.__nThreads = v 
    self.__core.setProperty( "NumberOfThreads", v)


  @property
  def Output( self ):
    return self.__output

  @Output.setter
  def Output( self, v ):
    self.__output = v
    self.__core.setProperty( "Output", v )



  @property
  def VisMacro( self ):
    return self.__visMacro

  @VisMacro.setter
  def VisMacro( self, v ):
    self.__visMacro = v
    self.__core.setProperty( "VisMacro", v )


  def __add__( self, alg ):
    self.__core.push_back( alg )
    return self


  def run( self, macro="" ):
    self.__core.run(macro)










