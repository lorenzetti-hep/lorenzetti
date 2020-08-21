#ifndef enumeration_h
#define enumeration_h


namespace CaloSampling{

  enum CaloSample{

    PSB       = 0,
    EMB1      = 1,
    EMB2      = 2,
    EMB3      = 3,
    TileCal1  = 4,
    TileCal2  = 5,
    TileCal3  = 6,
    TileExt1  = 7,
    TileExt2  = 8,
    TileExt3  = 9,
    EMEC1     = 10,
    EMEC2     = 11,
    EMEC3     = 12,
    HEC1      = 13,
    HEC2      = 14,
    HEC3      = 15,
  };

  enum CaloSection{
    ECal = 0,
    HCal = 1
  };


  enum CaloLayer{
    PS    = 0,
    EM1   = 1,
    EM2   = 2,
    EM3   = 3,
    HAD1  = 4,
    HAD2  = 5,
    HAD3  = 6,
  };


} 

#endif
