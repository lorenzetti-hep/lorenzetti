#!/bin/bash
#SBATCH --job-name=singularity_test
#SBATCH --cpus-per-task=2
#SBATCH --output=singularity_output.%j.out

export JOB_WORKAREA=
export OUTPUT_PATH=/home/joao.pinto/git_repos/lorenzetti/jobs/2021-09-01_16-00-00/output
export JOB_PATH=/home/joao.pinto/git_repos/lorenzetti/jobs/2021-09-01_16-00-00
export LZT_PATH=/home/joao.pinto/git_repos/lorenzetti
export IMG=/mnt/cern_data/joao.pinto/images/lorenzetti_dev.sif
singularity exec $IMG bash -c "source /hep/setup_hep.sh && cd $LZT_PATH && source setup.sh && cd $JOB_WORKAREA && run_job.py run -j $JOB_PATH -o $OUTPUT_PATH"
