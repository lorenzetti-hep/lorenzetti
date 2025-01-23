
__all__ = ['list2stdvector', 'stdvector2list', "get_argparser_formatter", "get_attributes", "check_for_unused_vars"]


import numpy as np
import ROOT
import inspect

ROOT.gSystem.Load('liblorenzetti')
from ROOT import RunManager
from rich_argparse import RichHelpFormatter





def check_for_unused_vars(d, fcn = None):
  """
    Checks if dict @d has unused properties and print them as warnings
  """
  for key in d.keys():
    if d[key] is None: continue
    msg = 'Obtained not needed parameter: %s' % key
    if fcn:
      fcn(msg)
    else:
      print('WARNING:%s' % msg)



def get_argparser_formatter( custom : bool=True):
    if custom:
        RichHelpFormatter.styles["argparse.args"]     = "green"
        RichHelpFormatter.styles["argparse.prog"]     = "bold grey50"
        RichHelpFormatter.styles["argparse.groups"]   = "bold green"
        RichHelpFormatter.styles["argparse.help"]     = "grey50"
        RichHelpFormatter.styles["argparse.metavar"]  = "blue"
        return RichHelpFormatter
    else:
        return argparse.HelpFormatter

def get_attributes(o, **kw):
  """
    Return attributes from a class or object.
  """
  onlyVars = kw.pop('onlyVars', False)
  getProtected = kw.pop('getProtected', True)
  check_for_unused_vars(kw)
  return [(a[0] if onlyVars else a) for a in inspect.getmembers(o, lambda a:not(inspect.isroutine(a))) \
             if not(a[0].startswith('__') and a[0].endswith('__')) \
                and (getProtected or not( a[0].startswith('_') or a[0].startswith('__') ) ) ]


def list2stdvector(vecType,l):
  from ROOT.std import vector
  vec = vector(vecType)()
  for v in l:
    vec.push_back(v)
  return vec


def stdvector2list(vec, size=None):
  if size:
    l=size*[0]
  else:
    l = vec.size()*[0]
  for i in range(vec.size()):
    l[i] = vec[i]
  return l

from . import enumerators
__all__.extend(enumerators.__all__)
from .enumerators import *

from . import Logger
__all__.extend(Logger.__all__)
from .Logger import *

from . import Cpp
__all__.extend(Cpp.__all__)
from .Cpp import *

from . import constants
__all__.extend(constants.__all__)
from .constants import *

from . import ComponentAccumulator
__all__.extend(ComponentAccumulator.__all__)
from .ComponentAccumulator import *

from . import MultiProcessing
__all__.extend(MultiProcessing.__all__)
from .MultiProcessing import *

