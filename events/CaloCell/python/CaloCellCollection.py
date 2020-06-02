__all__ = ["CaloCellCollection","CaloCell","CaloSample"]


from Gaugi import EDM, EnumStringification
from Gaugi import StatusCode
from GaugiKernel import Dataframe as DataframeEnum



class CaloSample(EnumStringification):
  PS = 0
  EM1= 1
  EM2= 2 
  EM3= 3
  HAD1=4 
  HAD2=5 
  HAD3=6
  HAD1_Extended=7
  HAD2_Extended=8
  HAD3_Extended=9
	

class CaloCell(EDM):
  
  __allow_keys = ['sampling', 'eta', 'phi', 'deta', 'dphi', 'energy', 'et', 'channelEta', 'channelPhi']
  
  def __init__(self, **kw ):
    EDM.__init__(self)
    self.__dict__.update(('_'+k, v) for k, v in kw.items() if k in self.__allow_keys)
  
  def et(self):
  	return self._et
  
  def eta(self):
  	return self._eta
  
  def phi(self):
  	return self._phi
  
  def sampling(self):
  	return self._sampling
  
  def deltaEta(self):
  	return self._deta
  
  def deltaPhi(self):
  	return self._dphi
  
  def energy(self):
    return self._energy




class CaloCellCollection(EDM):

  # define all skimmed branches here.
  __eventBranches = {
      "Lorenzett_v1" : {
        "Truth__CaloCells": [
														"mc_cl_cell_et",    
														"mc_cl_cell_eta",    
														"mc_cl_cell_phi",    
														"mc_cl_cell_deta",   
														"mc_cl_cell_dphi",   
														"mc_cl_cell_energy", 
														"mc_cl_cell_channel_eta", 
														"mc_cl_cell_channel_phi", 
														"mc_cl_cell_sampling", 
                            ],        
        "CaloCells"      : [
														"cl_cell_et",        
														"cl_cell_eta",       
														"cl_cell_phi",       
														"cl_cell_deta",      
														"cl_cell_dphi",      
														"cl_cell_energy",    
														"cl_cell_channel_eta",    
														"cl_cell_channel_phi",    
														"cl_cell_sampling",    
                            ]
                      }
      }

  def __init__(self):
    EDM.__init__(self)
  

  def initialize(self):
    
    if self._dataframe is DataframeEnum.Lorenzett_v1:
      if not self._is_hlt:
        branches = self.__eventBranches['Lorenzett_v1']['Truth__CaloCells']
      else:
        branches = self.__eventBranches['Lorenzett_v1']['CaloCells']
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
  
  
  def getCollection(self, sampling):
    """
      Retrieve the rings information from Lorenzett
    """
	
    collection = list()

    if self._dataframe is DataframeEnum.Lorenzett_v1:
      if not self._is_hlt:				
        size = self._event.mc_cell_energy.size()
        for idx in range(size): # Loop over all cells
          if sampling is self._event.mc_cell_sampling.at(idx):
            cell = CaloCell( et 			  = self._event.mc_cell_et.at(idx),
            								 eta			  = self._event.mc_cell_eta.at(idx),
            								 phi			  = self._event.mc_cell_phi.at(idx),
            								 deta			  = self._event.mc_cell_deta.at(idx),
            								 dphi			  = self._event.mc_cell_dphi.at(idx),
            								 energy		  = self._event.mc_cell_energy.at(idx),
            								 channelEta	= self._event.mc_cell_channel_eta.at(idx),
            								 channlePhi	= self._event.mc_cell_channel_phi.at(idx),
            								 sampling	  = self._event.mc_cell_sampling.at(idx),
                             )

            collection.append(cell)
      else:
        size = self._event.mc_cell_energy.size()
        for idx in range(size): # Loop over all cells
          if sampling is self._event.cell_sampling.at(idx):
            cell = CaloCell( et 			  = self._event.cell_et.at(idx),
            								 eta			  = self._event.cell_eta.at(idx),
            								 phi			  = self._event.cell_phi.at(idx),
            								 deta			  = self._event.cell_deta.at(idx),
            								 dphi			  = self._event.cell_dphi.at(idx),
            								 energy		  = self._event.cell_energy.at(idx),
            								 channelEta	= self._event.cell_channel_eta.at(idx),
            								 channelPhi	= self._event.cell_channel_phi.at(idx),
            								 sampling	  = self._event.cell_sampling.at(idx),
                             )
            collection.append(cell)
          
    else:
      self._logger.warning("Impossible to retrieve all CaloCells.")
    return collection




  def isValid(self):
    """
      Check if cluster match with the seed
    """
    if self._dataframe is DataframeEnum.Lorenzett_v1:
      if not self._is_hlt:
        return not self._event.mc_cell_energy.empty()
      else:
        return not self._event.cell_energy.empty()
    else:
      self._logger.warning("Impossible to retrieve the value of Calo match flag.")
      return -999


