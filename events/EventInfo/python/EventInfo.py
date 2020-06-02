
__all__ = ["EventInfo"]


from Gaugi import EDM
from Gaugi import StatusCode
from GaugiKernel import Dataframe as DataframeEnum


class EventInfo(EDM):

  # define all skimmed branches here.
  __eventBranches = {
      "Lorenzett_v1" : [
                        "EventNumber",
                        "avgmu",
                        "seed_et",
                        "seed_eta",
                        "seed_phi",
                      ]
      }

  def __init__(self):
    EDM.__init__(self)
  

  def initialize(self):
    if self._dataframe is DataframeEnum.Lorenzett_v1:
      branches = self.__eventBranches['Lorenzett_v1']
      # Link all branches 
      for branch in branches:
        self._logger.debug(branch)
        self.setBranchAddress( self._tree, branch  , self._event)
        self._branches.append(branch) # hold all branches from the body class
    else:
      self._logger.warning( "EventInfo object can''t retrieved" )
      return StatusCode.FAILURE
    # Success

    return StatusCode.SUCCESS
  
  def eta(self):
    """
      Retrieve the eta information from Lorenzett
    """
    if self._dataframe is DataframeEnum.Lorenzett_v1:
      return self._event.seed_eta
    else:
      self._logger.warning("Impossible to retrieve the value of eta seed")
      return -999
  
  def phi(self):
    """
      Retrieve the phi information from Lorenzett
    """
    if self._dataframe is DataframeEnum.Lorenzett_v1:
      return self._event.seed_phi
    else:
      self._logger.warning("Impossible to retrieve the value of phi seed")
      return -999
  
  def et(self):
    """
      Retrieve the et information from Lorenzett
    """
    if self._dataframe is DataframeEnum.Lorenzett_v1:
      return self._event.seed_et
    else:
      self._logger.warning("Impossible to retrieve the value of et seed")
      return -999
  
  def avgmu(self):
    """
      Retrieve the avgmu information from Lorenzett
    """
    if self._dataframe is DataframeEnum.Lorenzett_v1:
      return self._event.avgmu
    else:
      self._logger.warning("Impossible to retrieve the value of avgmu")
      return -999
  
  def eventNumber(self):
    """
      Retrieve the event number information from Lorenzett
    """
    if self._dataframe is DataframeEnum.Lorenzett_v1:
      return self._event.eventNumber
    else:
      self._logger.warning("Impossible to retrieve the value of event number")
      return -999
  









