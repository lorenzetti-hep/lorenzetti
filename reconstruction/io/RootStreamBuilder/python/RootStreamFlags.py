
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
    # list of hash IDs of noisy cells
    cellHash         = [
                        [ 20025610,  20025611,  20025612,  20025613], # center EMB1
                        # [ 30029160,  30029161,  30029162,  30029163], # center EMB2
                        # [ 40013020,  40013021,  40013022,  40013023], # center EMB3
                        # [250000120, 250000121, 250000122, 250000123], # right TileExt1
                        # [280000440, 280000441, 280000442, 280000443], # right EMEC1
                        # [320000560, 320000561, 320000562, 320000563], # right HEC2
                        # [440000106, 440000107, 440000108, 440000109], # left TileExt3
                        # [470005110, 470005111, 470005112, 470005113], # left EMEC3
                        # [500000290, 500000291, 500000292, 500000293], # left HEC3
                        ]
    # factor by which noise is increased in cell
    noiseFactor      = [5] #, 5, 5, 5, 5, 5, 5, 5, 5]           
    # list of event numbers from where noise should start and end
    noisyEvents      = [[1,100]]
    # [[1, 100], [201,300], [301,400], [401, 500],
    #                     [501, 600], [601,700], [701, 800], [801, 900], 
    #                     [901,1000]] 

class RootStreamAODFlags(EnumStringification):

    DumpCells = True