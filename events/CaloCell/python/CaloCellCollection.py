__all__ = ["CaloCellCollection","CaloCell","CaloLayer"]


from Gaugi import EDM, EnumStringification
from Gaugi import StatusCode
from GaugiKernel import Dataframe as DataframeEnum
import numpy as np


class CaloLayer(EnumStringification):
  PS = 0
  EM1= 1
  EM2= 2 
  EM3= 3
  HAD1=4 
  HAD2=5 
  HAD3=6
	

class CaloCell(EDM):
  
  __allow_keys = ['layer', 'eta', 'phi', 'deta', 'dphi', 'energy', 'et']
  
  def __init__(self, **kw ):
    EDM.__init__(self)
    self.__dict__.update(('_'+k, v) for k, v in kw.items() if k in self.__allow_keys)
  
  def et(self):
  	return self._et
  
  def eta(self):
  	return self._eta
  
  def phi(self):
  	return self._phi
  
  def layer(self):
  	return self._layer
  
  def deltaEta(self):
  	return self._deta
  
  def deltaPhi(self):
  	return self._dphi
  
  def energy(self):
    return self._energy





class CaloCellCollection(EDM):

  # define all skimmed branches here.
  __eventBranches = {
      "Lorenzetti_v1" : [
												"cl_cell_et",        
												"cl_cell_eta",       
												"cl_cell_phi",       
												"cl_cell_deta",      
												"cl_cell_dphi",      
												"cl_cell_energy",    
												"cl_cell_layer",    
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
      self._logger.warning( "CaloCellCollection object can''t retrieved" )
      return StatusCode.FAILURE
    # Success
    return StatusCode.SUCCESS
  
  
  def getCollection(self, layer):
    """
      Retrieve the rings information from Lorenzett
    """
	
    collection = list()

    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      size = self._event.cl_cell_energy.size()
      for idx in range(size): # Loop over all cells
        if layer is self._event.cl_cell_layer.at(idx):
          cell = CaloCell( et 			  = self._event.cl_cell_et.at(idx),
          								 eta			  = self._event.cl_cell_eta.at(idx),
          								 phi			  = self._event.cl_cell_phi.at(idx),
          								 deta			  = self._event.cl_cell_deta.at(idx),
          								 dphi			  = self._event.cl_cell_dphi.at(idx),
          								 energy		  = self._event.cl_cell_energy.at(idx),
          								 layer   	  = self._event.cl_cell_layer.at(idx),
                           )
          collection.append(cell)
          
    else:
      self._logger.warning("Impossible to retrieve all CaloCells.")
    return collection




  def isValid(self):
    """
      Check if cluster match with the seed
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return not self._event.cell_energy.empty()
    else:
      self._logger.warning("Impossible to retrieve the value of Calo match flag.")
      return -999


