



CPU_N=$(grep -c ^processor /proc/cpuinfo)

# main event simulation command
simu_trf.py -i Zee.EVT.root -o Zee.HIT.root -nt $CPU_N

# minimum bias event simulation
simu_trf.py -i mb.EVT.root -o mb.HIT.root -nt $CPU_N

