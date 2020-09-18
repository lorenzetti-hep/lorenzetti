
__all__ = ["CaloRings"]


from Gaugi import EDM
from Gaugi import StatusCode
from GaugiKernel import Dataframe as DataframeEnum


class CaloRings(EDM):

  # define all skimmed branches here.
  __eventBranches = {
      "Lorenzetti_v1" : [
													"cl_ringer_match",
  												"cl_rings",
                      ]
                  }

  def __init__(self):
    EDM.__init__(self)
  

  def initialize(self):
    
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      branches = self.__eventBranches['Lorenzetti_v1']
      # Link all branches 
      for branch in branches:
        self._logger.debug(branch)
        self.setBranchAddress( self._tree, branch  , self._event)
        self._branches.append(branch) # hold all branches from the body class
    
    else:
      self._logger.warning( "CaloCluster object can''t retrieved" )
      return StatusCode.FAILURE
    # Success

    return StatusCode.SUCCESS
  
  
  
  def ringsE(self):
    """
      Retrieve the rings information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_rings
    else:
      self._logger.warning("Impossible to retrieve the value of Calo rings.")
      return -999
  
  
  def isValid(self):
    """
      Check if cluster match with the seed
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_match
    else:
      self._logger.warning("Impossible to retrieve the value of Calo match flag.")
      return -999


