
numberOfJobs = 10

from Gaugi import save, mkdir_p

mkdir_p('job_config_dummy')

for job_id in range(numberOfJobs):
  name = 'job_config_dummy/job_config.id%1.4d.dummy' % (job_id)
  f= open(name,'w')
  f.write(str(job_id))
  f.close()



f= open('data.dummy','w')
f.close()



