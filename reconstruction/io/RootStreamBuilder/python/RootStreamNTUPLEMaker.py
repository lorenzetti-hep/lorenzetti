__all__ = ["RootStreamNTUPLEMaker"]

from GaugiKernel import Cpp
from GaugiKernel.macros import *
import ROOT

class RootStreamNTUPLEMaker( Cpp ):

  def __init__( self, name,
                InputEventKey    : str,
                InputTruthKey    : str,
                InputCellsKey    : str,
                InputClusterKey  : str,
                InputRingerKey   : str,
                InputFile        : str,
                OutputLevel      : int=0, 
                NtupleName       : str="CollectionTree",
                OutputNtupleName : str="events",
                SecondLambdaCuts  :list=[], #[loose,medium,tight]
                LateralMomCuts   : list=[],
                LongMomCuts      : list=[],
                FracMaxCuts      : list=[],
                SecondRCuts      : list=[],
                LambdaCenterCuts : list=[],
              ): 
    
    Cpp.__init__(self, ROOT.RootStreamNTUPLEMaker(name))
    self.setProperty( "InputEventKey"   , InputEventKey   )
    self.setProperty( "InputTruthKey"   , InputTruthKey   )
    self.setProperty( "InputCellsKey"   , InputCellsKey   )
    self.setProperty( "InputClusterKey" , InputClusterKey )
    self.setProperty( "InputRingerKey"  , InputRingerKey  )
    self.setProperty( "InputFile"       , InputFile       )
    self.setProperty( "SecondLambdaCuts", SecondLambdaCuts )
    self.setProperty( "LateralMomCuts"  , LateralMomCuts)
    self.setProperty( "LongMomCuts"     , LongMomCuts)
    self.setProperty( "FracMaxCuts"     , FracMaxCuts)
    self.setProperty( "SecondRCuts"     , SecondRCuts)
    self.setProperty( "LambdaCenterCuts", LambdaCenterCuts)


    # self.setProperty( "OutputEventKey"  , OutputEventKey if OutputEventKey else InputEventKey       )
    # self.setProperty( "OutputTruthKey"  , OutputTruthKey if OutputTruthKey else InputTruthKey       )
    # self.setProperty( "OutputCellsKey"  , OutputCellsKey if OutputCellsKey else InputCellsKey       )
    # self.setProperty( "OutputClusterKey", OutputClusterKey if OutputClusterKey else InputClusterKey )
    # self.setProperty( "OutputRingerKey" , OutputRingerKey if OutputRingerKey else InputRingerKey    )
    # self.setProperty( "OutputSeedsKey"  , OutputSeedsKey if OutputSeedsKey else InputSeedsKey       )
    
    self.setProperty( "OutputLevel"     , OutputLevel     ) 
    self.setProperty( "NtupleName"      , NtupleName      )
    self.setProperty( "OutputNtupleName", OutputNtupleName)

    f = ROOT.TFile( self.InputFile,"read")
    t = f.Get( self.NtupleName )
    self.__entries = t.GetEntries()


  def GetEntries(self):
    return self.__entries


  def merge(self, acc):
    acc.SetReader(self)