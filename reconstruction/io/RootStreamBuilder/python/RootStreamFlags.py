
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
    doDefects        = False         # whether to simulate detector defects
    # hash ID of noisy cells
    cellHash         = [440000106, 440000107, 440000108, 440000109, 
                        470005110, 470005111, 470005112, 470005113]  
    noiseFactor      = 5            # factor by which noise is increased in cell
    startNoise       = 1            # event number from where noise should start
    endNoise         = 20           # event number where noise ends

class RootStreamAODFlags(EnumStringification):

    DumpCells = True