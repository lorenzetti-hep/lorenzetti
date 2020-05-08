
__all__ = ["CaloCellBuilder"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
from G4Kernel import treatPropertyValue, recordable
import os






class CaloCellBuilder( Logger ):

  # ( CollectionKey, Detector Cell File, Pulse Shaper File )
  __configs = [
      # Lar Barrel Calorimter
      ("Collection_EM1" , 'detector_sampling_1.dat' , 'larcalorimeter_pulse_shape.dat'  , 7),
      ("Collection_EM2" , 'detector_sampling_2.dat' , 'larcalorimeter_pulse_shape.dat'  , 7),
      ("Collection_EM3" , 'detector_sampling_3.dat' , 'larcalorimeter_pulse_shape.dat'  , 7),
      # Tile Barrel Calorimeter
      ("Collection_HAD1", 'detector_sampling_4.dat' , 'tilecalorimeter_pulse_shape.dat' , 7),
      ("Collection_HAD2", 'detector_sampling_5.dat' , 'tilecalorimeter_pulse_shape.dat' , 7),
      ("Collection_HAD3", 'detector_sampling_6.dat' , 'tilecalorimeter_pulse_shape.dat' , 7),
     ] 

            

  __basepath = os.environ['LZT_PATH']+'/geometry/DetectorATLASModel/data/'


  def __init__( self, name, HistogramPath = "Expert", OutputLevel=1):

    Logger.__init__(self)
    self.__recoAlgs = []
    self.__histpath = HistogramPath
    self.__outputLevel = OutputLevel
    self.configure()




  def configure(self):


    from CaloRec import CaloCellMaker, CaloCellMerge, PulseGenerator, OptimalFilter
    for idx, config in enumerate( self.__configs ):
      pulse = PulseGenerator( "PulseGenerator", 
                              NSamples    = config[3], 
                              ShaperFile  = self.__basepath+config[2],
                              OutputLevel = self.__outputLevel)
      of = OptimalFilter("OptimalFilter", 
                          OutputLevel=self.__outputLevel)

      alg = CaloCellMaker("CaloCellMaker", 
                          CollectionKey           = recordable( config[0] ), 
                          CaloCellFile            = self.__basepath+config[1], 
                          BunchIdStart            = -8, # This shold be configurable for EM/HAD
                          BunchIdEnd              = 7, # This should be configurable for EM/HAD
                          BunchDuration           = 25,
                          NumberOfSamplesPerBunch = 1,
                          HistogramPath           = self.__histpath,
                          OutputLevel             = self.__outputLevel)
      alg.Tools = [pulse, of]
      self.__recoAlgs.append( alg )
  

    collectionKeys =  [ recordable(config[0]) for config in self.__configs ]
    
    # Merge all collection into a container and split between truth and reco
    mergeAlg = CaloCellMerge( "CaloCellMerge" , 
                              CollectionKeys  = collectionKeys,
                              CellsKey        = recordable("Cells"),
                              TruthCellsKey   = recordable("TruthCells"),
                              OutputLevel     = self.__outputLevel )

    self.__recoAlgs.append( mergeAlg )



  def merge( self, acc ):
    for reco in self.__recoAlgs:
      acc+=reco 















