
__all__ = ["ATLASDetector"]

#
# ATLAS Detector construction
#

from G4Kernel.DetectorConstruction import DetectorConstruction
import os

# import samplings
#from Tracking import createTracking
from ATLAS.ECAL import createECal
from ATLAS.TileCal import createTileCal
from ATLAS.TileCalExt import createTileCalExt
from ATLAS.EMEC import createEMEC
from ATLAS.HEC import createHEC
from ATLAS.Crack import createCrackVolumes
from ATLAS.DeadMaterials import createDMVolumes


def flatten(l):
    o = []
    for item in l:
        if type(item) is list:
            o.extend(item)
        else:
            o.append(item)
    return o



samplings = []

# Center
#samplings.extend( createTracking()                   )
samplings.extend( createECal()                       )
samplings.extend( createTileCal()                    )

# Right side (A)
samplings.extend( createTileCalExt()                 )
samplings.extend( createEMEC()                       )
samplings.extend( createHEC()                        )

# Left side (B)
samplings.extend( createTileCalExt(left_side = True) )
samplings.extend( createEMEC(left_side = True)       )
samplings.extend( createHEC( left_side = True)       )


extra_volumes = createCrackVolumes()
extra_volumes.extend( createCrackVolumes(left_side=True) )
extra_volumes.extend( createDMVolumes() )

vis_mac = os.environ['LZT_PATH']+'/geometry/ATLAS/data/vis.mac'


#
# Create the detector
#
class ATLASDetector(DetectorConstruction):
    def __init__(self, **kw):
        DetectorConstruction.__init__(self, "ATLAS", samplings=flatten(samplings), vis_mac=vis_mac, **kw)
        # Add extra volumes which is not a sensitive detector
        for pv in flatten(extra_volumes):
            self+=pv
        


if __name__ == "__main__":
    atlas = ATLASDetector()
    atlas.summary()
    #atlas.compile()

