__all__  = [
    "Parallel",
    "merge_args",
    "update_args",
    "create_parallel_job"
]


import ROOT
import argparse
import random
import json
import sys
import os
import joblib
import traceback

from math               import ceil
from typing             import List, Dict, Callable, Union
from pathlib            import Path
from pprint             import pprint
from expand_folders     import expand_folders
from tqdm               import tqdm

from reco import chunks, merge_args_from_file, update_args_from_file, append_index_to_file, check_file_exists, merge


def merge_args( parser, exclude_input_file : bool=False ):

    parser.add_argument('-i', '--input-file', action='store',
                            dest='input_file', required=True,
                            help="The input file or folder to run the job")
    parser.add_argument('-o', '--output-file', action='store',
                        dest='output_file', required=True,
                        help="The output file.")
    parser.add_argument('--nov', '--number-of-events', action='store',
                        dest='number_of_events', required=False,
                        type=int, default=-1,
                        help="The total number of events to run.")
    parser.add_argument('--events-per-job', action='store',
                        dest='events_per_job', required=False,
                        type=int, default=-1,
                        help="The number of events per job")
    parser.add_argument('-nt', '--number-of-threads', action='store',
                        dest='number_of_threads', required=False,
                        type=int, default=1,
                        help="The number of threads")
    parser.add_argument('-m','--merge', action='store_true',
                        dest='merge', required=False,
                        help='Merge all files.')
    parser.add_argument('--overwrite', action='store_true',
                        dest='overwrite', required=False,
                        help='Rerun all jobs.')
   
    return merge_args_from_file(parser)


def update_args( args ):

    args.input_file = Path(args.input_file)
    if not args.input_file.exists():
        raise FileNotFoundError(f"Input file {args.input_file} not found.")
    if args.input_file.is_dir():
        args.input_file = expand_folders(os.path.abspath(args.input_file))
        args.input_file = [Path(inp) for inp in args.input_file if inp.endswith('.root')]
    else:
        args.input_file = os.path.abspath(args.input_file)

    return update_args_from_file(args)



class Parallel:

    def __init__(self,
                 files             : List[str],
                 output_file       : str,
                 number_of_threads : int=1,
                 number_of_events  : int=-1,
                 events_per_job    : int=-1,
                 merge             : bool=False,
                 ntuple_name       : str="CollectionTree",
                 overwrite         : bool = False,
                ):  

        self.files             = files
        self.number_of_events  = number_of_events
        self.number_of_threads = number_of_threads
        self.events_per_job    = events_per_job
        self.merge_files       =  merge
        self.ntuple_name = ntuple_name
        self.output_file = output_file
        self.overwrite=overwrite


    def build_plan(self) -> Dict:

        plan = {}
        def add( d : Dict, key_a : str, key_b: str, value_a : str, value_b : int):
            if key_a in d.keys():
                if key_b in d[key_a].keys():
                    d[key_a][key_b]["evt"]+=[value_b]
                else:
                    d[key_a][key_b] = {"input_file":value_a, "evt":[value_b]}
            else:
                d[key_a] = {key_b:{"input_file":value_a, "evt":[value_b]}}

        nov = 0
        print(self.files)
        for idx, path in tqdm( enumerate(self.files), desc="Loop over files...", total=len(self.files)):
            try:
                f = ROOT.TFile( path,"read")
                entries = f.Get( self.ntuple_name ).GetEntries()
                f.Close()
            except:
                traceback.print_exc()
                print(f"it is not possible to read file {path}")
                continue
            if entries > 0:
                events_per_file = list(range(entries))
                events_per_file = chunks(events_per_file, self.events_per_job) if self.events_per_job > 0 else [events_per_file]                
                output_per_file = append_index_to_file(self.output_file, idx)
                for jdx, events in enumerate(events_per_file):
                    output_file_per_job = append_index_to_file( output_per_file, jdx )
                    for evt in events:
                        add(plan, output_per_file, output_file_per_job, path, evt )
                        nov+=1
                        if (self.number_of_events > 0) and (nov >= self.number_of_events):
                            return plan
        return plan



    def __call__(self, function, **args):

        plan = self.build_plan()
        jobs = []
        for output_per_file, values_per_file in plan.items():
            for output_per_file_per_job, job in values_per_file.items():
                input_file = job["input_file"]
                events     = job["evt"]
                if not check_file_exists(output_per_file_per_job, self.ntuple_name) or self.overwrite :
                    jobs.append( (input_file, output_per_file_per_job, events) )
        pprint(jobs)
        pool = joblib.Parallel(n_jobs=self.number_of_threads)
        pool( joblib.delayed(function)(events=events, input_file=input_file, output_file=output_file, **args) for input_file, output_file, events in jobs)
        
        files = []
        for output_per_file in plan.keys():
            files+=list(plan[output_per_file].keys())
        if self.merge_files or len(files)==1:
            merge( self.output_file , files)                    
            


def create_parallel_job( args ):
    return Parallel( 
                files             = args.input_file,
                output_file       = args.output_file,
                number_of_threads = args.number_of_threads,
                number_of_events  = args.number_of_events,
                events_per_job    = args.events_per_job,
                merge             = args.merge,
                overwrite         = args.overwrite
            )   