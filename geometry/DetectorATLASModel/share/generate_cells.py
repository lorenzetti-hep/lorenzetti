
import os
import json
import logging
import numpy as np
from math import *


#
# Cell
#
class Cell:

  #
  # Constructor
  #
  def __init__(self, eta, phi, deta, dphi, rmin, rmax, hash):
    self.__eta=eta 
    self.__phi=phi
    self.__deta=deta
    self.__dphi=dphi
    self.__rmin=rmin
    self.__rmax=rmax
    self.__hash = hash

  #
  # Get the hash id
  #
  def hash( self ):
    #return int(self.__layerid*1e7 + self.__sectionid*1e6 + self.__cellid)
    return self.__hash

  def eta(self):
    return self.__eta

  def phi(self):
    return self.__phi

  def deltaEta(self):
    return self.__deta

  def deltaPhi(self):
    return self.__dphi

  def rmin(self):
    return self.__rmin

  def rmax(self):
    return self.__rmax


#
# Helper container
#
class CellContainer:

  #
  # Constructor
  #
  def __init__( self, name, layer_id, section_id, etamin, etamax, etabins, phimin, phimax, phibins, rmin, rmax ):

    self.__name = name
    self.__layer_id = layer_id
    self.__section_id = section_id
    self.__etamin = etamin
    self.__etamax = etamax
    self.__etabins = etabins
    self.__phimin = phimin
    self.__phimax = phimax
    self.__phibins = phibins
    self.__rmin = rmin
    self.__rmax = rmax
    self.__collection = []

  def name(self):
    return self.__name

  #
  # Add cell
  #
  def push_back( self, cell ):
    self.__collection.append( cell )


  def id(self):
    return self.__layer_id

  #
  # Dump configuration
  #
  def dump(self, output):

    file = open( output, 'w' )
    file.write("# layer_id eta phi delta_eta delta_phi rmin rmax\n")


    s = "L {LAYER_ID} {ETA_MIN} {ETA_MAX} {ETA_BINS} {PHI_MIN} {PHI_MAX} {PHI_BINS} {RMIN} {RMAX}\n".format( 
                                                              LAYER_ID = self.__layer_id,
                                                              ETA_MIN = round(self.__etamin,8),
                                                              ETA_MAX = round(self.__etamax,8),
                                                              ETA_BINS = self.__etabins,
                                                              PHI_MIN = round(self.__phimin,8),
                                                              PHI_MAX = round(self.__phimax,8),
                                                              PHI_BINS = self.__phibins,
                                                              RMIN = round(self.__rmin,8),
                                                              RMAX = round(self.__rmax,8),
                                                              )
    file.write(s)
        

    for cell in self.__collection:
      s = "C {LAYER_ID} {ETA} {PHI} {DETA} {DPHI} {RMIN} {RMAX} {CELL_HASH}\n".format( 
                                                              LAYER_ID = self.__layer_id,
                                                              ETA=round(cell.eta(),8), 
                                                              PHI=round(cell.phi(),8),
                                                              DETA = round(cell.deltaEta(),8),
                                                              DPHI = round(cell.deltaPhi(),8),
                                                              RMIN = round(cell.rmin(),8),
                                                              RMAX = round(cell.rmax(),8),
                                                              CELL_HASH = cell.hash(),
                                                              )
      file.write(s)

    file.close()
    







#
# Cell generator
#
class CellGenerator(object):


    def __init__(self, config_dict ):
      self.__collections = config_dict
      self.containers = []
      self.compile()


    #
    # Compile all cells
    #
    def compile( self ):


      for key in self.__collections.keys() :

        collection = self.__collections[key]
        l_id   = collection['layer_id']
        r_min  = collection['min_dist_to_ip']
        r_max  = collection['max_dist_to_ip']
        
        # Because is a circle this is the radius in the transverse plane
        r_xy  = np.sqrt((r_max**2 + r_max**2))
        
        eta_step_size  = collection['delta_eta']/2.
        phi_step_size  = collection['delta_phi']/2.

        eta_limits = [0, 0]
        
        if 'etamin' in collection.keys():
          eta_limits[0] = collection['etamin']
        else:
          z_min  = collection['z_min']
          etamin ,thetamin  = self.calculate_eta(r_xy, z_min)
          eta_limits[0] = etamin

        if 'etamax' in collection.keys():
          eta_limits[1] = collection['etamin']
        else:
          z_max  = collection['z_max']
          etamax ,thetamax  = self.calculate_eta(r_xy, z_max)
          eta_limits[1] = etamax

       
        # Calorimter sections
        #
        #      phi
        #  D    |   A
        #       |
        # ------+------ eta
        #       |
        #  C    |   B
        # 

        print(eta_limits)
    
        vec_eta = self.calculate_cell_centers( eta_limits=eta_limits, is_eta=True, deta=eta_step_size )
        vec_phi = self.calculate_cell_centers( dphi=phi_step_size )
        self.containers.append( self.build_cells( key, vec_eta, vec_phi, eta_step_size, phi_step_size, r_min, r_max, l_id, 0) )

      




    #
    # This method will calculate the maximum of eta for the layer.
    # r_z: the size of the detector.
    #
    def calculate_eta(self, r_xy,  r_z):
       
      if r_z!=0:
        theta   = np.arctan2(  r_xy , r_z )
        try:
          eta_max = -1*np.log(np.tan( theta/2. ) )
        except:
          eta_max = -1*np.log(np.tan( (theta+np.pi)/2.) ) 
      else:
        eta_max = 0
        theta = np.pi/2
      return eta_max, theta
 

    #
    # Calculate all cell centers
    #
    def calculate_cell_centers(self, eta_limits=None, is_eta=False, deta=None, dphi=None):

      if is_eta:
          # the max value in np.arange is protected since this array can pass the eta_max when we add the delta_eta
          positive_eta = np.arange( eta_limits[0] + deta, eta_limits[1]-deta, step=2*deta)
          negative_eta = positive_eta * -1
          eta = np.concatenate ( (np.flip(negative_eta), positive_eta ) )
          #return positive_eta, negative_eta 
          return eta
      else:
          positive_phi = np.arange( dphi,  np.pi-dphi, step=2*dphi)
          negative_phi = positive_phi * -1
          phi = np.concatenate ( (np.flip(negative_phi), positive_phi) )
          #return positive_phi, negative_phi
          return phi

      

    def build_cells( self, name, vec_eta, vec_phi , deta, dphi, rmin, rmax, l_id, s_id ):

      container = CellContainer( name, l_id, s_id ,min(vec_eta)-deta, max(vec_eta)+deta, len(vec_eta), min(vec_phi)-dphi, max(vec_phi)+dphi, len(vec_phi), rmin, rmax )
      c_id = 0
      for eta_id, eta in enumerate(vec_eta):
        for phi_id, phi in enumerate(vec_phi):
          #hash = 'layer%d_eta%d_phi%d'%( l_id,eta_id,phi_id ) 
          hash =  int(l_id*1e7 + c_id)
          container.push_back( Cell( eta, phi, deta, dphi, rmin, rmax, hash )  )
          c_id += 1
      return container



    def dump(self):

      for container in self.containers:
        output = 'detector_sampling_%d.dat' % container.id()
        container.dump(output)

     






###########################################################################################

# everything to mm
m = 1000
cm = 10

# Detector ROI dimesions
#                (0,0,0)
#                   |
#       |--------- EM ----------|----------- HAD ----------|
#         15cm     30cm     3cm    40cm       40cm    20cm
#  +    +--------------------------------------------------+
#  |    |      |           |    |         |         |      |   
# 48cm  |      |           |    |         |         |      |
#  |    |      |           |    |         |         |      |
#  +    +--------------------------------------------------+


ps_nominal_radius = 146*cm
em_nominal_radius  = 150*cm
had_nominal_radius  = 228.3*cm

# LAr 28.5 X0, 0.71X0 per cm
#calo_radius = np.array( [10*cm, 33*cm, 5*cm, 40*cm, 40*cm, 20*cm] )
#calo_radius = np.array( [4.8*cm, 37.8*cm, 5.4*cm, 40*cm, 40*cm, 20*cm] )
#calo_radius = np.array( [3*cm, 39.6*cm, 5.4*cm, 40*cm, 40*cm, 20*cm] )
em_calo_radius = np.array( [1.1*cm, 9.6*cm, 33*cm, 5.4*cm] )
had_calo_radius = np.array( [40*cm, 110*cm, 50*cm] )



# Only right size, the configuration will be flipped in the end to fill the left side
config = {
    # Electromagnetic Layers
    'EM1'  : {
        'min_dist_to_ip' : em_nominal_radius,
        'max_dist_to_ip' : em_nominal_radius + em_calo_radius[1].sum(),
        'delta_eta'      : 0.00325,
        'delta_phi'      : pi/32,
        'layer_id'       : 1,
        'z_min'          : 0,
        'z_max'          : 3.4*m,
    },
    'EM2'  : {
        'min_dist_to_ip' : em_nominal_radius + em_calo_radius[1:2].sum(),
        'max_dist_to_ip' : em_nominal_radius + em_calo_radius[1:3].sum(),
        'delta_eta'      : 0.025,
        'delta_phi'      : pi/128,
        'layer_id'       : 2,
        'z_min'          : 0,
        'z_max'          : 3.4*m,
    },
    'EM3'  : {
        'min_dist_to_ip' : em_nominal_radius + em_calo_radius[1:3].sum(),
        'max_dist_to_ip' : em_nominal_radius + em_calo_radius[1:4].sum(),
        'delta_eta'      : 0.050,
        'delta_phi'      : pi/128,
        'layer_id'       : 3,
        'z_min'          : 0,
        'z_max'          : 3.4*m,
    },
    # Hadronic Layers
    'HAD1'  : {
        'min_dist_to_ip' : had_nominal_radius + had_calo_radius[:0].sum(),
        'max_dist_to_ip' : had_nominal_radius + had_calo_radius[:1].sum(),
        'delta_eta'      : 0.1,
        'delta_phi'      : pi/32,
        'layer_id'       : 4,
        'z_min'          : 0,
        'z_max'          : 3.4*m,
    },
    'HAD2'  : {
        'min_dist_to_ip' : had_nominal_radius + had_calo_radius[:1].sum(),
        'max_dist_to_ip' : had_nominal_radius + had_calo_radius[:2].sum(),
        'delta_eta'      : 0.1,
        'delta_phi'      : pi/32,
        'layer_id'       : 5,
        'z_min'          : 0,
        'z_max'          : 3.4*m,
    },
    'HAD3'  : {
        'min_dist_to_ip' : had_nominal_radius + had_calo_radius[:2].sum(),
        'max_dist_to_ip' : had_nominal_radius + had_calo_radius[:3].sum(),
        'delta_eta'      : 0.2,
        'delta_phi'      : pi/32,
        'layer_id'       : 6,
        'z_min'          : 0,
        'z_max'          : 3.4*m,
    },
}



from pprint import pprint
pprint(config)

gen = CellGenerator( config )
gen.dump()


