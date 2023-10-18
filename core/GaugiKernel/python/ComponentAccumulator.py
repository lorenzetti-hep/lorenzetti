
__all__ = ["ComponentAccumulator"]

from GaugiKernel import Logger
import numpy as np
import os

#
# Componenent 
#
class ComponentAccumulator( Logger ):


  def __init__(self, name, output, detector=None ):
    
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
    self.__detector = detector


  #
  # Set the reader as first alg
  #
  def SetReader(self, reader):
    self.__reader = reader
    self.__acc.push_back(reader.core())

  #
  # Add algorith to the main sequence
  #
  def __add__( self, algs ):
    if type(algs) is not list:
      algs = [algs]
    for alg in algs:
      self.__acc.push_back(alg.core())
    return self

  #
  # Get entries
  #
  def GetEntries(self):
    return self.__reader.GetEntries()

  #
  # Configure
  #
  def configure(self):

    self.__acc.initialize()
    self.__acc.bookHistograms(self.__ctx)

  #
  # Run events
  #
  def run( self , nov=None ):

    self.configure()

    if nov < 0:
      nov = self.GetEntries()
  
    elif nov > self.GetEntries():
      nov = self.GetEntries()
  
    for evt in range(nov):
      self.__acc.run(self.__ctx, evt)

    self.__acc.finalize()
    self.__ctx.getStoreGateSvc().save()
 
  

  def detector(self):
    return self.__detector
    
