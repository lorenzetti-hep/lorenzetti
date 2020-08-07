# SDumont Docker image

This docker image is provided to run jobs on the LNCC SDumont computer.

* Script usage:
```
lorenzett_run.sh - Script for running Lorenzett through docker in an one-line script
 
./lorenzett_run.sh [arguments]
 
Arguments:
-h, --help                show this message
-f, --filter              chooses which type of event you're interested
-e, --event               sets the desired number of events
-p, --pileup              sets the pileup average
-o, --output              sets the output filename
--bc_id_start             sets bc_id_start
--bc_id_end               sets bc_id_end
 
All arguments are required
```

* How to run:

```
docker run -v <output-path>:/output gabrielmilan/lorenzett:sdumont <args>
```

Refer to the script usage above for information on `<args>`.
