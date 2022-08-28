
__all__ = ["CaloSampling", "Detector"]

from GaugiKernel import EnumStringification


class Detector(EnumStringification):
  LAR     = 0
  TILE    = 1
  TTEM    = 2
  TTHEC   = 3
  FCALEM  = 5
  FCALHAD = 6


#
# NOTE: If you change something here, you must do the same at enumeration.h
#
class CaloSampling(EnumStringification):
    PSB       = 0
    PSE       = 1
    EMB1      = 2
    EMB2      = 3
    EMB3      = 4
    TileCal1  = 5
    TileCal2  = 6
    TileCal3  = 7
    TileExt1  = 8
    TileExt2  = 9
    TileExt3  = 10
    EMEC1     = 11
    EMEC2     = 12
    EMEC3     = 13
    HEC1      = 14
    HEC2      = 15
    HEC3      = 16


