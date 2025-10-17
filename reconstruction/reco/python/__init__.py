
__all__ = ["chunks",
           "check_file_exists",
           "merge_args_from_file",
           "update_args_from_file",
           "merge"
           ]


import ROOT
import json
import sys
import os

from typing  import List
from pathlib import Path


def chunks( lst: List, n : int) -> List:
    def _chunks(lst, n):
        """Yield successive n-sized chunks from lst."""
        for i in range(0, len(lst), n):
          yield lst[i:i + n]
    return list(_chunks(lst, n))

def check_file_exists( path : str , ntuple_name : str) -> bool:
    if os.path.exists(path):
        try:
            f = ROOT.TFile( path,"read")
            entries = f.Get( ntuple_name ).GetEntries()
            f.Close()
            return True if entries > 0 else False
        except:
            return False
    else:
        return False

def merge_args_from_file( parser ):
    parser.add_argument('--job-file', action='store',
                    dest='job_file', required=False,
                    type=str, default=None,
                    help='The JSON file used to configure this job automatically by overwritting some arguments.')

    return parser


def update_args_from_file( args  ): 
    if hasattr(args, "job_file"):
        if args.job_file: 
            args.job_file = Path(args.job_file)
            if not args.job_file.exists():
                raise FileNotFoundError(f"Input job file {args.job_file} not found.")
            with open(str(args.job_file), 'r') as f:
                d = json.load(f)
                for argname, argvalue in args._get_kwargs():
                    print(argname)
                    if argname in d.keys():
                        setattr(args, argname, d[argname])
                        print(f"overwritting {argname} with value {argvalue} to new value {d[argname]}")
    return args



def append_index_to_file( output_file : str, index : int ) -> str:
    output_file = output_file.split(".")
    output_file.insert(-1, str(index))
    return '.'.join(output_file)


def merge(output_file : str , files : List[str]):
    command=f"hadd -f {output_file} {' '.join(files)}"
    print(command)
    os.system(command)
    [os.remove(f) for f in files]



from . import gen_job
__all__.extend(gen_job.__all__)
from .gen_job import *


from . import reco_job
__all__.extend(reco_job.__all__)
from .reco_job import *
