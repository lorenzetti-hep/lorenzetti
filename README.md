# Lorenzett Event Simulator

This is the first event generator for HEP using a generic calorimeter for machine learning 
studies based on athena framework.

## Docker image:

Docker image for cluster usage based on Ubuntu 18.
```bash
docker pull jodafons/lorenzett
```


## Usage:

### Event generation:

Use this command to generate `Zee` events. To get the time clock system as seed use the argument `--seed 0`.
```bash
generator.py --filter Zee -i zee_config.cmnd --outputLevel 6 --seed 0 -evt 1000 -o zee.root --pileupAvg 40 --bc_id_start -8 --bc_id_end 7
```
To run the generator in parallel you must use this with the `prun_job` command. To run the `Zee` generation in parallel use this command:

```bash
prun_job.py -c "generator.py --filter Zee -i zee_config.cmnd --outputLevel 6 --seed 0 -evt 1000 -o zee.root --pileupAvg 40 --bc_id_start -8 --bc_id_end 7"
```
> **NOTE**: Use this command to run the event `Zee` generatio with 1K events, 4 threads (`-mt`) and 10 jobs (`-n`).

To run the `JF17` events use this command:
```bash
prun_job.py -c "generator.py --filter JF17 -i jets_config.cmnd --outputLevel 6 --seed 0 --evt 1000" -mt 4 -n 10 -o jf17.root --pileupAvg 40 --bc_id_start -8 --bc_id_end 7"
```

To generate and collect minimum bias for the entiry calorimeter only just use:
```bash
prun_job.py -c "generator.py --filter MinimumBiasAll  --outputLevel 6 --seed 0 --evt 1000" -mt 4 -n 10 -o mb.root --pileupAvg 40 --bc_id_start -8 --bc_id_end 7"
```

> **Note**: If you would like to collect minimum bias for an especific calorimete region you just include the eta/phi region [here](https://github.com/jodafons/lorenzett/blob/master/scripts/generator.py#L112) and use the `--filter` with name `MinimumBias`.


### Reconstruction:

After generate the events using the `generator.py` command you must pass the output file as input to the reconstruction transformation. To run the reconstruction use this command:

```bash
reco_trf.py -i zee.root --outputLevel 6 -nt 4 -o reco_zee.root
```

## Requirements For Local Installation:

- Geant4 (opengl or qt4 is required for graphic interface, https://github.com/jodafons/geant4.git);
- ROOT (https://github.com/root-project/root.git);
- Pythia8 (https://github.com/jodafons/pythia.git);
- HEPMC (https://github.com/jodafons/hepmc.git);
- FastJet (https://github.com/jodafons/fastjet.git).


## References:

- Athena framework: https://gitlab.cern.ch/atlas/athena


