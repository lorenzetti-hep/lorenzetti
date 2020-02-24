
__all__ = ['CaloCells', 'CaloCell','CaloGAN_Definitions']

from Gaugi import EDM
from Gaugi  import StatusCode, EnumStringification
from Gaugi.gtypes import NotSet
import numpy as np

# Do not change this definitions.
# the from: https://github.com/hep-lbdl/CaloGAN/tree/master/generation
class CaloGAN_Definitions(EnumStringification):
  # Definition <eta,phi> is <x,y> coordinates
  LAYER_SPECS = [(3, 96), (12, 12), (12, 6), (8, 8), (8, 8), (4, 4)]
  #LAYER_DIV = np.cumsum(map(np.prod, LAYER_SPECS)).tolist()
  #LAYER_DIV = list( zip([0] + LAYER_DIV, LAYER_DIV) )
  LAYER_DIV = [(0, 288), (288, 432), (432, 504), (504, 568), (568, 632), (632, 648)]
  FIRST_EM_LAYER   = 1
  SECOND_EM_LAYER  = 2
  THIRD_EM_LAYER   = 3
  FIRST_HAD_LAYER  = 4
  SECOND_HAD_LAYER = 5
  THIRD_HAD_LAYER  = 6
  OFFSET = 240




class CaloCell(object):
  def __init__(self):
    self._x= 0.0 # In MeV
    self._y= 0.0 # In MeV
    self._energy = 0.0 # In MeV

  def energy(self):
    return self._energy

  def x(self):
    return self._x

  def y(self):
    return self._y

  def setEnergy(self,v):
    self._energy=v

  def setX(self,v):
    self._x=v

  def setY(self,v):
    self._y=v

  def eta(self):
    return self.x()

  def phi(self):
    return self.y()

  def layer(self):
    return self._layer

  def setLayer(self,v):
    self._layer=v


class Deposit(object):
  def __init__(self, x, y, z, energy):
    self._x = x; self._y = y; self._z = z
    self._energy = energy

  def x(self):
    return self._x

  def y(self):
    return self._y

  def z(self):
    return self._z

  def energy(self):
    return self._energy


class CaloCells(EDM):

  __eventBranches = [
                      #'EventNumber',
                      #'RunNumber',
                      'TotalEnergy',
                      'deposit_x',
                      'deposit_y',
                      'deposit_z',
                      'deposit_energy',
                    ]
  __eventBranches.extend(['cell_%d'%(i) for i in range(648+6)])



  def __init__(self):
    EDM.__init__(self)
		#  Detector ROI dimesions:
		#        |--------- EM ----------|----------- HAD ----------|
		#          15cm     30cm     3cm    40cm       40cm    20cm
		#   +    +--------------------------------------------------+
		#   |    |      |           |    |         |         |      |   
		#  48cm  |      |           |    |         |         |      |
		#   |    |      |           |    |         |         |      |
		#   +    +--------------------------------------------------+
		# Eletromagnetic layers
    self._first_em_sampling  = NotSet
    self._second_em_sampling = NotSet
    self._third_em_sampling  = NotSet
		# Hadronic layers
    self._first_had_sampling  = NotSet
    self._second_had_sampling = NotSet
    self._third_had_sampling  = NotSet


  def initialize(self):
    try:
      # Link all branches
      for branch in self.__eventBranches:
        self.setBranchAddress( self._tree, branch, self._event)
        self._branches.append(branch) # hold all branches from the body class
      return StatusCode.SUCCESS
    except TypeError as e:
      self._logger.error("Impossible to create the CaloTowers Container. Reason:\n%s", e)
      return StatusCode.FAILURE


  def execute(self):
    from EventSimulator import CaloGAN_Definitions as Layer
    def convert2obj(c,layer):
      object_list=[]
      for (x,y), value in np.ndenumerate(c):
        obj=CaloCell(); obj.setX(x); obj.setY(y); obj.setEnergy(c[x][y]); obj.setLayer(layer)
        object_list.append(obj)
      return object_list

		# Eletromagnetic layers
    self._first_em_sampling  = convert2obj(self.get_raw_cells(Layer.FIRST_EM_LAYER ) , Layer.FIRST_EM_LAYER )
    self._second_em_sampling = convert2obj(self.get_raw_cells(Layer.SECOND_EM_LAYER) , Layer.SECOND_EM_LAYER)
    self._third_em_sampling  = convert2obj(self.get_raw_cells(Layer.THIRD_EM_LAYER ) , Layer.THIRD_EM_LAYER )
		# Hadronic layers
    self._first_had_sampling  = convert2obj(self.get_raw_cells(Layer.FIRST_HAD_LAYER ) , Layer.FIRST_HAD_LAYER )
    self._second_had_sampling = convert2obj(self.get_raw_cells(Layer.SECOND_HAD_LAYER) , Layer.SECOND_HAD_LAYER)
    self._third_had_sampling  = convert2obj(self.get_raw_cells(Layer.THIRD_HAD_LAYER ) , Layer.THIRD_HAD_LAYER )

    return StatusCode.SUCCESS


  # private method
  def get_raw_cells(self, layer):
    # See: https://github.com/hep-lbdl/CaloGAN/tree/master/generation
    # Get the calo GAN cells schemma
    # First EM layer definitions: 3 X 96
    if layer is CaloGAN_Definitions.FIRST_EM_LAYER:
      cells = np.array([ getattr(self._event, 'cell_%d'%c) for c in range(CaloGAN_Definitions.LAYER_DIV[0][0],
        CaloGAN_Definitions.LAYER_DIV[0][1] )])
      return cells.reshape((3,96))
    # Second EM layer definicions: 12 X 12
    elif layer is CaloGAN_Definitions.SECOND_EM_LAYER:
      cells = np.array([ getattr(self._event, 'cell_%d'%c) for c in range(CaloGAN_Definitions.LAYER_DIV[1][0],
        CaloGAN_Definitions.LAYER_DIV[1][1] )])
      return cells.reshape((12,12))
    # Third EM Layer definitions: 12 x 6
    elif layer is CaloGAN_Definitions.THIRD_EM_LAYER:
      cells = np.array([ getattr(self._event, 'cell_%d'%c) for c in range(CaloGAN_Definitions.LAYER_DIV[2][0],
        CaloGAN_Definitions.LAYER_DIV[2][1] )])
      return cells.reshape((12,6))


    # First HAD layer definitions: 8 X 8
    elif layer is CaloGAN_Definitions.FIRST_HAD_LAYER:
      cells = np.array([ getattr(self._event, 'cell_%d'%c) for c in range(CaloGAN_Definitions.LAYER_DIV[3][0],
        CaloGAN_Definitions.LAYER_DIV[3][1] )])
      return cells.reshape((8,8))
    # Second HAD layer definicions: 8 X 8
    elif layer is CaloGAN_Definitions.SECOND_HAD_LAYER:
      cells = np.array([ getattr(self._event, 'cell_%d'%c) for c in range(CaloGAN_Definitions.LAYER_DIV[4][0],
        CaloGAN_Definitions.LAYER_DIV[4][1] )])
      return cells.reshape((8,8))
    # Third HAD Layer definitions: 4 x 4
    elif layer is CaloGAN_Definitions.THIRD_HAD_LAYER:
      cells = np.array([ getattr(self._event, 'cell_%d'%c) for c in range(CaloGAN_Definitions.LAYER_DIV[5][0],
        CaloGAN_Definitions.LAYER_DIV[5][1] )])
      return cells.reshape((4,4))


    else:
      self._logger.warning('Invalid layer definition for CaloGAN')



  def totalEnergy(self):
    return self._event.TotalEnergy


  def getCollection( self, layer ):
    if layer is CaloGAN_Definitions.FIRST_EM_LAYER:
      return self._first_em_sampling
    elif layer is CaloGAN_Definitions.SECOND_EM_LAYER:
      return self._second_em_sampling
    elif layer is CaloGAN_Definitions.THIRD_EM_LAYER:
      return self._third_em_sampling
    elif layer is CaloGAN_Definitions.FIRST_HAD_LAYER:
      return self._first_had_sampling
    elif layer is CaloGAN_Definitions.SECOND_HAD_LAYER:
      return self._second_had_sampling
    elif layer is CaloGAN_Definitions.THIRD_HAD_LAYER:
      return self._third_had_sampling
    else:
      self._logger.warning('Invalid layer definition for CaloGAN')


  def getDeposits(self):
    deposits = list()
    for idx in range(self._event.deposit_x.size()):
      p = Deposit( self._event.deposit_x.at(idx),
                         self._event.deposit_y.at(idx),
                         self._event.deposit_z.at(idx),
                         self._event.deposit_energy.at(idx))
      deposits.append(p)
    return deposits

