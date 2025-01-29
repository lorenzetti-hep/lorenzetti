

## How to produce Zee events from scratch?

'''
run_job.py create --production-card zee.card -o zee_jobs
'''

Than, we can launch as a single job:

'''
run_job.py run -j zee_jobs/job.0.json -o experiment
'''

in parallel using prun_jobs:

'''
mkdir zee
prun_jobs.py -i jobs -o zee/job --number-of-threads 10 -c "run_job.py run -j %IN -o %OUT"
'''


