__all__ = [
           "Plates", 
           "DetectorConstruction", 
           "PhysicalVolume", 
           "SensitiveCaloVolume", 
           "ProductionCuts",
           ]


from GaugiKernel.constants import *
from GaugiKernel import Cpp, Logger
from GaugiKernel.macros import *
from GaugiKernel import EnumStringification

from prettytable import PrettyTable
from tqdm import tqdm
import numpy as np
import collections

import ROOT

class Plates(EnumStringification):
  Horizontal = 0
  Vertical   = 1


class DetectorConstruction( Cpp ):

  def __init__( self, 
                name              : str, 
                vis_path          : str, 
                samplings         : list=[], 
                trackings         : list=[], 
                UseMagneticField  : bool=False, 
                CutOnPhi          : bool=False ):

    Cpp.__init__(self, ROOT.DetectorConstruction(name) ) 
    
    self.setProperty( "UseMagneticField", UseMagneticField  )
    self.setProperty( "CutOnPhi"        , CutOnPhi          )

    self.VisMac = vis_path
    self.samplings = samplings
    self.trackings = trackings
    self.__volumes = collections.OrderedDict({})

    # add all volumes from samplings
    for samp in self.samplings:
      pv = samp.volume(); self+=pv
    for samp in self.trackings:
      pv = samp.volume(); self+=pv 



  def __add__(self, pv):
    if pv.Name not in self.__volumes.keys():
      self.__volumes[pv.Name] = pv
    return self

  #
  # Build the core object
  #
  def compile(self):
    # Create all volumes inside of the detector
    for pv in tqdm( self.__volumes.values(), desc="Compiling...", ncols=70):
      self._core.AddVolume( pv.name(), pv.Plates, pv.AbsorberMaterial, pv.GapMaterial, 
                             # layer
                             pv.NofLayers, pv.AbsorberThickness, pv.GapThickness,
                             # dimensions
                             pv.RMin, pv.RMax, pv.ZSize, pv.X, pv.Y, pv.Z,
                             # production cuts
                             pv.Cuts.ElectronCut, pv.Cuts.PositronCut, pv.Cuts.GammaCut, pv.Cuts.PhotonCut
                             )
    create_vis_mac(self.__volumes.values(), self.VisMac)


  def summary(self):

      samp_vol_names = []

      print('Display all calorimeter samplings...')

      t = PrettyTable(["Name", "Plates", "z",'Zmin','Zmax', "Rmin", 
                       "Rmax", "abso","gap", "deta", "dphi", "EtaMin", 
                       "EtaMax", "N_bins", "Container"])

      # Add all volumes that came from a sampling detector and has a sensitive parameter
      for samp in self.samplings:
        pv = samp.volume(); sv = samp.sensitive(); samp_vol_names.append(pv.Name)
        t.add_row( [pv.Name,
                    Plates.tostring(pv.Plates),pv.ZSize,pv.ZMin,pv.ZMax,pv.RMin,pv.RMax,
                    pv.AbsorberMaterial,pv.GapMaterial,
                    round(sv.DeltaEta,4) ,
                    round(sv.DeltaPhi,4) ,
                    sv.EtaMin,sv.EtaMax, 
                    len(sv.EtaBins)*len(sv.PhiBins) if sv.DeltaEta else len(sv.ZBins)*len(sv.PhiBins)  ,
                    samp.CollectionKey
                  ])
      print(t)


      print('Display all tracking samplings...')

      t = PrettyTable(["Name", "Plates", "z",'Zmin','Zmax', "Rmin", 
                       "Rmax", "abso","gap", "dz", "dphi", "N_bins", "Container"])

      # Add all volumes that came from a sampling detector and has a sensitive parameter
      #for samp in self.samplings:
      #  pv = samp.volume(); sv = samp.sensitive(); samp_vol_names.append(pv.Name)
      #  t.add_row( [pv.Name,
      #              Plates.tostring(pv.Plates),pv.ZSize,pv.ZMin,pv.ZMax,pv.RMin,pv.RMax,
      #              pv.AbsorberMaterial,pv.GapMaterial,
      #              round(sv.DeltaEta,4) ,
      #              round(sv.DeltaPhi,4) ,
      #              sv.EtaMin,sv.EtaMax, 
      #              len(sv.EtaBins)*len(sv.PhiBins) if sv.DeltaEta else len(sv.ZBins)*len(sv.PhiBins)  ,
      #              samp.CollectionKey
      #            ])
      print(t)

      print('Display all non-sensitive volumes...')

      t = PrettyTable(["Name", "Plates", "z",'Zmin','Zmax', "Rmin", 
                       "Rmax", "abso","gap"])

      # Add ither volumes that not came from a sampling detector (extra volumes only)
      for key, pv in self.__volumes.items():
        if key not in samp_vol_names:
          t.add_row([pv.Name, Plates.tostring(pv.Plates),pv.ZSize, pv.ZMin, pv.ZMax, 
                     pv.RMin, pv.RMax, pv.AbsorberMaterial, pv.GapMaterial]) 
      print(t)






#
# x,y,z to eta,phi coords. transformation functions
#

def xy_z_to_theta( r_xy, r_z ):
  if r_z!=0:
    theta = np.arctan2(  r_xy , r_z )
  else:
    theta = np.pi/2
  return theta

def theta_to_eta( theta ):
  try:
    eta = -np.log(np.tan( theta/2. ) )
  except:
    eta = -np.log(np.tan( (theta+np.pi)/2.) )
  return eta

def xy_z_to_eta( r_xy, z ):
  return theta_to_eta( xy_z_to_theta( r_xy, z ) )






#
# Geant4 physical volume dimensions
#
class PhysicalVolume(Logger):

    __allow_keys = [
                      "Name",
                      "Plates",
                      "AbsorberMaterial",
                      "GapMaterial",
                      "NofLayers",
                      "AbsorberThickness",
                      "GapThickness",
                      "RMin",
                      "RMax",
                      "ZSize",
                      "X",
                      "Y",
                      "Z",
                      "Visualization",
                      "Color",
                   ]

    # Constructor
    def __init__(self, **kw):

        Logger.__init__(self)
        for key, value in kw.items():
          if key in self.__allow_keys:
            setattr(self, key, value )
          else:
            MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

        self.ZMin = self.Z - self.ZSize / 2
        self.ZMax = self.Z + self.ZSize / 2
        self.Cuts = ProductionCuts()

    def name(self):
      return self.Name.replace('::','_')


# https://acode-browser1.usatlas.bnl.gov/lxr/source/athena/Simulation/G4Atlas/G4AtlasTools/python/G4PhysicsRegionConfig.py
class ProductionCuts(Logger):
  def __init__(self, GammaCut =  700*um, ElectronCut = 700*um, PositronCut = 700*um, PhotonCut = 700*um):
    self.GammaCut=GammaCut; self.ElectronCut=ElectronCut; self.PositronCut=PositronCut; self.PhotonCut=PhotonCut




#
# Sensitive volume
#
class SensitiveZPhiVolume(Logger):

    __allow_keys = [
                      "DeltaPhi",
                      "DeltaZ",
                   ]

    # Hold all granularity information from physical volume in eta x phi plane
    def __init__(self, pv, ZMin=None, ZMax=None, Segment=0, **kw):

        Logger.__init__(self)
        for key, value in kw.items():
          if key in self.__allow_keys:
            setattr(self, key, value )
          else:
            MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

        self.pv = pv
        self.PhiBins = np.round( np.arange( -np.pi, np.pi+self.DeltaPhi, step = self.DeltaPhi ), 4 ).tolist()
      

        # Create eta bins given the detector z position
        if pv.ZMin > 0: # entire in positive side
          z_min = pv.ZMin if ZMin is None else ZMin
          z_max = pv.ZMax if ZMax is None else ZMax
          self.ZBins = np.round( np.arange( z_min, z_max, step = self.DeltaZ ), 4 ).tolist()
          self.ZMin = self.ZBins[0]; self.ZMax = self.ZBins[-1]
        elif pv.ZMax < 0: # entire in negative side:
          #etamin=EtaMax;EtaMin=EtaMax;EtaMax=etamin 
          eta_min = round(xy_z_to_eta( pv.RMax, pv.ZMax ), 4) if not EtaMin else EtaMin
          eta_max = round(xy_z_to_eta( pv.RMin, pv.ZMin ), 4) if not EtaMax else EtaMax
          self.EtaBins = np.flip(np.round( np.arange( eta_min, eta_max , step = -1*self.DeltaEta ), 4 )).tolist()
          self.EtaMin = self.EtaBins[-1]; self.EtaMax = self.EtaBins[0]
        else: # The volume cut the zero axis
          eta_min = round(xy_z_to_eta( pv.RMax, 0 ), 4) if EtaMin is None else EtaMin
          eta_max = round(xy_z_to_eta( pv.RMin, pv.ZMax ), 4) if EtaMax is None else EtaMax
          right_eta_bins = np.round( np.arange( eta_min, eta_max  , step = self.DeltaEta ), 4 )
          eta_min = round(xy_z_to_eta( pv.RMax, 0 ), 4) if EtaMin is None else EtaMin
          eta_max = round(xy_z_to_eta( pv.RMin, pv.ZMin ), 4) if EtaMax is None else EtaMax
          left_eta_bins = np.flip(np.round( np.arange( eta_min, eta_max  , step = -1*self.DeltaEta ), 4 ))
          self.EtaBins = np.concatenate( (left_eta_bins, right_eta_bins[1::]) ).tolist()
          self.EtaMin = self.EtaBins[0];self.EtaMax = self.EtaBins[-1]

        if self.DeltaZ:
          self.EtaMin = round(xy_z_to_eta( pv.RMax, pv.ZMin ), 4)
          self.EtaMax = round(xy_z_to_eta( pv.RMin, pv.ZMax ), 4)
          z_center = pv.ZMin + 0.5*(pv.ZMax - pv.ZMin)
          self.ZBins = np.concatenate( [
                                          np.flip(np.round(np.arange( z_center, pv.ZMin, step=-self.DeltaZ),4)),
                                          np.round(np.arange( z_center, pv.ZMax, step=self.DeltaZ),4)
                                       ])


        self.Segment = Segment


    def volume(self):
      return self.pv






#
# Sensitive volume
#
class SensitiveCaloVolume(Logger):

    __allow_keys = [
                      "DeltaPhi",
                      "DeltaEta",
                   ]

    # Hold all granularity information from physical volume in eta x phi plane
    def __init__(self, pv, EtaMin=None, EtaMax=None, Segment=0, **kw):

        Logger.__init__(self)
        for key, value in kw.items():
          if key in self.__allow_keys:
            setattr(self, key, value )
          else:
            MSG_FATAL( self, "Property with name %s is not allow for %s object", key, self.__class__.__name__)

        self.pv = pv
        self.Segment = Segment
        self.PhiBins = np.round( np.arange( -np.pi, np.pi+self.DeltaPhi, step = self.DeltaPhi ), 4 ).tolist()

        # Create eta bins given the detector z position
        if pv.ZMin > 0: # entire in positive side
          eta_min = round(xy_z_to_eta( pv.RMax, pv.ZMin ), 4) if EtaMin is None else EtaMin 
          eta_max = round(xy_z_to_eta( pv.RMin, pv.ZMax ), 4) if EtaMax is None else EtaMax
          self.EtaBins = np.round( np.arange( eta_min, eta_max, step = self.DeltaEta ), 4 ).tolist()
          self.EtaMin = self.EtaBins[0]; self.EtaMax = self.EtaBins[-1]
        elif pv.ZMax < 0: # entire in negative side:
          #etamin=EtaMax;EtaMin=EtaMax;EtaMax=etamin 
          eta_min = round(xy_z_to_eta( pv.RMax, pv.ZMax ), 4) if not EtaMin else EtaMin
          eta_max = round(xy_z_to_eta( pv.RMin, pv.ZMin ), 4) if not EtaMax else EtaMax
          self.EtaBins = np.flip(np.round( np.arange( eta_min, eta_max , step = -1*self.DeltaEta ), 4 )).tolist()
          self.EtaMin = self.EtaBins[-1]; self.EtaMax = self.EtaBins[0]
        else: # The volume cut the zero axis
          eta_min = round(xy_z_to_eta( pv.RMax, 0 ), 4) if EtaMin is None else EtaMin
          eta_max = round(xy_z_to_eta( pv.RMin, pv.ZMax ), 4) if EtaMax is None else EtaMax
          right_eta_bins = np.round( np.arange( eta_min, eta_max  , step = self.DeltaEta ), 4 )
          eta_min = round(xy_z_to_eta( pv.RMax, 0 ), 4) if EtaMin is None else EtaMin
          eta_max = round(xy_z_to_eta( pv.RMin, pv.ZMin ), 4) if EtaMax is None else EtaMax
          left_eta_bins = np.flip(np.round( np.arange( eta_min, eta_max  , step = -1*self.DeltaEta ), 4 ))
          self.EtaBins = np.concatenate( (left_eta_bins, right_eta_bins[1::]) ).tolist()
          self.EtaMin = self.EtaBins[0]; self.EtaMax = self.EtaBins[-1]


    def volume(self):
      return self.pv




vis_begin = """
/vis/open OGL 600x600-0+0
/vis/viewer/set/autoRefresh false
/vis/verbose errors
/vis/drawVolume
/vis/viewer/set/viewpointVector 1 0 0
/vis/viewer/set/lightsVector 1 0 0
/vis/viewer/set/style wireframe
/vis/viewer/set/auxiliaryEdge true
/vis/viewer/set/lineSegmentsPerCircle 100
/vis/scene/add/trajectories smooth
/vis/modeling/trajectories/create/drawByCharge
/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true
/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 2
/vis/scene/endOfEventAction accumulate
/vis/geometry/set/visibility World 0 false
#/vis/geometry/set/visibility World 0 true
/vis/ogl/set/displayListLimit 10000000
"""
  
vis_command = """
/vis/geometry/set/colour {name} 0 {color}
/vis/geometry/set/colour {name}_Layer 0 {color}
/vis/geometry/set/colour {name}_Abso 0 {color}
/vis/geometry/set/colour {name}_Gap 0 {color}
/vis/geometry/set/visibility {name} 0 {visualization}
/vis/geometry/set/visibility {name}_Layer 0 {visualization}
/vis/geometry/set/visibility {name}_Abso 0 {visualization}
/vis/geometry/set/visibility {name}_Gap 0 {visualization}
"""

vis_end = """
/vis/viewer/set/autoRefresh true
/vis/verbose warnings
"""

def create_vis_mac(volumes, opath ):
  with open(opath, 'w') as f:
    f.write(vis_begin)
    for vol in volumes:
      f.write(vis_command.format(color=vol.Color, name=vol.name(), visualization='true' if vol.Visualization else 'false'))
    f.write(vis_end)



