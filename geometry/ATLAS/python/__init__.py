
__all__ = ['flatten']



def flatten(l):
    o = []
    for item in l:
        if type(item) is list:
            o.extend(item)
        else:
            o.append(item)
    return o


from . import Tracking
__all__.extend(Tracking.__all__)
from .Tracking import *

from . import ECAL
__all__.extend(ECAL.__all__)
from .ECAL import *

from . import TILE
__all__.extend(TILE.__all__)
from .TILE import *

from . import EMEC
__all__.extend(EMEC.__all__)
from .EMEC import *

from . import HEC
__all__.extend(HEC.__all__)
from .HEC import *

from . import DeadMaterials
__all__.extend(DeadMaterials.__all__)
from .DeadMaterials import *

from . import ATLASConstruction
__all__.extend(ATLASConstruction.__all__)
from .ATLASConstruction import *




