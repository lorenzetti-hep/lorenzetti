
import os
import json
import logging
import numpy as np
from math import *
from pprint import pprint

# everything to mm
m = 1000
cm = 10
mm = 1

eps = 1e-5

def xy_z_to_theta( r_xy, z ):
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

class LateralSegmentation(object):
  def __init__( self 
              , rMin, rMax
              , delta_eta, delta_phi
              , z_min, z_max
              , eta_min = None, eta_max = None ):
    self.name = name
    self.delta_eta = delta_eta
    self.delta_phi = delta_phi
    self.layer_id = layer_id
    self.eta_min = eta_min
    self.eta_max = eta_max
    self.rMin = rMin
    self.rMax = rMax
    self.z_min = z_min
    self.z_max = z_max

  @property
  def eta_min(self):
    return self._eta_min

  @property
  def eta_max(self):
    return self._eta_max

  @property
  def eta_min(self):
    return self._eta_min

  @property
  def eta_max(self):
    return self._eta_max

  @zmin.setter
  def set_zmin(self, value):
    self._zmin = value
    if self.eta_max is None: 
      self._eta_max = xy_z_to_eta( self.rMax, self._zmin )

  @zmax.setter
  def set_zmax(self, value):
    self._zmax = value
    if self.eta_min is None: 
      self._eta_min = xy_z_to_eta( self.rMin, self._zmax )

  @eta_max.setter
  def set_eta_max(self, value):
    self._eta_max = value

  @eta_min.setter
  def set_eta_max(self, value):
    self._eta_max = value

  @property
  def phi_min(self):
    return -np.pi

  @property
  def phi_max(self):
    return np.pi

  def _compute_eta_cell_centers(self):
    # the max value in np.arange is protected since this array can pass the eta_max when we add the delta_eta
    positive_eta = np.arange( 
        self.eta_min + self.delta_eta/2, 
        self.eta_max - self.delta_eta/2 + eps, 
        step = self.delta_eta )
    return np.concatenate( [np.flip(-positive_eta), positive_eta] )

  def _compute_eta_cell_centers(self):
    positive_phi = np.arange( 
        0. + self.delta_phi/2,  
        np.pi - self.delta_phi/2 + eps,
        step = self.delta_phi )
    return np.concatenate ( [np.flip(-positive_phi), positive_phi] )

  def __str__(self):
    return 

  def __repr__(self):
    return self.__class__.__name__ + str(self)

  def dump(self,f):
    eta_centers = self._compute_eta_cell_centers()
    phi_centers = self._compute_eta_cell_centers()
    eta_nbins = len(eta_centers)
    phi_nbins = len(phi_centers)

    for  eta_idx , eta in enumerate(eta_centers):
      for phi_idx , phi in enumerate(phi_centers):
        cell_hash = 'layer%d_eta%d_phi%d' % (layer_id, eta_idx, phi_idx)
        s = "C {LAYER_ID} {ETA} {PHI} {DETA} {DPHI} {RMIN} {RMAX} {ZMIN} {ZMAX} {CELL_HASH} {ETA_IDX} {PHI_IDX}\n".format( 
          LAYER_ID  = layer_id,
          ETA       = round(eta,8),
          PHI       = round(phi,8),
          DETA      = round(self.delta_eta,8),
          DPHI      = round(self.delta_phi,8),
          RMIN      = round(self.rMin,8),
          RMAX      = round(self.rMax,8),
          ZMIN      = round(self.zMin,8),
          ZMAX      = round(self.zMax,8),
          CELL_HASH = cell_hash,
          ETA_IDX   = eta_idx,
          PHI_IDX   = phi_idx,
        )
        f.write(s)
      #
    #

class Layer(object):
  def __init__(self, name, layer_id, segmentations ):
    self.layer = name 
    self.layer_id = layer_id
    if not isinstance(segmentations,(list,tuple)):
      segmentations = [segmentations]
    self.segmentations = segmentations

  def dump(self):
    output = 'detector_sampling_%d.dat' % layer_id
    with open( output, 'w' ) as f:
      f.write("# layer_id eta phi delta_eta delta_phi rmin rmax zmin zmax\n")
      s = "L {LAYER_ID} {ETA_MIN} {ETA_MAX} {ETA_NBINS} {PHI_MIN} {PHI_MAX} {PHI_BINS} {RMIN} {RMAX} {ZMIN} {ZMAX}\n".format( 
        LAYER_ID  = layer_id,
        ETA_MIN   = round(self.eta_min,8),
        ETA_MAX   = round(self.eta_max,8),
        ETA_NBINS = eta_nbins,
        PHI_MIN   = round(self.phi_min,8),
        PHI_MAX   = round(self.phi_max,8),
        PHI_NBINS = phi_nbins,
        RMIN      = round(self.rMin,8),
        RMAX      = round(self.rMax,8),
        ZMIN      = round(self.zMin,8),
        ZMAX      = round(self.zMax,8),
      )
      f.write(s)
      for s in self.segmentations:
        s.dump(f)

  @property
  def eta_min(self):
    return np.min( map( lambda s: s.eta_min, self.segmentations ) )

  @property
  def eta_max(self):
    return np.max( map( lambda s: s.eta_max, self.segmentations ) )

  @property
  def zMin(self):
    return np.min( map( lambda s: s.zMax, self.segmentations ) )

  @property
  def zMax(self):
    return np.max( map( lambda s: s.zMax, self.segmentations ) )

class SingleSegmentationLayer(object):
  def __init__(self, **kw):
    segmentation = LateralSegmentation(**kw)
    self.layer = Layer(**kw, segmentations = segmentation)

  def dump(self):
    self.layer.dump()

barrel_ps_nominal_radius  = 146*cm
barrel_em_nominal_radius  = 150*cm
barrel_had_nominal_radius = 228.3*cm
extended_barrel_nominal_z = 2.83*m

barrel_em_calo_radius = np.array( [1.1*cm, 9.6*cm, 33*cm, 5.4*cm] )
barrel_had_calo_radius = np.array( [40*cm, 110*cm, 50*cm] )

end_cap_start_z = 3704.*mm;

emec_dead_material_z = 78.*mm;
emec_start_radius = 302.*mm
emec_end_radius = 2032.*mm
emec_start_z = endcap_start+emec_dead_material_z;

hec_start_z = 4262*mm;
hec_start_radius = np.array([372.*mm,475.*mm, 475.*mm])
hec_end_radius = np.array([2030.*mm]*3)

emec_calo_z = np.array( [96*mm, 330*mm, 54*mm] )
hec_calo_z =  np.array( [289*mm, 536*mm, 969.5*mm] )

psb = SingleSegmentationLayer( name = "PSB",
    layer_id = 0,
    rMin = barrel_ps_nominal_radius, 
    rMax = barrel_ps_nominal_radius + barrel_em_calo_radius[0].sum(),
    delta_eta = 0.025, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.4*m )

emb1 = SingleSegmentationLayer(
    name = "EMB1",
    layer_id = 1,
    rMin = barrel_em_nominal_radius, 
    rMax = barrel_em_nominal_radius + barrel_em_calo_radius[1:2].sum(),
    delta_eta = 0.00325, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.4*m )

emb2 = SingleSegmentationLayer(
    name = "EMB2",
    layer_id = 2,
    rMin = barrel_em_nominal_radius + barrel_em_calo_radius[1:2].sum(), 
    rMax = barrel_em_nominal_radius + barrel_em_calo_radius[1:3].sum(),
    delta_eta = 0.025, 
    delta_phi = pi/128,
    z_min = 0*m, 
    z_max = 3.4*m )

emb3 = SingleSegmentationLayer(
    name = "EMB3",
    layer_id = 3,
    rMin = barrel_em_nominal_radius + barrel_em_calo_radius[1:3].sum(), 
    rMax = barrel_em_nominal_radius + barrel_em_calo_radius[1:4].sum(),
    delta_eta = 0.050, 
    delta_phi = pi/128,
    z_min = 0*m, 
    z_max = 3.4*m )

tilecal1 = SingleSegmentationLayer(
    name = "TileCal1",
    layer_id = 4,
    rMin = barrel_had_nominal_radius, 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:2].sum(),
    delta_eta = 0.1, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.024*m )

tilecal2 = SingleSegmentationLayer(
    name = "TileCal2",
    layer_id = 5,
    rMin = barrel_had_nominal_radius + barrel_had_calo_radius[1:2].sum(), 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:3].sum(),
    delta_eta = 0.1, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.024*m )

tilecal3 = SingleSegmentationLayer(
    name = "TileCal3",
    layer_id = 6,
    rMin = barrel_had_nominal_radius + barrel_had_calo_radius[1:3].sum(), 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:4].sum(),
    delta_eta = 0.2, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.024*m )

tileext1 = SingleSegmentationLayer(
    name = "TileExt1",
    layer_id = 7,
    rMin = barrel_had_nominal_radius, 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:2].sum(),
    delta_eta = 0.1, 
    delta_phi = pi/32,
    z_min = endcap_start_z, 
    z_max = endcap_start_z + extended_barrel_nominal_z )

tileext2 = SingleSegmentationLayer(
    name = "TileExt2",
    layer_id = 8,
    rMin = barrel_had_nominal_radius + barrel_had_calo_radius[1:2].sum(), 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:3].sum(),
    delta_eta = 0.1, 
    delta_phi = pi/32,
    z_min = endcap_start_z, 
    z_max = endcap_start_z + extended_barrel_nominal_z )

tileext3 = SingleSegmentationLayer(
    name = "TileExt3",
    layer_id = 9,
    rMin = barrel_had_nominal_radius + barrel_had_calo_radius[1:3].sum(), 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:4].sum(),
    delta_eta = 0.2, 
    delta_phi = pi/32,
    z_min = endcap_start_z,
    z_max = endcap_start_z + extended_barrel_nominal_z )

emec1 = Layer(
    name = "EMEC1",
    layer_id = 10,
    segmentations = [
      LateralSegmentation( 
        rMin = emec_start_radius, 
        rMax = emec_end_radius,
        delta_eta = 0.00325, 
        delta_phi = pi/32,
        z_min = emec_start_z + emec_layer_z[0:0].sum(), 
        z_max = emec_start_z + emec_layer_z[0:1].sum(),
        eta_min = 1.375, 
        eta_max = 1.8 ),
      LateralSegmentation( 
        rMin = emec_start_radius, 
        rMax = emec_end_radius,
        delta_eta = 0.025/4, 
        delta_phi = pi/32,
        z_min = emec_start_z + emec_layer_z[0:0].sum(), 
        z_max = emec_start_z + emec_layer_z[0:1].sum(),
        eta_min = 1.8, 
        eta_max = 2.0 ),
      LateralSegmentation( 
        rMin = emec_start_radius, 
        rMax = emec_end_radius,
        delta_eta = 0.00650, 
        delta_phi = pi/32,
        z_min = emec_start_z + emec_layer_z[0:0].sum(), 
        z_max = emec_start_z + emec_layer_z[0:1].sum(),
        eta_min = 2.0, 
        eta_max = 2.37 ),
      LateralSegmentation( 
        rMin = emec_start_radius, 
        rMax = emec_end_radius,
        delta_eta = 0.1, 
        delta_phi = pi/32,
        z_min = emec_start_z + emec_layer_z[0:0].sum(), 
        z_max = emec_start_z + emec_layer_z[0:1].sum(),
        eta_min = 2.37, 
        eta_max = 3.2 ),
    ],
)

emec2 = Layer(
    name = "EMEC2",
    layer_id = 11,
    segmentations = [
      LateralSegmentation( 
        rMin = emec_start_radius, 
        rMax = emec_end_radius,
        delta_eta = 0.025,
        delta_phi = pi/128,
        z_min = emec_start_z + emec_layer_z[0:1].sum(), 
        z_max = emec_start_z + emec_layer_z[0:2].sum(),
        eta_min = 1.35, 
        eta_max = 2.5 ),
      LateralSegmentation( 
        rMin = emec_start_radius, 
        rMax = emec_end_radius,
        delta_eta = 0.1,
        delta_phi = pi/32,
        z_min = emec_start_z + emec_layer_z[0:1].sum(), 
        z_max = emec_start_z + emec_layer_z[0:2].sum(),
        eta_min = 2.5, 
        eta_max = 3.2 ),
    ],
)

emec3 = Layer(
    name = "EMEC3",
    layer_id = 12,
    segmentations = [
      LateralSegmentation( 
        rMin = emec_start_radius, 
        rMax = emec_end_radius,
        delta_eta = 0.050,
        delta_phi = pi/128,
        z_min = emec_start_z + emec_layer_z[0:2].sum(), 
        z_max = emec_start_z + emec_layer_z[0:3].sum(),
        eta_min = 1.35, 
        eta_max = 2.5 ),
      LateralSegmentation( 
        rMin = emec_start_radius, 
        rMax = emec_end_radius,
        delta_eta = 0.1,
        delta_phi = pi/32,
        z_min = emec_start_z + emec_layer_z[0:2].sum(), 
        z_max = emec_start_z + emec_layer_z[0:3].sum(),
        eta_min = 2.5, 
        eta_max = 3.2 ),
    ],
)

hec1 = Layer(
    name = "HEC1",
    layer_id = 13,
    segmentations = [
      LateralSegmentation( 
        rMin = hec_start_radius[0], 
        rMax = hec_end_radius[0],
        delta_eta = 0.1,
        delta_phi = pi/32,
        z_min = hec_start_z + hec_layer_z[0:0].sum(), 
        z_max = hec_start_z + hec_layer_z[0:1].sum(),
        eta_min = 1.5, 
        eta_max = 2.5 ),
      LateralSegmentation( 
        rMin = hec_start_radius[0], 
        rMax = hec_end_radius[0],
        delta_eta = 0.2,
        delta_phi = pi/16,
        z_min = hec_start_z + hec_layer_z[0:0].sum(), 
        z_max = hec_start_z + hec_layer_z[0:1].sum(),
        eta_min = 2.5, 
        eta_max = 3.2 ),
    ],
)

hec2 = Layer(
    name = "HEC2",
    layer_id = 14,
    segmentations = [
      LateralSegmentation( 
        rMin = hec_start_radius[1], 
        rMax = hec_end_radius[1],
        delta_eta = 0.1,
        delta_phi = pi/32,
        z_min = hec_start_z + hec_layer_z[0:1].sum(), 
        z_max = hec_start_z + hec_layer_z[0:2].sum(),
        eta_min = 1.5, 
        eta_max = 2.5 ),
      LateralSegmentation( 
        rMin = hec_start_radius[1], 
        rMax = hec_end_radius[1],
        delta_eta = 0.2,
        delta_phi = pi/16,
        z_min = hec_start_z + hec_layer_z[0:1].sum(), 
        z_max = hec_start_z + hec_layer_z[0:2].sum(),
        eta_min = 2.5, 
        eta_max = 3.2 ),
    ],
)

hec3 = Layer(
    name = "HEC3",
    layer_id = 14,
    segmentations = [
      LateralSegmentation( 
        rMin = hec_start_radius[2], 
        rMax = hec_end_radius[2],
        delta_eta = 0.1,
        delta_phi = pi/32,
        z_min = hec_start_z + hec_layer_z[0:2].sum(), 
        z_max = hec_start_z + hec_layer_z[0:3].sum(),
        eta_min = 1.5, 
        eta_max = 2.5 ),
      LateralSegmentation( 
        rMin = hec_start_radius[2], 
        rMax = hec_end_radius[2],
        delta_eta = 0.2,
        delta_phi = pi/16,
        z_min = hec_start_z + hec_layer_z[0:2].sum(), 
        z_max = hec_start_z + hec_layer_z[0:3].sum(),
        eta_min = 2.5, 
        eta_max = 3.2 ),
    ],
)

em_barrel = [psb, emb1, emb2, emb3]
had_barrel = [ tilecal1, tilecal2, tilecal3
             , tileext1, tileext2, tileext3 ]
emec = [emec1, emec2, emec3]
hec = [hec1, hec2, hec3]

all_calo = [em_barrel, had_barrel, emec, hec]

for layer in all_calo:
  layer.dump()
