
import os
import json
import logging
import numpy as np
from math import *

RES=3

class CellGenerator(object):
    '''
    A class to create the Lorenzetti cell grid in term os (phi, eta) using any granularity.
    Parameters:
    layer_dict: a dictionary contains the following keys:
        * distance_to_ip : the maximum distante to the interaction point (0,0,0) of the detector.
        * nominal_size   : half the size of the detector.
        * delta_eta      : the cell size in term of eta. The eta center is delta_eta divided by 2.
        * delta_eta      : same of delta_eta but using phi.
    '''
    def __init__(self, layer_dict ):
        print('init lorenzetti_cells_grid_builder... ')
        self.layer_dict = layer_dict


    def get_layer_info(self, layer):#, distance_to_ip, detector_size, delta_eta, delta_phi):
        
        self.detector_size  = self.layer_dict['nominal_size']
        self.eta_step_size  = self.layer_dict[layer]['delta_eta']/2.
        self.phi_step_size  = self.layer_dict[layer]['delta_phi']/2.
        # Because is a circle this is the radius in the transverse plane
        self.r_xy           = np.sqrt((self.layer_dict[layer]['max_dist_to_ip']**2 + 
                                       self.layer_dict[layer]['max_dist_to_ip']**2))
        self.eta_max        = self.calculate_eta_max(self.detector_size)-self.eta_step_size
        # Because eta is symmetric
        self.eta_min        = -1*self.eta_max


    def get_theta(self):
        return self.theta

    def get_eta_bounds(self):
        return [self.eta_min, self.eta_max]
    
    def get_phi_bounds(self):
        return [-1.*np.pi, np.pi]

    def get_r_xy(self):
        return self.r_xy



    def calculate_eta_max(self, r_z):
        '''
        This method will calculate the maximum of eta for the layer.
        Parameters:
        r_z: the size of the detector.
        '''
        if r_z!=0:
          self.theta   = np.arctan2(  self.r_xy , r_z )
          try:
            eta_max      = -1*np.log(np.tan( self.theta/2. ) )
          except:
            eta_max      = -1*np.log(np.tan( (self.theta+np.pi)/2.) ) 
        else:
          eta_max = 0
          self.theta = np.pi/2
        print("Eta max for this layer is: ",eta_max)
        return eta_max
    


    def calculate_cell_centers(self, eta_lims=None, is_eta=True):
        '''
        This method will calculate the cell centers based on the delta_eta or delta_phi.
        Parameters:
        is_eta: this parameter is used to calculate the center in eta. 
                if False the center will calculate in terms of phi.
        '''
        if eta_lims is None:
          eta_lims = (0,self.eta_max)
        else:
          print( eta_lims )
          print( ( eta_lims[0] + self.eta_step_size, eta_lims[1], 2*self.eta_step_size ) )
        if is_eta:
            # the max value in np.arange is protected since this array can pass the eta_max when we add the delta_eta
            positive_eta = np.arange( eta_lims[0] + self.eta_step_size, eta_lims[1], step=2*self.eta_step_size)
            negative_eta = positive_eta * -1
            eta = np.concatenate ( (np.flip(negative_eta), positive_eta ) )
            return eta 
        else:
            positive_phi = np.arange( self.phi_step_size,  np.pi-self.phi_step_size, step=2*self.phi_step_size)
            negative_phi = positive_phi * -1
            phi = np.concatenate ( (np.flip(negative_phi), positive_phi) )
            return phi




    def get_all_cell_centers(self):
        for layer in self.layer_dict.keys():
            if layer == 'nominal_size':
                continue
            eta_lims = None
            for k,v in self.layer_dict[layer].items():
              if k == 'eta_max':
                if eta_lims is None: eta_lims = [None,None]
                eta_lims[1] = v
              elif k == 'eta_min':
                if eta_lims is None: eta_lims = [None,None]
                eta_lims[0] = v
            temp_dict = self.layer_dict[layer]
            print( ('Creating the cell center for %s using:' %(layer) + 
                   'max_radius: %1.8f \n delta_eta: %1.8f \n and delta_phi: %1.8f')
                   %(temp_dict['max_dist_to_ip'],
                     temp_dict['delta_eta'],
                     temp_dict['delta_phi']))
            
            self.get_layer_info(layer)

            # Get the cell centers and the boundaries
            self.layer_dict[layer]['eta_centers'] = self.calculate_cell_centers(eta_lims=eta_lims,is_eta=True)
            if eta_lims is not None: print(self.layer_dict[layer]['eta_centers'])
            self.layer_dict[layer]['eta_bounds']  = self.get_eta_bounds() # this is wrong for the _extended layers
            self.layer_dict[layer]['phi_centers'] = self.calculate_cell_centers(is_eta=False)
            self.layer_dict[layer]['phi_bounds']  = self.get_phi_bounds()
            logging.info(60*'-')
    

            from pprint import pprint
            #pprint (self.layer_dict[layer])

    def dump(self):



        from pprint import pprint

        for layer in self.layer_dict.keys():
            if layer == 'nominal_size':
              continue

            
           
            layer_id = self.layer_dict[layer]['layer_id']

            eta_bins = len(self.layer_dict[layer]['eta_centers'])
            delta_eta = self.layer_dict[layer]['delta_eta']/2.
            eta = self.layer_dict[layer]['eta_centers']
            eta_min = eta[0] - delta_eta
            eta_max = eta[-1] + delta_eta

            phi_bins = len(self.layer_dict[layer]['phi_centers'])
            delta_phi = self.layer_dict[layer]['delta_phi']/2.
            phi = self.layer_dict[layer]['phi_centers']
            phi_min = phi[0] - delta_phi
            phi_max = phi[-1] + delta_phi

            output = 'detector_sampling_%d.dat' % layer_id

            file = open( output, 'w' )
            file.write("# layer_id eta phi delta_eta delta_phi rmin rmax\n")
 
            s = "L {LAYER_ID} {ETA_MIN} {ETA_MAX} {ETA_BINS} {PHI_MIN} {PHI_MAX} {PHI_BINS} {RMIN} {RMAX}\n".format( LAYER_ID = layer_id,
                                                                      ETA_MIN = round(eta_min,8),
                                                                      ETA_MAX = round(eta_max,8),
                                                                      ETA_BINS = eta_bins,
                                                                      PHI_MIN = round(phi_min,8),
                                                                      PHI_MAX = round(phi_max,8),
                                                                      PHI_BINS = phi_bins,
                                                                      RMIN = round(self.layer_dict[layer]['min_dist_to_ip'],8),
                                                                      RMAX = round(self.layer_dict[layer]['max_dist_to_ip'],8),
                                                                      )
            file.write(s)
            

            for  eta_i , eta in enumerate(self.layer_dict[layer]['eta_centers']):
              for phi_i , phi in enumerate(self.layer_dict[layer]['phi_centers']):
    


                cell_hash = 'layer%d_eta%d_phi%d' % (layer_id, eta_i, phi_i)
                s = "C {LAYER_ID} {ETA} {PHI} {DETA} {DPHI} {RMIN} {RMAX} {CELL_HASH} {ETA_IDX} {PHI_IDX}\n".format( 
                                                                        LAYER_ID = layer_id,
                                                                        ETA=round(eta,8), 
                                                                        PHI=round(phi,8),
                                                                        DETA = round(self.layer_dict[layer]['delta_eta'],8),
                                                                        DPHI = round(self.layer_dict[layer]['delta_phi'],8),
                                                                        RMIN = round(self.layer_dict[layer]['min_dist_to_ip'],8),
                                                                        RMAX = round(self.layer_dict[layer]['max_dist_to_ip'],8),
                                                                        CELL_HASH = cell_hash,
                                                                        ETA_IDX=eta_i,
                                                                        PHI_IDX=phi_i,
                                                                        )
                file.write(s)

            file.close()
    


def transform_to_ATLAS_coord(vectors):
    '''
    This function will transform a cartesian coordinates (x, y, z)
    into a ATLAS (r, phi, eta) coordinates
    '''

    phi       = np.arctan2( vectors[:,1], vectors[:, 0]) # y/x
    theta     = np.arctan2( vectors[:,0], vectors[:, 2]) # x/z

    # check if tan_theta is negative if True use the sumplement of theta
    tan_theta = np.tan(theta/2.)
    negatives = np.where(tan_theta < 0.)
    if negatives[0].size != 0:
        tan_theta[negatives[0]] = np.tan((np.pi + theta[negatives[0]])/2.)

    eta       = -1.*np.log(tan_theta)
    # momentum
    r         = np.sqrt((vectors[:, 0]**2 + vectors[:, 1]**2 + vectors[:, 2]**2))
    return np.vstack((r, phi, eta)).T






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


config = {
    'nominal_size'       : 3.4*m,
    #'PS'  : {
    #    'min_dist_to_ip' : ps_nominal_radius*cm,
    #    'max_dist_to_ip' : ps_nominal_radius*cm + em_calo_radius[0].sum(),
    #    'delta_eta'      : 0.025,
    #    'delta_phi'      : pi/32,
    #    'layer_id'       : 0,
    #},
    # Electromagnetic Layers
    'EM1'  : {
        'min_dist_to_ip' : em_nominal_radius,
        'max_dist_to_ip' : em_nominal_radius + em_calo_radius[1].sum(),
        'delta_eta'      : 0.00325,
        'delta_phi'      : pi/32,
        #'delta_eta'      : 0.00650,
        #'delta_phi'      : 0.1,
        #'delta_eta'      : 0.025,
        #'delta_phi'      : 0.025,
        'layer_id'       : 1,
    },
    'EM2'  : {
        'min_dist_to_ip' : em_nominal_radius + em_calo_radius[1:2].sum(),
        'max_dist_to_ip' : em_nominal_radius + em_calo_radius[1:3].sum(),
        'delta_eta'      : 0.025,
        'delta_phi'      : pi/128,
        'layer_id'       : 2,
    },
    'EM3'  : {
        'min_dist_to_ip' : em_nominal_radius + em_calo_radius[1:3].sum(),
        'max_dist_to_ip' : em_nominal_radius + em_calo_radius[1:4].sum(),
        'delta_eta'      : 0.050,
        'delta_phi'      : pi/128,
        'layer_id'       : 3,
    },
    # Hadronic Layers
    'HAD1'  : {
        'min_dist_to_ip' : had_nominal_radius + had_calo_radius[:0].sum(),
        'max_dist_to_ip' : had_nominal_radius + had_calo_radius[:1].sum(),
        'delta_eta'      : 0.1,
        'delta_phi'      : pi/32,
        'layer_id'       : 4,
    },
    'HAD2'  : {
        'min_dist_to_ip' : had_nominal_radius + had_calo_radius[:1].sum(),
        'max_dist_to_ip' : had_nominal_radius + had_calo_radius[:2].sum(),
        'delta_eta'      : 0.1,
        'delta_phi'      : pi/32,
        'layer_id'       : 5,
    },
    'HAD3'  : {
        'min_dist_to_ip' : had_nominal_radius + had_calo_radius[:2].sum(),
        'max_dist_to_ip' : had_nominal_radius + had_calo_radius[:3].sum(),
        'delta_eta'      : 0.2,
        'delta_phi'      : pi/32,
        'layer_id'       : 6,
    },
    #'HAD1_Extended'  : {
    #    'min_dist_to_ip' : had_nominal_radius + had_calo_radius[:0].sum(),
    #    'max_dist_to_ip' : had_nominal_radius + had_calo_radius[:1].sum(),
    #    'delta_eta'      : 0.1,
    #    'delta_phi'      : pi/32,
    #    'layer_id'       : 7,
    #    'eta_min'        : 1.3453172584676594,
    #    'eta_max'        : 1.8281107528183245,
    #},
    #'HAD2_Extended'  : {
    #    'min_dist_to_ip' : had_nominal_radius + had_calo_radius[:1].sum(),
    #    'max_dist_to_ip' : had_nominal_radius + had_calo_radius[:2].sum(),
    #    'delta_eta'      : 0.1,
    #    'delta_phi'      : pi/32,
    #    'layer_id'       : 8,
    #    'eta_min'        : 1.3453172584676594,
    #    'eta_max'        : 1.8281107528183245,
    #},
    #'HAD3_Extended'  : {
    #    'min_dist_to_ip' : had_nominal_radius + had_calo_radius[:2].sum(),
    #    'max_dist_to_ip' : had_nominal_radius + had_calo_radius[:3].sum(),
    #    'delta_eta'      : 0.2,
    #    'delta_phi'      : pi/32,
    #    'layer_id'       : 9,
    #    'eta_min'        : 1.3453172584676594,
    #    'eta_max'        : 1.8281107528183245,
    #},
}

from pprint import pprint
pprint(config)




gen = CellGenerator( config )
gen.get_all_cell_centers()
gen.dump()





