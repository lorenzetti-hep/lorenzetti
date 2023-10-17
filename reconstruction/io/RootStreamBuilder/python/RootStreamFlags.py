
__all__ = ["RootStreamFlags"]

from GaugiKernel import EnumStringification


class RootStreamFlags(EnumStringification):

    EtaWindow = 0.6 
    PhiWindow = 0.6
    OnlyRoI   = True
    SaveCells = False


