
__all__ = ['EventSimulator']

from Gaugi.messenger import Logger, LoggingLevel
from Gaugi.messenger.macros import *
from lorenzet.enumerations import Dataframe as DataframeEnum
from Gaugi import StatusCode
from Gaugi.gtypes import NotSet
from Gaugi import TEventLoop

# Import all root classes
import ROOT


# The main framework base class for SIM analysis.
# This class is responsible to build all containers object and
# manager the storegate and histogram services for all classes.
class EventSimulator( TEventLoop ):
    
  def __init__(self, name, **kw):
    # Retrieve all information needed
    TEventLoop.__init__(self, name, **kw)
    # Loading libraries
    ROOT.gSystem.Load('liblorenzet')
    
  # Initialize all services
  def initialize( self ):

    MSG_INFO( self, 'Initializing EventSimulator...')
    
    if super(EventSimulator,self).initialize().isFailure():
      MSG_FATAL( self, "Impossible to initialize the TEventLoop services.")

   
    # RingerPhysVal hold the address of required branches
    if self._dataframe is DataframeEnum.Lorenzet:
      #self._t.SetBranchStatus("*", False)
      from ROOT import edm
      self._event = edm.Lorenzet()
      self._t.GetEntry(0)
    elif self._dataframe is DataframeEnum.Delphes:
      try:
        ROOT.gSystem.Load("libDelphes")
      except:
        MSG_FATAL( self, "Can not import Delphes library. You should install delphes before! Bye...")
      try:
        ROOT.gInterpreter.Declare('#include "classes/DelphesClasses.h"')
        ROOT.gInterpreter.Declare('#include "external/ExRootAnalysis/ExRootTreeReader.h"')
      except:
        MSG_WARNING( self, "Can not include DelphesClasses/ExRootTreeReader.")
      self._event = NotSet
      self._t = ROOT.ExRootTreeReader(self._t)
      self._t.ReadEntry(0)
    else:
      return StatusCode.FATAL


    MSG_INFO( self, "Creating containers...")
    # Allocating containers
    if self._dataframe is DataframeEnum.Lorenzet:
      from EventSimulator import CaloCells, CaloRings, ShowerShapes
    elif self._dataframe is DataframeEnum.Delphes:
      from EventDelphes import CaloTowers
    else:
      pass
    
    # Initialize the base of this container. 
    # Do not change this key names!
    # NOTE: Do not change this order.
    # we must retrieve the cells first and the reco other features
    # event dataframe containers
    self._containersSvc['CaloCellsContainer']    = CaloCells() 
    self._containersSvc['CaloRingsContainer']    = CaloRings()
    self._containersSvc['ShowerShapesContainer'] = ShowerShapes()
                           

    # configure all EDMs needed
    for key, edm in self._containersSvc.items():
      # attach the EDM pointer into the context list
      self.getContext().setHandler(key,edm)
      # add properties
      edm.dataframe = self._dataframe
      edm.tree  = self._t
      edm.level = self._level
      edm.event = self._event
      edm.setContext(self.getContext())
      # If initializations is failed, we must remove this from the container 
      # service
      if(edm.initialize().isFailure()):
        MSG_WARNING( self, 'Impossible to create the EDM: %s',key)

    self.getContext().initialize()

    return StatusCode.SUCCESS


  def getEntry( self, entry ):
    if self._dataframe is DataframeEnum.Lorenzet:
      self._t.GetEntry( entry )
    elif self._dataframe is DataframeEnum.Delphes:
      self._t.ReadEntry( entry )
    else:
      self._t.GetEntry( entry )



