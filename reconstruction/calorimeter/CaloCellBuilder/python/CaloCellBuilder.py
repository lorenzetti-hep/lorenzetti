
__all__ = ["CaloCellBuilder"]

from GaugiKernel        import Logger
from GaugiKernel.macros import *
from GaugiKernel        import GeV
from G4Kernel           import treatPropertyValue
from RootStreamBuilder  import recordable
import os


#
# Calo cell builder
#
class CaloCellBuilder( Logger ):


  def __init__( self, name, detector,
                      HistogramPath        = "Expert", 
                      InputHitsKey         = "Hits",
                      OutputCellsKey       = "Cells",
                      OutputTruthCellsKey  = "TruthCells",
                      DoCrosstalk          = False,
                      OutputLevel          = 1,
                      ):

    Logger.__init__(self)
    self.__recoAlgs = []
    self.HistogramPath    = HistogramPath
    self.OutputLevel      = OutputLevel
    self.InputHitsKey     = InputHitsKey
    self.OutputCellsKey   = OutputCellsKey
    self.OutputTruthCells = OutputTruthCellsKey
    self.__detector       = detector
    self.OutputCollectionKeys   = []

    


  #
  # Configure 
  #
  def configure(self):

    MSG_INFO(self, "Configure CaloCellBuilder.")

    from CaloCellBuilder import CaloCellMaker, CaloCellMerge, PulseGenerator, OptimalFilter, CrossTalk

 
  
    for samp in self.__detector.samplings:

      MSG_INFO(self, "Create new CaloCellMaker and dump all cells into %s collection", samp.CollectionKey)
      pulse = PulseGenerator( "PulseGenerator", 
                              NSamples        = samp.Samples, 
                              ShaperFile      = samp.Shaper,
                              OutputLevel     = self.OutputLevel,
                              SamplingRate    = 25.0,
                              Pedestal        = 0.0,
                              DeformationMean = 0.0, 
                              DeformationStd  = 0.0,
                              NoiseMean       = 0.0,
                              NoiseStd        = samp.Noise,
                              StartSamplingBC = samp.StartSamplingBC )
      print('2')

      of= OptimalFilter("OptimalFilter",
                        WeightsEnergy  = samp.OFWeightsEnergy,
                        WeightsTime    = samp.OFWeightsTime,
                        OutputLevel=self.OutputLevel)
      print('1')
      print(self.InputHitsKey)
      print(samp.CollectionKey)
      alg = CaloCellMaker("CaloCellMaker_" + samp.CollectionKey, 
                            # input key
                            InputHitsKey            =  self.InputHitsKey, # hits
                            # output key
                            OutputCollectionKey     = samp.CollectionKey, # descriptors
                            # Hits grid configuration
                            EtaBins                 = samp.sensitive().EtaBins,
                            PhiBins                 = samp.sensitive().PhiBins,
                            ZMin                    = samp.volume().ZMin,
                            ZMax                    = samp.volume().ZMax,
                            Sampling                = samp.Sampling,
                            Segment                 = samp.sensitive().Segment,
                            Detector                = samp.Detector,
                            # Bunch crossing configuration
                            BunchIdStart            = samp.BunchIdStart,
                            BunchIdEnd              = samp.BunchIdEnd,
                            BunchDuration           = 25, #ns
                            # monitoring configuration
                            HistogramPath           = self.HistogramPath + '/' + samp.name(),
                            OutputLevel             = self.OutputLevel,
                            DetailedHistograms      = False, # Use True when debug with only one thread
                            )
      print('done')


      alg.PulseGenerator = pulse # for all cell
      alg.Tools = [of] # for each cel
      self.__recoAlgs.append( alg )
      self.OutputCollectionKeys.append( samp.CollectionKey )



    MSG_INFO(self, "Create CaloCellMerge and dump all cell collections into %s container", "Cells")
    # Merge all collection into a container and split between truth and reco
    mergeAlg = CaloCellMerge( "CaloCellMerge" , 
                              # input key
                              InputCollectionKeys   = self.OutputCollectionKeys, # descriptors
                              # output key
                              OutputTruthCellsKey   = self.OutputTruthCellsKey , # cells
                              OutputCellsKey        = self.OutputCellsKey      , # cells
                              # configs
                              OutputLevel           = self.OutputLevel )

    self.__recoAlgs.append( mergeAlg )


  def merge( self, acc ):
    # configure
    self.configure()
    for reco in self.__recoAlgs:
      acc+=reco 















