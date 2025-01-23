#!/usr/bin/env python3

import argparse
import numpy as np
import multiprocessing
import sys,os,traceback,json
from GaugiKernel import get_argparser_formatter, chunks
from GaugiKernel import LoggingLevel, Logger


    
def run():
    
    logger = Logger.getModuleLogger("job")
    parser = argparse.ArgumentParser(description = '', formatter_class=get_argparser_formatter() )

    parser.add_argument('--job','-j', action='store', dest='job', 
                        required = True, type=str, default=None,
                        help = "The production card")
    
    parser.add_argument('--output_dir','-o', action='store', dest='output_dir',
                        required = False, type=str, default='job.json',
                        help = "The production card")
    
    parser.add_argument('-nt','--number-of-threads', action='store', dest='number_of_threads', required = False, type=int, 
                        default=multiprocessing.cpu_count(),
                        help = "The number of threads")

    if len(sys.argv)==1:
      parser.print_help()
      sys.exit(1)
    args = parser.parse_args()

    localdir = os.path.abspath(args.output_dir)
    job      = json.load(open(args.job,'r'))
    job_id   = job["job"].get("job_id")
    run_number = job["run"].get("run_number")
    seed      = job["job"]["seed"]
    workarea  = f"{localdir}/{job_id}"
    
    envs = {
        f"%SEED"           : str(seed),
        f"%RUN_NUMBER"     : str(run_number),
        f"%JOB_WORKAREA"   : workarea,
        f"%EVENT_NUMBERS"  : ",".join([str(evt) for evt in job["job"]["event_numbers"]]),
        f"%CPU_CORES"      : str(args.number_of_threads)
    }
    
    os.makedirs(workarea, exist_ok=True)
    
    stages = job.get("stages", [])
    
    for idx, params in enumerate(stages):

        step_dir = params.get("name")
        step_path = f"{workarea}/{step_dir}"
        os.makedirs(step_path, exist_ok=True)
        command     = params.get("script")
        input_path  = params.get("input",None) 
        output_path = params.get("output")
        if input_path:
            command+= f" -i {input_path}"
        command += f" -o {output_path}"
        for key, value in params.get("extra_args",{}).items():
            command += f" --{key} {value}" if value else f" --{key}"
        for key, value in envs.items():
            command = command.replace(key,value)
        
        
        
        print(command)
        if not os.path.exists(f"{step_path}/completed"):
            returncode = os.system(command)
            print(returncode)
            if returncode==0:
                with open(f"{step_path}/completed", 'w') as f:
                    f.write("completed")
        else:
            print("skip!")
        
        
        
if __name__ == "__main__":
    run()
        
        
        
        
         
    
    