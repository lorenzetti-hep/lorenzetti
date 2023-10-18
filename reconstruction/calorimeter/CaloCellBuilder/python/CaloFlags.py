
__all__ = ["CaloFlags"]


from GaugiKernel import EnumStringification
from GaugiKernel.constants import *

class CaloFlags(EnumStringification):

    # cell global parameters
    SamplingnoiseStd    = 0.0 
    # crosstalk flags
    DoCrossTalk         = False
    XTMinEnergy         = 1*GeV
    XTAmpCapacitive     = 4.2
    XTAmpInductive      = 2.3
    XTAmpResistive      = 1.0