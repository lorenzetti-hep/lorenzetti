
__all__ = ["InnerSampling", "InnerDetector"]

from GaugiKernel import EnumStringification


class InnerDetector(EnumStringification):
    PIXEL = 0


#
# NOTE: If you change something here, you must do the same at enumeration.h
#
class InnerSampling(EnumStringification):
   PIXEL1 = 0
   PIXEL2 = 1
   PIXEL3 = 2

