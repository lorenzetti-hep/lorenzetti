
__all__ = ["CaloCluster"]


from Gaugi import EDM
from Gaugi import StatusCode
from GaugiKernel import Dataframe as DataframeEnum
import numpy as np

class CaloCluster(EDM):

  # define all skimmed branches here.
  __eventBranches = {
      "Lorenzetti_v1" : [
												"cl_match",
  											"cl_eta",
  											"cl_phi",     
  											"cl_et",      
  											"cl_e1",      
  											"cl_e2",      
  											"cl_e3",      
  											"cl_ehad1",   
  											"cl_ehad2",   
  											"cl_ehad3",   
  											"cl_etot",    
  											"cl_reta",    
  											"cl_rphi",    
  											"cl_rhad",    
  											"cl_eratio",  
  											"cl_f0",      
  											"cl_f1",      
  											"cl_f2",      
  											"cl_f3",      
  											"cl_weta2",   
  											"cl_e233",    
  											"cl_e237",    
  											"cl_e277",    
  											"cl_emaxs1",  
  											"cl_e2tsts1", 
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
        self.setBranchAddress( self._tree, branch , self._event)
        self._branches.append(branch) # hold all branches from the body class
    
    else:
      self._logger.warning( "CaloCluster object can''t retrieved" )
      return StatusCode.FAILURE
    # Success

    return StatusCode.SUCCESS




  def et(self):
    """
      Retrieve the Et information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_et
    else:
      self._logger.warning("Impossible to retrieve the value of Calo Et.")
      return -999


  def eta(self):
    """
      Retrieve the Eta information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_eta
    else:
      self._logger.warning("Impossible to retrieve the value of Calo Eta.")
      return -999



  def phi(self):
    """
      Retrieve the phi information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_phi
    else:
      self._logger.warning("Impossible to retrieve the value of Calo Phi.")
      return -999




  def e1(self):
    """
      Retrieve the e1 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_e1
    else:
      self._logger.warning("Impossible to retrieve the value of Calo e1.")
      return -999




  def e2(self):
    """
      Retrieve the e2 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_e2
    else:
      self._logger.warning("Impossible to retrieve the value of Calo e2.")
      return -999


  def e3(self):
    """
      Retrieve the e3 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_e3
    else:
      self._logger.warning("Impossible to retrieve the value of Calo e3.")
      return -999



  def ehad1(self):
    """
      Retrieve the ehad1 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_ehad1
    else:
      self._logger.warning("Impossible to retrieve the value of Calo ehad1.")
      return -999



  def ehad2(self):
    """
      Retrieve the ehad2 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_ehad2
    else:
      self._logger.warning("Impossible to retrieve the value of Calo ehad2.")
      return -999



  def ehad3(self):
    """
      Retrieve the ehad3 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_ehad3
    else:
      self._logger.warning("Impossible to retrieve the value of Calo ehad3.")
      return -999




  def etot(self):
    """
      Retrieve the etot information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_etot
    else:
      self._logger.warning("Impossible to retrieve the value of Calo etot.")
      return -999



  def reta(self):
    """
      Retrieve the ehad3 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_reta
    else:
      self._logger.warning("Impossible to retrieve the value of Calo reta.")
      return -999



  def rphi(self):
    """
      Retrieve the ehad3 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_rphi
    else:
      self._logger.warning("Impossible to retrieve the value of Calo rphi.")
      return -999
  
  
  def rhad(self):
    """
      Retrieve the ehad3 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_rhad
    else:
      self._logger.warning("Impossible to retrieve the value of Calo rhad.")
      return -999
  
  
  def eratio(self):
    """
      Retrieve the eratio information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_eratio
    else:
      self._logger.warning("Impossible to retrieve the value of Calo eratio.")
      return -999
  
  
  def f0(self):
    """
      Retrieve the f0 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_f0
    else:
      self._logger.warning("Impossible to retrieve the value of Calo f0.")
      return -999
  
  def f1(self):
    """
      Retrieve the f1 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_f1
    else:
      self._logger.warning("Impossible to retrieve the value of Calo f1.")
      return -999
  
  
  def f2(self):
    """
      Retrieve the f2 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_f2
    else:
      self._logger.warning("Impossible to retrieve the value of Calo f2.")
      return -999
  
  
  def f3(self):
    """
      Retrieve the f3 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_f3
    else:
      self._logger.warning("Impossible to retrieve the value of Calo f3.")
      return -999
  
  
  
  def weta2(self):
    """
      Retrieve the weta2 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_weta2
    else:
      self._logger.warning("Impossible to retrieve the value of Calo weta2.")
      return -999
  
  
  def e233(self):
    """
      Retrieve the e233 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_e233
    else:
      self._logger.warning("Impossible to retrieve the value of Calo e233.")
      return -999
  
  
  
  def e237(self):
    """
      Retrieve the e237 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_e237
    else:
      self._logger.warning("Impossible to retrieve the value of Calo e237.")
      return -999
  
  
  
  def e277(self):
    """
      Retrieve the e277 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_e277
    else:
      self._logger.warning("Impossible to retrieve the value of Calo e277.")
      return -999
  
  
  
  def emaxs1(self):
    """
      Retrieve the emaxs1 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_emaxs1
    else:
      self._logger.warning("Impossible to retrieve the value of Calo emaxs1.")
      return -999
  
  
  
  def e2tsts1(self):
    """
      Retrieve the e2tsts1 information from Lorenzetti
    """
    if self._dataframe is DataframeEnum.Lorenzetti_v1:
      return self._event.cl_e2tsts1
    else:
      self._logger.warning("Impossible to retrieve the value of Calo e2tsts1.")
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


  def caloRings(self):
    """
      Get the ringer object
    """
    return self.getContext().getHandler( "CaloRingsContainer" )



  def getCells(self, layer):

    collection = self.getContext().getHandler( "CaloCellsContainer" )
    # get all cells for this specific layer
    return collection.getCollection( layer )
  


  def getCells(self, layer ):
    collection = self.getContext().getHandler( "CaloCellsContainer" )
    # get all cells for this specific layer
    return collection.getCollection( layer )








