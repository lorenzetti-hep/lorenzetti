

__all__ = ["CaloCellMaker", "CaloSamplingMaker"]


from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue



class CaloCellMaker( Logger ):

  __allow_keys = [
                    "EventKey"          ,
                    "HitsKey"           ,
                    "CollectionKey"     ,
                    "EtaBins"           ,
                    "PhiBins"           ,
                    "RMin"              ,
                    "RMax"              ,
                    "Sampling"          ,
                    "Segment"           ,
                    "Detector"          ,
                    "BunchIdStart"      ,
                    "BunchIdEnd"        ,
                    "BunchDuration"     ,
                    "OutputLevel"       ,
                    "DetailedHistograms",
                    "HistogramPath"     ,
                ]

  def __init__( self, name, **kw ): 
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import CaloCellMaker as core
    # Create the algorithm
    self.__core = core(name)
    self.Tools = []
    self.PulseGenerator = None
    for key, value in kw.items():
      self.setProperty( key, value )


  def core(self):
    # Attach all tools before return the core
    for tool in self.Tools:
      self.__core.push_back(tool.core())
    self.__core.setPulseGenerator(self.PulseGenerator.core())
    return self.__core


  def setProperty( self, key, value ):
    if key in self.__allow_keys:
      setattr( self, '__' + key , value )
      self.__core.setProperty( key, treatPropertyValue(value) )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

 
  def getProperty( self, key ):
    if key in self.__allow_keys:
      return getattr( self, '__' + key )
    else:
      MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)



  def __add__( self, tool ):
    self.Tools += tool
    return self
  
#
# Internal class 
#
class LateralSegmentation( Logger ):

  __allow_keys = [
                  "ShaperFile",      
                  "BunchIdStart",    
                  "BunchIdEnd",      
                  "StartSamplingBC",
                  "NSamples",        
                  "EletronicNoise",  
                  "OFWeights", 
                  "Detector",      
                  ]


  def __init__( self, name, collectionKey, f, **kw ): 
    Logger.__init__(self)
    self.name = name
    self.CaloCellFile = f
    self.CollectionKey = collectionKey
    for key, value in kw.items():

      if key in self.__allow_keys:
        setattr( self, key , value )
      else:
        MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)



class CaloSamplingMaker( Logger ):

  #
  # Constructor
  #
  def __init__( self, name, collectionKey, files, **kw ):
    Logger.__init__(self)
    self.__segments = [ LateralSegmentation( name+"_"+str(idx), collectionKey+"_"+str(idx), f , **kw ) for idx, f in enumerate(files)]

  def segments(self):
    return self.__segments

