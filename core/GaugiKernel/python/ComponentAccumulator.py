__all__ = ["ComponentAccumulator"]

from GaugiKernel import Logger
from typing import List
import numpy as np
import os

#
# Componenent 
#
class ComponentAccumulator( Logger ):

  def __init__(self, 
               name            : str, 
               output          : str, 
              ):
    
    Logger.__init__(self)
    import ROOT
    ROOT.gSystem.Load('liblorenzetti')
    from ROOT import RunManager
    from ROOT import Gaugi as GK
    self.__acc = GK.ComponentAccumulator( name )
    from ROOT import SG
    self.__ctx = SG.EventContext("EventContext")
    self.__store = SG.StoreGate(output)
    self.__ctx.setStoreGateSvc(self.__store)

  def SetReader(self, reader):
    self.__reader = reader
    self.__acc.push_back(reader.core())

  def __add__( self, algs ):
    if type(algs) is not list:
      algs = [algs]
    for alg in algs:
      self.__acc.push_back(alg.core())
    return self

  def GetEntries(self):
    return self.__reader.GetEntries()

  def configure(self):
    self.__acc.initialize()
    self.__acc.bookHistograms(self.__ctx)

 
  def run( self , events : List[int] ):
    self.configure()
    for evt in events:
      self.__acc.run(self.__ctx, evt)
    self.__acc.finalize()
    self.__ctx.getStoreGateSvc().save()
 