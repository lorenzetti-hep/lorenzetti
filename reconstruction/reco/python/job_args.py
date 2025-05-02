__all__  = [
    "merge_args",
    "update_args",
    "get_events_per_job",
    "get_evt_job_params",
    "get_input_output_job_pairs",
    "merge",
]



import argparse
import random
import json
import sys
import os

from math        import ceil
from typing      import List
from pathlib     import Path
from joblib      import Parallel, delayed
from evtgen      import Pythia8
from filters     import Zee, Pileup
from pprint      import pprint

from GenKernel   import EventTape
from GaugiKernel import get_argparser_formatter
from GaugiKernel import LoggingLevel
from GaugiKernel import GeV



def merge_args( parser ):

    parser.add_argument('--job-file', action='store',
                        dest='job_file', required=False,
                        type=str, default=None,
                        help='The JSON file used to configure this job automatically by overwritting some arguments.')

    return parser


def update_args( args ):

    if hasattr(args, "job_file") and hasattr(args, "job_key"):
        if args.job_file: 
            args.job_file = Path(args.job_file)
            if not args.job_file.exists():
                raise FileNotFoundError(f"Input job file {args.job_file} not found.")
            with open(str(args.job_file), 'r') as f:
                d = json.load(f)
                for argname, argvalue in args._get_kwargs():
                    if argname in d.keys():
                        setattr(args, argname, d[argname])
                        print(f"overwritting {argname} with value {argvalue} to new value {d[argname]}")
    return args



def get_events_per_job(args):
    if args.events_per_job is None:
        return ceil(args.number_of_events/args.number_of_threads)
    else:
        return args.events_per_job

def get_random_seed()->int:
    return random.randint(1, 900000000 )

def get_evt_job_params(args, force:bool=False):
    if args.event_numbers:
        if type(args.event_numbers) is str: # convert from str to a list of ints
            event_numbers_list = args.event_numbers.split(",")
            event_numbers_list = [int(event_number) for event_number in event_numbers_list]
        else:
            event_numbers_list = args.event_numbers
        args.number_of_events = len(event_numbers_list)
        events_per_job = get_events_per_job(args)
        event_numbers = (
            event_numbers_list[start:start+events_per_job]
            for start in range(0, args.number_of_events, events_per_job)
        )
    else:
        events_per_job = get_events_per_job(args)
        event_numbers = (
            list(range(start, start+events_per_job))
            for start in range(0, args.number_of_events, events_per_job)
        )

    random.seed(args.seed)
    splitted_output_filename = args.output_file.split(".")
    for i, events in enumerate(event_numbers):
        output_file = splitted_output_filename.copy()
        output_file.insert(-1, str(i))
        output_file = '.'.join(output_file)
        if not force and os.path.exists(output_file):
            print(f"{i} - Output file {output_file} already exists. Skipping.")
            continue
        yield events, output_file, get_random_seed()




def get_input_output_job_pairs(args, force:bool=False):
    splitted_output_filename = args.output_file.split(".")
    for i, input_file in enumerate(args.input_file):
        output_file = splitted_output_filename.copy()
        if len(args.input_file)>1:
          output_file.insert(-1, str(i))
        output_file = Path('.'.join(output_file))
        if not force and output_file.exists():
            print(f"{i} - Output file {output_file} already exists. Skipping.")
            continue
        yield input_file, output_file


def merge( args, files : List[str]):
    command=f"hadd -f {args.output_file} {' '.join(files)}"
    print(command)
    os.system(command)
    [os.remove(f) for f in files]
