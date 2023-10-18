
__all__ = ["Calorimeter"]

from GaugiKernel        import Logger

class Calorimeter(Logger):

  __allow_keys = ["CollectionKey", 
                  "Detector", 
                  "Sampling",
                  "Shaper",
                  "Noise",
                  "Samples",
                  "OFWeightsEnergy",
                  "OFWeightsTime",
                  ]

  def __init__(self, sv, bunch_start, bunch_end, sampling_start, **kw):
                    



    Logger.__init__(self)
    for key, value in kw.items():
      if key in self.__allow_keys:
        setattr(self, key, value )
      else:
        MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

    self.sv = sv 
    self.BunchIdStart    = bunch_start
    self.BunchIdEnd      = bunch_end
    self.StartSamplingBC = sampling_start 

  def volume(self):
    return self.sv.volume()

  def sensitive(self):
    return self.sv

  def name(self):
    return self.volume().name() + '_' + str(self.sv.Segment)

