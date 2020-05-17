


def get_pulse(path):
  
  file = open(path,'r')
  lines = file.readlines()
  amplitude = []; time = []
  for line in lines:
    a = line.split(' ')[1].replace('\n','')  
    t = line.split(' ')[0]
    amplitude.append( float(a) ); time.append(float(t))
  return time , amplitude



def plot_pulse( time, amplitude , outname):

  import matplotlib.pyplot as plt
  plt.plot(time, amplitude)
  plt.ylabel('Amplitude')
  plt.xlabel('Time [ns]')
  plt.savefig(outname)
  plt.show()


lar_time, lar_amp = get_pulse('../data/pulseLarg.dat')
tile_time, tile_amp = get_pulse('../data/pulseTile.dat')


plot_pulse( lar_time, lar_amp , 'lar_pulse.pdf')
plot_pulse( tile_time, tile_amp , 'tile_pulse.pdf')

















