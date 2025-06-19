__all__  = [
    "Parallel",
    "merge_args",
    "update_args",
]


import ROOT
import argparse
import random
import json
import sys
import os
import joblib

from math               import ceil
from typing             import List, Dict, Callable, Union
from pathlib            import Path
from pprint             import pprint
from expand_folders     import expand_folders
from tqdm               import tqdm

from reco import chunks, merge, merge_args_from_file, update_args_from_file, append_index_to_file, check_file_exists




def merge_args( parser ):

    parser.add_argument('-e', '--event-numbers', action='store',
                    dest='event_numbers', required=False,
                    type=str, default=None,
                    help="The event number list separated "
                    "by ','. e.g. --event-numbers '0,1,2,3'")
    parser.add_argument('-o', '--output-file', action='store',
                        dest='output_file', required=True,
                        help="The output file.") 
    parser.add_argument('-s', '--seed', action='store',
                        dest='seed', required=False,
                        type=int, default=0,
                        help="The pythia seed (zero is the clock system)")
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



def update_args( args  ):
    return update_args_from_file(args)

class Parallel:

    def __init__(self,
                 output_file       : str,
                 seed              : int=512,
                 number_of_threads : int=1,
                 number_of_events  : int=-1,
                 events_per_job    : int=-1,
                 event_numbers     : Union[str,List[int]]=None,
                 merge             : bool=False,
                 ntuple_name       : str="particles",
                 overwrite         : bool = False,
                ):
        
        self.event_numbers     = event_numbers
        self.number_of_events  = number_of_events
        self.number_of_threads = number_of_threads
        self.events_per_job    = events_per_job
        self.merge_files       = merge
        self.ntuple_name       = ntuple_name
        self.output_file       = output_file
        self.overwrite=overwrite
        self.seed=seed


    def __call__(self, function : Callable, **args ):

        plan = self.build_plan()
        pprint(plan)
        jobs = []
        for output_file, config in plan.items():
            if not check_file_exists( output_file, self.ntuple_name ) or self.overwrite:
                jobs.append( (output_file, config['evt'], config['seed']) )
        pprint(jobs)
        pool = joblib.Parallel(n_jobs=self.number_of_threads)
        pool(joblib.delayed(function)(
            events=events,
            output_file=output_file,
            seed=seed,
            **args
        )
            for output_file, events, seed in jobs)
        files = list(plan.keys())
        if self.merge_files or len(files)==1:
            merge(self.output_file, files)


    def get_events_per_job(self):
        if self.events_per_job < 0:
            return ceil(self.number_of_events/self.number_of_threads)
        else:
            return self.events_per_job

    def build_plan(self): 
    
        def get_random_seed()->int:
            return random.randint(1, 900000000 )

        if self.event_numbers:
            event_numbers = [int(event_number) for event_number in self.event_numbers.split(",")] if type(self.event_numbers) is str else self.event_numbers
            self.number_of_events = len(event_numbers_list)
            event_numbers = chunks( event_numbers, self.get_events_per_job())
        else:
            event_numbers = chunks( list(range(self.number_of_events)) ,self.get_events_per_job() )
        
        plan = {}
        random.seed(self.seed)
        for idx, events in enumerate(event_numbers):
            output_file = append_index_to_file(self.output_file, idx)
            plan[output_file] = {"evt":events, "seed": get_random_seed()}
        return plan
     

def create_parallel_job( args ):
    
    return Parallel( 
                event_numbers     = args.event_numbers,
                output_file       = args.output_file,
                number_of_threads = args.number_of_threads ,
                number_of_events  = args.number_of_events,
                events_per_job    = args.events_per_job,
                merge             = args.merge,
                overwrite         = args.overwrite,
                seed              = args.seed,
            )
