
__all__ = ["create_ATLAS_layers"]





#
# ATLAS detector cell configuration
#
def create_ATLAS_layers():

  from CaloRecBuilder import CaloSample

  psb = CaloSample( "PSB", "Collection_PSB", 
                    # segmentation
                    [ "detector_sample_0_seg_0.dat" ], 
                    # layer configuration
                    ShaperFile      = "pulseLar.dat",
                    BunchIdStart    = -21, # -525ns
                    BunchIdEnd      = 3, # +75ns
                    StartSamplingBC = -2, 
                    NSamples        = 5,
                    EletronicNoise  = 26, # MeV
                    OFWeights       = [ -0.0720, 0.2191, 0.6351, 0.3738, 0.0762],
                    )
  
  emb1 =CaloSample( "EMB1", "Collection_EMB1", 
                    # segmentation
                    [ "detector_sample_1_seg_0.dat" ], 
                    # layer configuration
                    ShaperFile      = "pulseLar.dat",
                    BunchIdStart    = -21, # -525ns
                    BunchIdEnd      = 3, # +75ns
                    StartSamplingBC = -2, 
                    NSamples        = 5,
                    EletronicNoise  = 26, # MeV
                    OFWeights       = [ -0.0720, 0.2191, 0.6351, 0.3738, 0.0762],
                    )
  
  emb2 =CaloSample( "EMB2", "Collection_EMB2", 
                    # segmentation
                    [ "detector_sample_2_seg_0.dat" ], 
                    # layer configuration
                    ShaperFile      = "pulseLar.dat",
                    BunchIdStart    = -21, # -525ns
                    BunchIdEnd      = 3, # +75ns
                    StartSamplingBC = -2, 
                    NSamples        = 5,
                    EletronicNoise  = 60, # MeV
                    OFWeights       = [ -0.1108, 0.2146, 0.6378, 0.3856, 0.0360],
                    )
  
  emb3 =CaloSample( "EMB3", "Collection_EMB3", 
                    # segmentation
                    [ "detector_sample_3_seg_0.dat" ], 
                    ShaperFile      = "pulseLar.dat",
                    BunchIdStart    = -21, # -525ns
                    BunchIdEnd      = 3, # +75ns
                    StartSamplingBC = -2, 
                    NSamples        = 5,
                    EletronicNoise  = 40, # MeV
                    OFWeights       = [ -0.0125, 0.2379, 0.6097, 0.3863, 0.1183],
                    )
  
  tilecal1 = CaloSample( "TileCal1", "Collection_TileCal1", 
                    # segmentation
                    [ "detector_sample_4_seg_0.dat" ], 
                    ShaperFile      = "pulseTile.dat",
                    BunchIdStart    = -6, # -150ns
                    BunchIdEnd      = 4, # +100ns
                    StartSamplingBC = -3, 
                    NSamples        = 7,
                    EletronicNoise  = 40, # MeV
                    OFWeights       = [ -0.3683, -0.3389, 0.1549, 0.8386, 0.2539, -0.2149, -0.3253],
                    )
  
  tilecal2 = CaloSample( "TileCal2", "Collection_TileCal2", 
                    [ "detector_sample_5_seg_0.dat" ], 
                    ShaperFile      = "pulseTile.dat",
                    BunchIdStart    = -6, # -150ns
                    BunchIdEnd      = 4, # +100ns
                    StartSamplingBC = -3, 
                    NSamples        = 7,
                    EletronicNoise  = 40, # MeV
                    OFWeights       = [ -0.3808, -0.3594, 0.1807, 0.8108, 0.2796, -0.1997, -0.3311],
                    )
  
  tilecal3 = CaloSample( "TileCal3", "Collection_TileCal3", 
                    [ "detector_sample_6_seg_0.dat" ], 
                    ShaperFile      = "pulseTile.dat",
                    BunchIdStart    = -6, # -150ns
                    BunchIdEnd      = 4, # +100ns
                    StartSamplingBC = -3, 
                    NSamples        = 7,
                    EletronicNoise  = 40, # MeV
                    OFWeights       = [ -0.3892, -0.3554, 0.1847, 0.8053, 0.2893, -0.2156,-0.3191] ,
                    )
  
  tilecalExt1 = CaloSample( "TileCalExt1", "Collection_TileCalExt1", 
                    [ "detector_sample_7_seg_0.dat" ], 
                    ShaperFile      = "pulseTile.dat",
                    BunchIdStart    = -6, # -150ns
                    BunchIdEnd      = 4, # +100ns
                    StartSamplingBC = -3, 
                    NSamples        = 7,
                    EletronicNoise  = 40, # MeV
                    OFWeights       = [ -0.3892, -0.3554, 0.1847, 0.8053, 0.2893, -0.2156,-0.3191] ,
                    )
  
  tilecalExt2 = CaloSample( "TileCalExt2", "Collection_TileCalExt2", 
                    [ "detector_sample_8_seg_0.dat" ], 
                    ShaperFile      = "pulseTile.dat",
                    BunchIdStart    = -6, # -150ns
                    BunchIdEnd      = 4, # +100ns
                    StartSamplingBC = -3, 
                    NSamples        = 7,
                    EletronicNoise  = 40, # MeV
                    OFWeights       = [ -0.3892, -0.3554, 0.1847, 0.8053, 0.2893, -0.2156,-0.3191] ,
                    )
  
  tilecalExt3 = CaloSample( "TileCalExt3", "Collection_TileCalExt3", 
                    [ "detector_sample_9_seg_0.dat" ], 
                    ShaperFile      = "pulseTile.dat",
                    BunchIdStart    = -6, # -150ns
                    BunchIdEnd      = 4, # +100ns
                    StartSamplingBC = -3, 
                    NSamples        = 7,
                    EletronicNoise  = 40, # MeV
                    OFWeights       = [ -0.3892, -0.3554, 0.1847, 0.8053, 0.2893, -0.2156,-0.3191] ,
                    )
  
  emec1 = CaloSample( "EMEC1", "Collection_EMEC1", 
                    [ 
                      "detector_sample_10_seg_0.dat" ,
                      "detector_sample_10_seg_1.dat" ,
                      "detector_sample_10_seg_2.dat" ,
                      "detector_sample_10_seg_3.dat" ,
                      ], 
                    ShaperFile      = "pulseLar.dat",
                    BunchIdStart    = -21, # -525ns
                    BunchIdEnd      = 3, # +75ns
                    StartSamplingBC = -2, 
                    NSamples        = 5,
                    EletronicNoise  = 26, # MeV
                    OFWeights       = [ -0.0720, 0.2191, 0.6351, 0.3738, 0.0762],
                    )
  
  emec2 = CaloSample( "EMEC2", "Collection_EMEC2", 
                    [ 
                      "detector_sample_11_seg_0.dat" ,
                      "detector_sample_11_seg_1.dat" ,
                      ], 
                    ShaperFile      = "pulseLar.dat",
                    BunchIdStart    = -21, # -525ns
                    BunchIdEnd      = 3, # +75ns
                    StartSamplingBC = -2, 
                    NSamples        = 5,
                    EletronicNoise  = 60, # MeV
                    OFWeights       = [ -0.0720, 0.2191, 0.6351, 0.3738, 0.0762],
                    )
  
  emec3 = CaloSample( "EMEC3", "Collection_EMEC3", 
                    [ 
                      "detector_sample_12_seg_0.dat",
                      "detector_sample_12_seg_1.dat",
                      ], 
                    ShaperFile      = "pulseLar.dat",
                    BunchIdStart    = -21, # -525ns
                    BunchIdEnd      = 3, # +75ns
                    StartSamplingBC = -2, 
                    NSamples        = 5,
                    EletronicNoise  = 40, # MeV
                    OFWeights       = [ -0.0720, 0.2191, 0.6351, 0.3738, 0.0762],
                    )
  
  hec1 = CaloSample( "HEC1", "Collection_HEC1", 
                    [ 
                      "detector_sample_13_seg_0.dat",
                      "detector_sample_13_seg_1.dat",
                      ], 
                    ShaperFile      = "pulseTile.dat",
                    BunchIdStart    = -6, # -150ns
                    BunchIdEnd      = 4, # +100ns
                    StartSamplingBC = -3, 
                    NSamples        = 7,
                    EletronicNoise  = 250, # MeV
                    OFWeights       = [ -0.3892, -0.3554, 0.1847, 0.8053, 0.2893, -0.2156,-0.3191] ,
                    )
  
  hec2 = CaloSample( "HEC2", "Collection_HEC2", 
                    [ 
                      "detector_sample_14_seg_0.dat",
                      "detector_sample_14_seg_1.dat",
                      ], 
                    ShaperFile      = "pulseTile.dat",
                    BunchIdStart    = -6, # -150ns
                    BunchIdEnd      = 4, # +100ns
                    StartSamplingBC = -3, 
                    NSamples        = 7,
                    EletronicNoise  = 400, # MeV
                    OFWeights       = [ -0.3892, -0.3554, 0.1847, 0.8053, 0.2893, -0.2156,-0.3191] ,
                    )
  
  hec3 = CaloSample( "HEC3", "Collection_HEC3", 
                    [ 
                      "detector_sample_15_seg_0.dat",
                      "detector_sample_15_seg_1.dat",
                      ], 
                    ShaperFile      = "pulseTile.dat",
                    BunchIdStart    = -6, # -150ns
                    BunchIdEnd      = 4, # +100ns
                    StartSamplingBC = -3, 
                    NSamples        = 7,
                    EletronicNoise  = 750, # MeV
                    OFWeights       = [ -0.3892, -0.3554, 0.1847, 0.8053, 0.2893, -0.2156,-0.3191] ,
                    )


  layers = [    
      [ psb ], # PS=0, ECal=0
      [ emb1, emec1 ], # EM1=1, ECal=0
      [ emb2, emec2 ], # EM2=2, ECal=0
      [ emb3, emec3 ], # EM3=3, ECal=0
      [ tilecal1, tilecalExt1, hec1 ], # HAD1=4, HCal=1
      [ tilecal2, tilecalExt2, hec2 ], # HAD2=5, HCal=1
      [ tilecal3, tilecalExt3, hec3 ], # HAD3=6, HCal=1
     ] 


  return layers

