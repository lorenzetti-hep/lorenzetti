__all__ = ["CaloCutBasedHypoTool","CaloCutBasedHypoCfg"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
from CaloCell.CaloDefs import CaloSampling
import ROOT
import numpy as np

class CaloCutBasedHypoTool( Cpp ):
    def __init__( self, name,
                  OutputLevel      : str=0,
    ):
        Cpp.__init__(self, ROOT.CaloCutBasedHypoTool(name) )


def CaloCutBasedHypoCfg( name             : str, 
                         OutputLevel      : int=0, 

                       ):

  hypo   = CaloCutBasedHypoTool(   "CaloCutBasedHypoTool",
                                    OutputLevel   = OutputLevel)
  return hypo

