__all__ = ["CaloCellMerge"]

from GaugiKernel import Logger
from GaugiKernel.macros import *
from G4Kernel import treatPropertyValue


class CaloCellMerge( Cpp ):


  def __init__( self, name, 
                InputCollectionKeys : str="Collection" ,
                OutputCellsKey      : str="Cells",
                OutputTruthCellsKey : str="TruthCells",
                OutputLevel         : int=0 
                ): 
    
    Cpp.__init__(self, name, "Root.CaloCellMerge", OutputLevel=OutputLevel)
    # Create the algorithm
    self.setProperty( "InputCollectionKeys" , InputCollectionKeys ) 
    self.setProperty( "OutputCellsKey"      , OutputCellsKey      ) 
    self.setProperty( "OutputTruthCellsKey" , OutputTruthCellsKey ) 
    self.setProperty( "OutputLevel"         , OutputLevel         ) 

  








