__all__ = ["CaloCellMerge"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
import ROOT

class CaloCellMerge( Cpp ):


  def __init__( self, name          : str, 
                InputCollectionKeys : str="Collection" ,
                OutputCellsKey      : str="Cells",
                OutputTruthCellsKey : str="TruthCells",
                OutputLevel         : int=0 
                ): 
    
    Cpp.__init__(self, ROOT.CaloCellMerge(name) )
    # Create the algorithm
    self.setProperty( "InputCollectionKeys" , InputCollectionKeys ) 
    self.setProperty( "OutputCellsKey"      , OutputCellsKey      ) 
    self.setProperty( "OutputTruthCellsKey" , OutputTruthCellsKey ) 
    self.setProperty( "OutputLevel"         , OutputLevel         ) 

  








