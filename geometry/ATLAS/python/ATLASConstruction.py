
__all__ = ["ATLASConstruction"]

#
# ATLAS Detector construction
#
from ATLAS import flatten
from G4Kernel.DetectorConstruction import DetectorConstruction
import os

# import samplings
#from Tracking import createTracking
from ATLAS.ECAL import *
from ATLAS.TILE import *
from ATLAS.EMEC import *
from ATLAS.HEC  import *
from ATLAS.DeadMaterials import *
from ATLAS.Tracking      import *



samplings = []; volumes = []; tracking = []

# Center
#volumes.extend( getPixelBarrelCfg()   )
samplings.extend( getLArBarrelCfg()   )
samplings.extend( getTileBarrelCfg()  )
volumes.extend( getDMVolumesCfg()      )


# Right side (A)
samplings.extend( getTileExtendedCfg()    )
samplings.extend( getLArEMECCfg()         ) 
samplings.extend( getHECCfg()             )
volumes.extend( getCrackVolumesCfg()      )

# Left side (B)
samplings.extend( getTileExtendedCfg(left_side = True) )
samplings.extend( getLArEMECCfg(left_side=True)        ) 
samplings.extend( getHECCfg(left_side=True)            )
volumes.extend( getCrackVolumesCfg(left_side=True)     )


# Do NOT uncomment this unless you know what you are doing.
#volumes.extend( getPixelBarrelCfg() )


vispath = os.environ['LZT_PATH']+'/geometry/ATLAS/data/vis.mac'


#
# Create the detector
#
class ATLASConstruction(DetectorConstruction):
    def __init__(self, **kw):
        DetectorConstruction.__init__(self, "ATLAS", vispath,
                                            samplings=flatten(samplings), 
                                            trackings=flatten(tracking),
                                            **kw)
        # Add extra volumes which is not a sensitive detector
        for pv in flatten(volumes):
            self+=pv
    

if __name__ == "__main__":
    atlas = ATLASConstruction()
    atlas.summary()
    #atlas.compile()

