#!/usr/bin/env python3

import argparse
import numpy as np
import multiprocessing
import sys,os,json
from GaugiKernel import get_argparser_formatter, chunks



def build_argparser_create_jobs():
    parser = argparse.ArgumentParser(description = '', formatter_class=get_argparser_formatter() , add_help=False)

    parser.add_argument('--production-card','-p', action='store', dest='production_card', 
                        required = False, type=str, default=None,
                        help = "The production card")
    
    parser.add_argument('--output','-o', action='store', dest='output',
                        required = False, type=str, default=f'jobs',
                        help = "The production card")
    return parser

def build_argparser_run_job():
    parser = argparse.ArgumentParser(description = '', formatter_class=get_argparser_formatter() , add_help=False)

    parser.add_argument('--job','-j', action='store', dest='job', 
                        required = True, type=str, default=None,
                        help = "The production card")
    
    parser.add_argument('--output-dir','-o', action='store', dest='output_dir',
                        required = False, type=str, default='job.json',
                        help = "The job output path.")
    
    parser.add_argument('-nt','--number-of-threads', action='store', dest='number_of_threads', required = False, type=int, 
                        default=multiprocessing.cpu_count(),
                        help = "The number of threads")
    return parser

def run_create_jobs(args):
    prod = json.load(open(args.production_card,'r'))
    chunk_size = prod["run"]["nov_per_job"]
    seed = prod["run"]["seed"]
    os.makedirs(args.output, exist_ok=True)
    for idx, evts in enumerate(chunks(np.arange( 0, prod["run"]["nov"]).tolist(), chunk_size)):
        with open( f"{args.output}/job.{idx}.json", 'w') as f:
            d = prod | {"job":{"event_numbers":evts, "seed":seed*(idx+1),"job_id":idx}}
            json.dump(d,f ,indent = 4)

def run_job(args):
    job        = json.load(open(args.job,'r'))
    job_id     = job["job"].get("job_id")
    run_number = job["run"].get("run_number")
    seed       = job["job"]["seed"]
    workarea   = os.path.abspath(args.output_dir)
    
    envs = {
        f"%SEED"           : str(seed),
        f"%RUN_NUMBER"     : str(run_number),
        f"%JOB_WORKAREA"   : workarea,
        f"%EVENT_NUMBERS"  : ",".join([str(evt) for evt in job["job"]["event_numbers"]]),
        f"%CPU_CORES"      : str(args.number_of_threads),
        f"%JOB_ID"         : str(job_id),
    }
    
    os.makedirs(workarea, exist_ok=True)
    stages = job.get("stages", [])
    for params in stages:
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
                sys.exit(returncode)
    sys.exit(0)


def build_argparser():
    formatter_class = get_argparser_formatter()
    parser    = argparse.ArgumentParser(formatter_class=formatter_class)
    option = parser.add_subparsers(dest='option')
    option.add_parser("run"   , parents = [build_argparser_run_job()]   ,help='Run a job using trf scripts',formatter_class=formatter_class)
    option.add_parser("create", parents = [build_argparser_create_jobs()]   ,help='Create jobs from a production card.',formatter_class=formatter_class)
    return parser
    
    
    
    
def run_parser(args):
    if args.option == "create":
        run_create_jobs(args)
    elif args.option == "run":  
        run_job(args)
    else:
        print("Option not implemented")
  


        

if __name__ == "__main__":
    parser = build_argparser()
    if len(sys.argv)==1:
        print(parser.print_help())
        sys.exit(1) 
    args = parser.parse_args()
    run_parser(args)    