
__all__ = ["CaloCellBuilder"]

from GaugiKernel        import Logger
from GaugiKernel.macros import *
from GaugiKernel        import GeV
from G4Kernel     import treatPropertyValue, recordable
import os


#
# Calo cell builder
#
class CaloCellBuilder( Logger ):


  # basepath
  __basepath = os.environ['LZT_PATH']+'/geometry/DetectorATLASModel/data/'


  def __init__( self, name, 
                      HistogramPath  = "Expert", 
                      HitsKey = "Hits",
                      OutputLevel    = 1,
                      EstimationMethodECAL = 'OF',
                      EstimationMethodHAD = 'COF',
                      DoCrosstalk = True,
                      ):

    Logger.__init__(self)
    self.__recoAlgs = []
    self.__histpath = HistogramPath
    self.__outputLevel = OutputLevel
    self.__hitsKey = HitsKey
    self.__estimationMethodECAL = EstimationMethodECAL
    self.__estimationMethodHAD = EstimationMethodHAD
    self.__doCrosstalk = DoCrosstalk
    
    # configure
    self.configure()



  #
  # Configure 
  #
  def configure(self):

    MSG_INFO(self, "Configure CaloCellBuilder.")

    from CaloCellBuilder import CaloCellMaker, CaloCellMerge, PulseGenerator, OptimalFilter, CrossTalk, ConstrainedOptimalFilter

    collectionKeys = []
 
    # Get the layer cell configuration
    from DetectorATLASModel import create_ATLAS_layers 
    layers = create_ATLAS_layers(self.__basepath)
    

    for layer_id, layer in enumerate( layers ):

      for sampling in layer:

        for seg in sampling.segments:
          MSG_INFO(self, "Create new CaloCellMaker and dump all cells into %s collection", seg.CollectionKey)
          pulse = PulseGenerator( "PulseGenerator", 
                                  NSamples        = seg.NSamples, 
                                  ShaperFile      = seg.ShaperFile,
                                  OutputLevel     = self.__outputLevel,
                                  SamplingRate    = 25.0,
                                  Pedestal        = 0.0,
                                  DeformationMean = 0.0, 
                                  DeformationStd  = 0.0,
                                  NoiseMean       = 0.0,
                                  NoiseStd        = seg.EletronicNoise,
                                  StartSamplingBC = seg.StartSamplingBC, 
                                )

          cx = CrossTalk("CrossTalk",
                          MinEnergy       =1*GeV,
                          # input key
                          CollectionKey   = seg.CollectionKey, 
                          )
          method = []
          if seg.isEMLayer:
            if self.__estimationMethodECAL == 'OF':
              method = OptimalFilter("OptimalFilter",
                              Weights  = seg.OFWeights,
                              OutputLevel=self.__outputLevel)
            elif self.__estimationMethodECAL == 'COF':
              method = ConstrainedOptimalFilter("ConstrainedOptimalFilter",
                              OutputLevel=self.__outputLevel,
                              PulsePath = seg.ShaperFile,
                              Threshold = 3*(seg.EletronicNoise*seg.EletronicNoise),
                              NSamples = seg.NSamples,
                              StartSamplingBC = seg.StartSamplingBC,
                              SamplingRate    = 25.0)
          else:
              if self.__estimationMethodHAD == 'OF':
                method = OptimalFilter("OptimalFilter",
                                Weights  = seg.OFWeights,
                                OutputLevel=self.__outputLevel)
              elif self.__estimationMethodHAD == 'COF':
                method = ConstrainedOptimalFilter("ConstrainedOptimalFilter",
                                OutputLevel=self.__outputLevel,
                                PulsePath = seg.ShaperFile,
                                Threshold = 3*(seg.EletronicNoise*seg.EletronicNoise),
                                NSamples = seg.NSamples,
                                StartSamplingBC = seg.StartSamplingBC,
                                SamplingRate    = 25.0)
          
          alg = CaloCellMaker("CaloCellMaker_" +seg.CollectionKey, 
                              # input key
                              EventKey                = recordable( "EventInfo" ), 
                              HitsKey                 = recordable( self.__hitsKey ),
                              # output key
                              CollectionKey           = seg.CollectionKey, 
                              # Hits grid configuration
                              EtaBins                 = seg.EtaBins,
                              PhiBins                 = seg.PhiBins,
                              RMin                    = seg.RMin,
                              RMax                    = seg.RMax,
                              Sampling                = seg.Sampling,
                              Segment                 = seg.Segment,
                              Detector                = seg.Detector,
                              # Bunch crossing configuration
                              BunchIdStart            = seg.BunchIdStart,
                              BunchIdEnd              = seg.BunchIdEnd,
                              BunchDuration           = 25, #ns
                              # monitoring configuration
                              HistogramPath           = self.__histpath + '/' + seg.name,
                              OutputLevel             = self.__outputLevel,
                              DetailedHistograms      = False, # Use True when debug with only one thread
                              )
  
          alg.PulseGenerator = pulse # for all cell

          if self.__doCrosstalk:
            # print(seg.name)
            if seg.name == 'EMB2_0' or seg.name == 'EMEC2_0' or seg.name == 'EMEC2_1':
              # print(seg.name)
              MSG_INFO(self, "Applying CrossTalk effect into %s sampling.",seg.name)
              alg.Tools = [cx, method] # for each cell
            else:
              alg.Tools = [ method] # for each cell
          else:
            alg.Tools = [ method] # for each cell

          self.__recoAlgs.append( alg )
          collectionKeys.append( seg.CollectionKey )


    MSG_INFO(self, "Create CaloCellMerge and dump all cell collections into %s container", "Cells")
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















