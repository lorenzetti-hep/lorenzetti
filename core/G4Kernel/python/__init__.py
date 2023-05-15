
__all__ = ["treatPropertyValue"]



from GaugiKernel import list2stdvector



def treatPropertyValue( value ):

  if (type(value) is list) and (type(value[0]) is str):
    return list2stdvector('string', value)
  elif (type(value) is list) and (type(value[0]) is int):
    return list2stdvector('int', value)
  elif (type(value) is list) and (type(value[0]) is float):
    return list2stdvector('float', value)
  elif (type(value) is list) and (type(value[0]) is bool):
    return list2stdvector('bool', value)
  # list of list with ints, should be vector<vector<int>>
  elif (type(value) is list) and (type(value[0]) is list) and (type(value[0][0]) is int) :
    from ROOT.std import vector
    vec = vector("vector<int>")()
    for v in value:
      vec.push_back( list2stdvector('int', v) )
    return vec
  else:
    return value



from . import ComponentAccumulator
__all__.extend(ComponentAccumulator.__all__)
from .ComponentAccumulator import *

from . import CaloPhiRange
__all__.extend(CaloPhiRange.__all__)
from .CaloPhiRange import *

from . import EventReader
__all__.extend(EventReader.__all__)
from .EventReader import *

from . import DetectorConstruction
__all__.extend(DetectorConstruction.__all__)
from .DetectorConstruction import *


