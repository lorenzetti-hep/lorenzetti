
__all__ = ['CaloSamplingMaker']

class CaloSamplingMaker:

  def __init__( self, name, collectionKey, cells, **kw ):
    self.segments = []
    class Segment:
      def __init__( self, name, collectionKey, **kw ): 
        self.name = name
        self.CollectionKey = collectionKey
        for key, value in kw.items():
            setattr( self, key , value )
    
    for idx, config in enumerate(cells):
      for key, value in config.items():
        kw[key] = value
      self.segments.append( Segment( name+"_"+str(idx), collectionKey+"_"+str(idx), **kw ) )




from . import CaloCellBuilder
__all__.extend(CaloCellBuilder.__all__)
from .CaloCellBuilder import *

from . import CaloHitBuilder
__all__.extend(CaloHitBuilder.__all__)
from .CaloHitBuilder import *

from . import DetectorLayers
__all__.extend(DetectorLayers.__all__)
from .DetectorLayers import *

from . import DetectorConstruction
__all__.extend(DetectorConstruction.__all__)
from .DetectorConstruction import *


