from Gaugi import load
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm

data = load('e.npz')
features = data['features'].tolist()



def plot_layers( sampling, d, norm=False, evt=None ):

  def plot_cells( ax, cells, vmin=None, vmax=None , label='', norm=False):
    
    ax.imshow(cells,interpolation='nearest',aspect= cells.shape[1]/cells.shape[0],
                                   norm=LogNorm(vmin=vmin, vmax=vmax) if norm else None)
    ax.set(xlabel='$\eta$', ylabel='$\phi$',title=label)
    ax.tick_params(axis='both', which='both', bottom='off', top='off',
                   left='off', right='off', labelbottom='off',
                   labelleft='off')
  import random
  f, axarr = plt.subplots(1,3, figsize=(18, 10))
  evt=1
  #if evt is None:
  #  evt = random.randint(0,len(d))
  print( ('Eta = %1.2f, Phi = %1.2f')%(d['data'][evt, features.index('eta')] , 
                                       d['data'][evt, features.index('phi')]) )

  plot_cells( axarr[0],d['cells_'+sampling+'1'][evt] , norm=norm)
  plot_cells( axarr[1],d['cells_'+sampling+'2'][evt] , norm=norm)
  plot_cells( axarr[2],d['cells_'+sampling+'3'][evt] , norm=norm)

  f.savefig( "cells.pdf" )


plot_layers('em',data,True)















