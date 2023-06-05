
__all__ = [
            "MeV",
            "GeV",
            "m","dm","cm","mm","um",
            "pi",
            "SECONDS", "MINUTES"
          ]


import numpy as np

# metric system
m   = 1000
dm  = 100
cm  = 10
mm  = 1
um  = 0.001

# math constants
pi  = np.pi

# energy system
MeV = 1
GeV = 1000 * MeV


SECONDS = 1
MINUTES = 60*SECONDS
