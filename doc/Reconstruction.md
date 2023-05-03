

# Reconstruction

The simulation pipeline has four main steps:

1. Event generation
2. Shower propagation
3. Digitalization
4. Event reconstruction


## Generation

A set of `event filters` is provided on `generator/filters/scripts`. Usage example:

```
prun_jobs.py -c "gen_zee.py --pileupAvg 0 --nov %NOV --eventNumber %OFFSET -o %OUT -s %SEED" -nt 40 --nov $NOV --seed $seed --novPerJob 200 -o EVT.root
```

It is also possible to use different simulators (`gun`) to generate the inputs for the framework. For this, it is required to produce HepMC-compatible files after the simulation. These files may be used as inputs for the `event filters` stage. See details below on the `generator/scripts/hepmc`.


## Simulation

This is an examplete how to propagate the particles through the `Geant4` module (considering the file produced on the first example step):

```
simu_trf.py -i EVT.root -o HIT.root --enableMagneticField
```

##  Digitalization

The digitalization process may be configured to use different energy estimation methods such as Optimal Filter (OF) and Constrained Optimal Filter (COF). It is also possible to simulate the crosstalk effect between adjacent calorimeter cells. More details on how to use such features, check the Customization section. In the digitalization step, it is used second step output to feed `digit_trf.py` script. For instance: 

```
digit_trf.py -i HIT.root -o ESD.root
```

Some customization concerning about CrossTalk simulation and energy estimation methods changes:

- ```--simulateCrossTalk```: this simulates the crosstalk efect on the calorimeter cells
- ```--estimationMethodHAD```: this change the energy estimation method on the hadronic calorimeter. Two options for now are allowd: ```OF``` and ```COF```

An example for full customized digitalization step:

```
digit_trf.py -i HIT.root -o ESD.root --simulateCrossTalk --estimationMethodHAD COF
```

### Event Reconstruction

Use the third step output to feed `reco_trf.py` script. For instance:

```
reco_trf.py -i ESD.root -o AOD.root
```

A set of examples files for each step can be found at the ```project drive``` folder [here](https://drive.google.com/drive/folders/1z3h6kP0VTVNml4sIQ8sB6eZtwXnrAdtG?usp=share_link). These small examples are for `Zee` decay.


