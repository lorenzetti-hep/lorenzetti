
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

    from CaloCellBuilder import CaloCellMaker, CaloCellMerge, CrossTalkMaker, PulseGenerator, OptimalFilter

 
  
    for samp in self.__detector.samplings:

      DoCrosstalk = True if CaloFlags.DoCrosstalk and (samp.Sampling == CaloSampling.EMEC2 or samp.Sampling == CaloSampling.EM2) else False



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
     

      of= OptimalFilter("OptimalFilter",
                        WeightsEnergy  = samp.OFWeightsEnergy,
                        WeightsTime    = samp.OFWeightsTime,
                        OutputLevel=self.OutputLevel)
    
      alg = CaloCellMaker("CaloCellMaker_" + samp.CollectionKey, samp,
                            # input key
                            InputHitsKey            =  self.InputHitsKey, # hits
                            # output key
                            OutputCollectionKey     = samp.CollectionKey + "_Aux" if DoCrosstalk else samp.CollectionKey, # descriptors
                            # monitoring configuration
                            HistogramPath           = self.HistogramPath + '/' + samp.name(),
                            OutputLevel             = self.OutputLevel,
                            DetailedHistograms      = False, # Use True when debug with only one thread
                            )
  
      alg.PulseGenerator = pulse # for all cell
      alg.Tools = [of] # for each cel
      self.__recoAlgs.append( alg )


      if DoCrosstalk:
          cx = CrossTalkMaker( "CrossTalkMaker_" + samp.CollectionKey,
                                InputCellsKey    = samp.CollectionKey + "_Aux",
                                OutputCellsKey   = samp.CollectionKey,
                                MinEnergy        = CaloFlags.XTMinEnergy,
                                XTAmpCapacitive  = CaloFlags.XTAmpCapacitive,
                                XTAmpInductive   = CaloFlags.XTAmpInductive,
                                XTAmpResistive   = CaloFlags.XTAmpResistive,
                                HistogramPath    = self.HistogramPath + '/CrossTalk'
                                OutputLevel      = self.OutputLevel
                             )
          cx.Tools = [of]
          self.__recoAlgs.append( cx )



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

   
      
      self.__recoAlgs.append( cx )



  def merge( self, acc ):
    # configure
    self.configure()
    for reco in self.__recoAlgs:
      acc+=reco 















