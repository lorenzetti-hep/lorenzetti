

import os
import json
import logging
import numpy as np
from math import *
from pprint import pprint

from GaugiKernel import Logger
from GaugiKernel.macros import *

from CaloCell.CaloDefs import CaloSampling, Detector

# everything to mm
m = 1000
cm = 10
mm = 1
pi = np.pi

eps = 1e-4

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
# Lateral segmentation
#
class LateralSegmentation(object):
  
  def __init__( self, rMin, rMax
              , delta_eta, delta_phi
              , z_min, z_max
              , eta_min = None, eta_max = None, sampling_id=None,
              detector_id=None ):
    self.delta_eta = delta_eta
    self.delta_phi = delta_phi
    self.sampling_id = sampling_id
    self.detector_id = detector_id
    self.eta_min = eta_min 
    self.eta_max = eta_max
    self.rMin = rMin
    self.rMax = rMax
    self.zMin = z_min
    self.zMax = z_max


  #
  # eta min
  #
  @property
  def eta_min(self):
    return self._eta_min

  @eta_min.setter
  def eta_min(self, value):
    self._eta_min = value

  #
  # eta max
  #
  @property
  def eta_max(self):
    return self._eta_max

  @eta_max.setter
  def eta_max(self, value):
    self._eta_max = value

  #
  # z min
  #
  @property
  def zMin(self):
    return self._zMin

  @zMin.setter
  def zMin(self, value):
    self._zMin = value
    if self.eta_min is None: 
      self._eta_min = round(xy_z_to_eta( self.rMax, self._zMin ), 8)

  #
  # z max
  #
  @property
  def zMax(self):
    return self._zMax

  @zMax.setter
  def zMax(self, value):
    self._zMax = value
    if self.eta_max is None: 
      self._eta_max = round(xy_z_to_eta( self.rMin, self._zMax ),8)
  
  #
  # phi min/max
  #
  @property
  def phi_min(self):
    return -np.pi

  @property
  def phi_max(self):
    return np.pi

  #
  # Get the eta bins for all segmentations
  #
  @property
  def eta_bins(self):
    eta_bins = np.arange( self.eta_min, self.eta_max  + eps, step = self.delta_eta ) 
    return np.unique( np.round( np.concatenate ( [np.flip(-eta_bins), eta_bins] ), 4 ) )
 
  #
  # Get the phi bins for all segmentations
  #
  @property
  def phi_bins(self):
    return np.round( np.arange( self.phi_min, self.phi_max  + eps, step = self.delta_phi ), 4 ) 


  #
  # Compute all eta cell centers
  #
  def compute_eta_cell_centers(self):
    # the max value in np.arange is protected since this array can pass the eta_max when we add the delta_eta
    positive_eta = np.arange( 
        self.eta_min + self.delta_eta/2, 
        self.eta_max - self.delta_eta/2 + eps, 
        step = self.delta_eta )

    return np.concatenate( [np.flip(-positive_eta), positive_eta] )

  #
  # Compute all phi cell centers
  #
  def compute_phi_cell_centers(self):
    positive_phi = np.arange( 
        0. + self.delta_phi/2,  
        np.pi - self.delta_phi/2 + eps,
        step = self.delta_phi )
    return np.concatenate ( [np.flip(-positive_phi), positive_phi] )


  def __str__(self):
    return 

  def __repr__(self):
    return self.__class__.__name__ + str(self)


  #
  # Dump
  #
  def to_raw(self):
    
    eta_max = max(abs(self.eta_bins))
    eta_min = min(abs(self.eta_bins))
    return {
              'EtaBins'  : self.eta_bins.tolist(),
              'PhiBins'  : self.phi_bins.tolist(),
              'Detector' : self.detector_id,
              'Sampling' : self.sampling_id,
              'EtaMin'   : round(eta_min, 4),
              'EtaMax'   : round(eta_max, 4),
              'RMin'     : round(self.rMin,4),
              'RMax'     : round(self.rMax,4),
              'ZMin'     : round(self.zMin,4),
              'ZMax'     : round(self.zMax,4),
    }


          



class Layer(object):

  def __init__(self, name, sampling_id, detector_id, segmentations ):
    self.layer = name 
    self.sampling_id = sampling_id
    self.detector_id = detector_id
    if not isinstance(segmentations,(list,tuple)):
      segmentations = [segmentations]
    # set the layer id by hand
    for seg in segmentations:
      seg.sampling_id = sampling_id
      seg.detector_id = detector_id
    self.segmentations = segmentations


  #
  # Dump layer
  #
  def to_raw(self):

    raw = { self.layer : [] }
    for idx, seg in enumerate(self.segmentations):
      print(self.layer)
      print(seg.sampling_id)
      configs = seg.to_raw()
      configs['Segment'] = idx
      raw[self.layer].append(configs)

    return raw







class SingleSegmentationLayer(object):
  def __init__(self, name, sampling_id, detector_id, **kw):
    segmentation = LateralSegmentation(sampling_id = sampling_id, detector_id=detector_id, **kw)
    self.layer = Layer( name, sampling_id, detector_id, segmentations = segmentation)

  def to_raw(self):
    return self.layer.to_raw()




#
# Build the ATLAS detector here
#




barrel_ps_nominal_radius  = 146*cm
barrel_em_nominal_radius  = 150*cm
barrel_had_nominal_radius = 228.3*cm
extended_barrel_nominal_z = 2.83*m

barrel_em_calo_radius = np.array( [1.1*cm, 9.6*cm, 33*cm, 5.4*cm] )
barrel_had_calo_radius = np.array( [40*cm, 110*cm, 50*cm] )

endcap_start_z = 3704.*mm

ps_endcap_z = 5.*mm
emec_dead_material_z = 78.*mm - ps_endcap_z
ps_endcap_start = endcap_start_z+emec_dead_material_z

ps_endcap_start_radius = 1232.*mm
ps_endcap_end_radius = 1700.*mm

emec_start_radius = 302.*mm
emec_end_radius = 2032.*mm

emec_start_z = endcap_start_z+emec_dead_material_z+ps_endcap_z

hec_start_z = 4262*mm
hec_start_radius = np.array([372.*mm,475.*mm, 475.*mm])
hec_end_radius = np.array([2030.*mm]*3)

emec_layer_z = np.array( [96*mm, 330*mm, 54*mm] )
hec_layer_z =  np.array( [289*mm, 536*mm, 969.5*mm] )




psb = SingleSegmentationLayer( name = "PSB",
    sampling_id = CaloSampling.PSB,
    detector_id = Detector.TTEM,
    rMin = barrel_ps_nominal_radius, 
    rMax = barrel_ps_nominal_radius + barrel_em_calo_radius[0].sum(),
    delta_eta = 0.025, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.4*m )

emb1 = SingleSegmentationLayer(
    name = "EMB1",
    sampling_id = CaloSampling.EMB1,
    detector_id = Detector.TTEM,
    rMin = barrel_em_nominal_radius, 
    rMax = barrel_em_nominal_radius + barrel_em_calo_radius[1:2].sum(),
    delta_eta = 0.00325, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.4*m )

emb2 = SingleSegmentationLayer(
    name = "EMB2",
    sampling_id = CaloSampling.EMB2,
    detector_id = Detector.TTEM,
    rMin = barrel_em_nominal_radius + barrel_em_calo_radius[1:2].sum(), 
    rMax = barrel_em_nominal_radius + barrel_em_calo_radius[1:3].sum(),
    delta_eta = 0.025, 
    delta_phi = pi/128,
    z_min = 0*m, 
    z_max = 3.4*m )

emb3 = SingleSegmentationLayer(
    name = "EMB3",
    sampling_id = CaloSampling.EMB3,
    detector_id = Detector.TTEM,
    rMin = barrel_em_nominal_radius + barrel_em_calo_radius[1:3].sum(), 
    rMax = barrel_em_nominal_radius + barrel_em_calo_radius[1:4].sum(),
    delta_eta = 0.050, 
    delta_phi = pi/128,
    z_min = 0*m, 
    z_max = 3.4*m )

tilecal1 = SingleSegmentationLayer(
    name = "TileCal1",
    sampling_id = CaloSampling.TileCal1,
    detector_id = Detector.TILE,
    rMin = barrel_had_nominal_radius, 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:2].sum(),
    delta_eta = 0.1, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.024*m )

tilecal2 = SingleSegmentationLayer(
    name = "TileCal2",
    sampling_id = CaloSampling.TileCal2,
    detector_id = Detector.TILE,
    rMin = barrel_had_nominal_radius + barrel_had_calo_radius[1:2].sum(), 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:3].sum(),
    delta_eta = 0.1, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.024*m )

tilecal3 = SingleSegmentationLayer(
    name = "TileCal3",
    sampling_id = CaloSampling.TileCal3,
    detector_id = Detector.TILE,
    rMin = barrel_had_nominal_radius + barrel_had_calo_radius[1:3].sum(), 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:4].sum(),
    delta_eta = 0.2, 
    delta_phi = pi/32,
    z_min = 0*m, 
    z_max = 3.024*m )

tileext1 = SingleSegmentationLayer(
    name = "TileExt1",
    sampling_id = CaloSampling.TileExt1,
    detector_id = Detector.TILE,
    rMin = barrel_had_nominal_radius, 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:2].sum(),
    delta_eta = 0.1, 
    delta_phi = pi/32,
    z_min = endcap_start_z, 
    z_max = endcap_start_z + extended_barrel_nominal_z )

tileext2 = SingleSegmentationLayer(
    name = "TileExt2",
    sampling_id = CaloSampling.TileExt2,
    detector_id = Detector.TILE,
    rMin = barrel_had_nominal_radius + barrel_had_calo_radius[1:2].sum(), 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:3].sum(),
    delta_eta = 0.1, 
    delta_phi = pi/32,
    z_min = endcap_start_z, 
    z_max = endcap_start_z + extended_barrel_nominal_z )

tileext3 = SingleSegmentationLayer(
    name = "TileExt3",
    sampling_id = CaloSampling.TileExt3,
    detector_id = Detector.TILE,
    rMin = barrel_had_nominal_radius + barrel_had_calo_radius[1:3].sum(), 
    rMax = barrel_had_nominal_radius + barrel_had_calo_radius[1:4].sum(),
    delta_eta = 0.2, 
    delta_phi = pi/32,
    z_min = endcap_start_z,
    z_max = endcap_start_z + extended_barrel_nominal_z )

pse = SingleSegmentationLayer(
    name = "PSE",
    sampling_id = CaloSampling.PSE,
    detector_id = Detector.TTEM,
    rMin = ps_endcap_start_radius, 
    rMax = ps_endcap_end_radius,
    delta_eta = 0.025,
    delta_phi = pi/32,
    z_min = ps_endcap_start, 
    z_max = ps_endcap_start+ps_endcap_z,
    eta_min = 1.5, 
    eta_max = 1.8, 
)

emec1 = Layer(
    name = "EMEC1",
    sampling_id = CaloSampling.EMEC1,
    detector_id = Detector.TTEM,
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
    sampling_id = CaloSampling.EMEC2,
    detector_id = Detector.TTEM,
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
    sampling_id = CaloSampling.EMEC3,
    detector_id = Detector.TTEM,
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
    sampling_id = CaloSampling.HEC1,
    detector_id = Detector.TTHEC,
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
    sampling_id = CaloSampling.HEC2,
    detector_id = Detector.TTHEC,
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
    sampling_id = CaloSampling.HEC3,
    detector_id = Detector.TTHEC,
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

em_barrel = [psb, pse, emb1, emb2, emb3]
had_barrel = [ tilecal1, tilecal2, tilecal3, tileext1, tileext2, tileext3 ]
emec = [emec1, emec2, emec3]
hec = [hec1, hec2, hec3]

all_calo = [em_barrel, had_barrel, emec, hec]

raw = {}
for calo in all_calo:
  for layer in calo:
    raw.update(layer.to_raw())


import json
with open('atlas_granularity.json', 'w') as handle:
    json.dump(raw, handle,indent=4)

