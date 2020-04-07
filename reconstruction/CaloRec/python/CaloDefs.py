
from Gaugi import EnumStringification


class CaloSample(EnumStringification):
  PS    = 0
  EM1   = 1
  EM2   = 2
  EM3   = 3
  HAD1  = 4
  HAD2  = 5
  HAD3  = 6


collection_keys = {
                    CaloSample.EM1  : "CaloCellCollection_EM1" , 
                    CaloSample.EM2  : "CaloCellCollection_EM2" , 
                    CaloSample.EM3  : "CaloCellCollection_EM3" , 
                    CaloSample.HAD1 : "CaloCellCollection_HAD1", 
                    CaloSample.HAD2 : "CaloCellCollection_HAD2", 
                    CaloSample.HAD3 : "CaloCellCollection_HAD3", 
                  }



reco_keys = {
              "CellsKey" , 
            }


def recordable( key ):





