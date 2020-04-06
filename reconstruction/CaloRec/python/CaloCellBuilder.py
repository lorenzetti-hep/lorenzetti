
__all__ = ["CaloCellBuilder"]

from Gaugi import Logger
from Gaugi.messenger.macros import *
import os

default_basepath = os.environ['LZT_PATH']+'/reconstruction/CaloRec/data/'

class CaloCellBuilder( Logger ):

  # ( CollectionKey, Detector Cell File, Pulse Shaper File )
  __detectors = [
      #"xAOD__CaloCellCollection_Barrel_PS",
      # Lar Barrel Calorimter
      ("xAOD__CaloCellCollection_Barrel_EM1" , 'detector_sampling_1.dat' , 'larcalorimeter_pulse_shape.dat' , 7),
      ("xAOD__CaloCellCollection_Barrel_EM2" , 'detector_sampling_2.dat' , 'larcalorimeter_pulse_shape.dat' , 7),
      ("xAOD__CaloCellCollection_Barrel_EM3" , 'detector_sampling_3.dat' , 'larcalorimeter_pulse_shape.dat' , 7),
      # Tile Barrel Calorimeter
      ("xAOD__CaloCellCollection_Barrel_HAD1", 'detector_sampling_4.dat' , 'tilecalorimeter_pulse_shape.dat' , 7),
      ("xAOD__CaloCellCollection_Barrel_HAD2", 'detector_sampling_5.dat' , 'tilecalorimeter_pulse_shape.dat' , 7),
      ("xAOD__CaloCellCollection_Barrel_HAD3", 'detector_sampling_6.dat' , 'tilecalorimeter_pulse_shape.dat' , 7),
      # Extended Barrel
      #"xAOD__CaloCellCollection_ExtBarrel_HAD1",
      #"xAOD__CaloCellCollection_ExtBarrel_HAD2",
      #"xAOD__CaloCellCollection_ExtBarrel_HAD3",
     ] 

            


  def __init__( self, name, basepath=default_basepath ):

    Logger.__init__(self)
    self.__recoAlgs = []
    self.__basepath = basepath
    self.configure()




  def configure(self):

    from CaloRec import CaloCellMaker, PulseGenerator, OptimalFilter
    for idx, config in enumerate( self.__detectors ):

      suffix = config[0].split('_')
      print(suffix)
      pulse = PulseGenerator("PulseGenerator", NSamples = config[3], ShaperFile = self.__basepath+config[2])
      of = OptimalFilter("OptimalFilter")

      alg = CaloCellMaker("CaloCellMaker", CollectionKey = config[0], CaloCellFile = self.__basepath+config[1], HistogramPath = '/CaloCellMaker')
      alg.Tools = [pulse, of]
      self.__recoAlgs.append( alg )
    

  def merge( self, acc ):
    for reco in self.__recoAlgs:
      acc+=reco 



















