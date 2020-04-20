# Lorenzetti Event Simulator

This is the first event generator for HEP using a generic calorimeter for machine learning 
studies based on athena framework (ATLAS).



## Detector construction:

It's a composite cilindric calorimeter with eletromagnetic and hadronic layers with 6.12m in z starting 90cm of disntance
in the collision line and 148cm of material.

Description:

- First EM layer (stripts) with 15 cm depth (xy) and 2 segments (layers) of LAr and Pb (0.003 x 0.003); 
- Second EM layer with 30 cm depth (xy) and 2 segments (layers) of LAr and Pb (0.025 x 0.025); 
- Third EM layer with 3 cm depth (xy) and 2 segments (layers) of LAr and Pb (0.05 x 0.025); 
- First HAD layer with 40 cm depth (xy) and 2 segments (layers) of Cesium iodide and iron (0.1 x 0.1); 
- Second HAD layer with 40 cm depth (xy) and 2 segments (layers) of Cesium iodide and iron (0.1 x 0.1); 
- Third HAD layer with 20 cm depth (xy) and 1 segments (layers) of Cesium iodide and iron (0.2 x 0.1); 


## Requirements For Local Installation:

- Geant4 (opengl or qt4 is required for graphic interface, https://github.com/jodafons/geant4.git);
- ROOT (https://github.com/root-project/root.git);
- Pythia8 (https://github.com/jodafons/pythia.git);
- HEPMC (https://github.com/jodafons/hepmc.git);
- FastJet (https://github.com/jodafons/fastjet.git).


## Docker image:

Docker image for cluster usage based on Ubuntu 18.
```bash
docker push jodafons/lorenzett
```


## Usage:

### Event generation:

Use this command to generate `Zee` events:
```bash
generator.py --filter Zee -b $LZT_PATH/generator/PythiaGenerator/data/minbias_zee_config.cmnd -m $LZT_PATH/generator/PythiaGenerator/data/zee_config.cmnd --outputLevel 6 --seed 0 -evt 1000 -o zee.root
```
> **NOTE**: To get the time clock system as seed use the argument `--seed 0`.

To run the generator in parallel you must use this with the `prun_job` command. To run the `Zee` generation in parallel use this command:

```bash
prun_job.py -c "generator.py --filter Zee -b $LZT_PATH/generator/PythiaGenerator/data/minbias_zee_config.cmnd -m $LZT_PATH/generator/PythiaGenerator/data/zee_config.cmnd --outputLevel 6 --seed 0 --evt 1000" -mt 4 -n 10 -o zee.root
```
> **NOTE**: Use this command to run the event `Zee` generatio with 1K events, 4 threads (`-mt`) and 10 jobs (`-n`).

To run the `JF17` events use this command:
```bash
prun_job.py -c "generator.py --filter JF17 -b $LZT_PATH/generator/PythiaGenerator/data/minbias_jets_config.cmnd -m $LZT_PATH/generator/PythiaGenerator/data/jets_config.cmnd --outputLevel 6 --seed 0 --evt 1000" -mt 4 -n 10 -o jf17.root
```

### Reconstruction:

After generate the events using the `generator.py` command you must pass the output file as input to the reconstruction transformation. To run the reconstruction use this command:

```bash
reco_trf.py -i zee.root --outputLevel 6 -nt 4 -o reco_zee.root
```
> **NOTE**: Run the reconstruction with 4 threads (`-nt`). You can pass the number of events to reconstruct with `--evt`.




## References:

- Athena framework: https://gitlab.cern.ch/atlas/athena


