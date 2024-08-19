
__all__ = ["RootStreamHITFlags", "RootStreamESDFlags", "RootStreamAODFlags"]


from GaugiKernel import EnumStringification


class RootStreamHITFlags(EnumStringification):

    OnlyRoI          = True
    EtaWindow        = 0.4
    PhiWindow        = 0.4

class RootStreamESDFlags(EnumStringification):
    
    EtaWindow        = 0.4
    PhiWindow        = 0.4

    # new ESD flags
    doDefects        = True
    cellHash         = 440000107  # need hash ID of noisy cells
    noiseBurst       = 5


class RootStreamAODFlags(EnumStringification):

    DumpCells = True