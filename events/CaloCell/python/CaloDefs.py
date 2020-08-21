
__all__ = ["CaloLayer", "CaloSection"]

from Gaugi import EnumStringification


class CaloLayer(EnumStringification):
  PS    = 0
  EM1   = 1
  EM2   = 2
  EM3   = 3
  HAD1  = 4
  HAD2  = 5
  HAD3  = 6



class CaloSection(EnumStringification):
  ECal  = 0
  HCal  = 1


