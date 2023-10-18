__all__ = ["CaloClusterFlags"]

from GaugiKernel import EnumStringification
from GaugiKernel.constants import *

class CaloClusterFlags(EnumStringification):

    MinCenterEnergy  = 1*GeV
    EtaWindow        = 0.4
    PhiWindow        = 0.4