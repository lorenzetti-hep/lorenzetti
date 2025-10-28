#!/usr/bin/env python3

import argparse
import sys
import os
import evtgen

from math        import ceil
from pathlib     import Path
from typing      import List
from joblib      import Parallel, delayed
from evtgen      import Pythia8
from filters     import JF17
from GenKernel   import EventTape
from GaugiKernel import get_argparser_formatter
from GaugiKernel import LoggingLevel
from GaugiKernel import GeV

from reco.gen_job import merge_args, update_args, create_parallel_job


datapath    = os.environ["LORENZETTI_EVTGEN_DATA_DIR"]
JF17_FILE   = f'{datapath}/jet_config.cmnd'


def parse_args():

    parser = argparse.ArgumentParser(
        description='',
        formatter_class=get_argparser_formatter(),
        add_help=False)

    parser.add_argument('--run-number', action='store',
                        dest='run_number', required=False,
                        type=int, default=0,
                        help="The run number.")
    parser.add_argument('--output-level', action='store',
                        dest='output_level', required=False,
                        type=str, default="INFO",
                        help="The output level messenger.")
    parser.add_argument('--eta-min', action='store',
                        dest='eta_min', required=False,
                        type=float, default=0.0,
                        help="The eta min used in generator.")
    parser.add_argument('--eta-max', action='store',
                        dest='eta_max', required=False,
                        type=float, default=3.2,
                        help="The eta max used in generator.")
    parser.add_argument('--energy-min', action='store', 
                        dest='energy_min', required = False, 
                        type=float, default=17,
                        help = "Minimum energy")
    parser.add_argument('--energy-max', action='store', 
                        dest='energy_max', required = False, 
                        type=float, default=6500, 
                        help = "Maximum energy")
    parser.add_argument('--jf17-file', action='store',
                        dest='jf17_file', required=False,
                        type=str, default=JF17_FILE,
                        help="The pythia JF17 file configuration.")
 

    return merge_args(parser)


def main(events: List[int],
         logging_level: str,
         output_file: str,
         run_number: int,
         seed: int,
         jf17_file: str,
         eta_min: float,
         eta_max: float,
         energy_min: float,
         energy_max: float
        ):

    print(seed)
    outputLevel = LoggingLevel.toC(logging_level)

    tape = EventTape("EventTape", OutputFile=output_file,
                     RunNumber=run_number)

    
    jets = JF17( "JF17",
                 #SherpaGun("Generator", File=main_file, Seed=args.seed)
                 Pythia8("Generator", 
                         File=jf17_file, 
                         Seed=seed),
                 EtaMax      = eta_max,
                 EtaMin      = eta_min,
                 MinPt       = energy_min*GeV,
                 MaxPt       = energy_max*GeV,
                 Select      = 2,
                 EtaWindow   = 0.4,
                 PhiWindow   = 0.4,
                 OutputLevel = outputLevel,
                )


    tape += jets
    tape.run(events)



if __name__ == "__main__":
    parser=parse_args()
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)

    args = parser.parse_args()
    if Path(args.output_file).is_dir():
        raise IsADirectoryError(f"Output file '{args.output_file}' was expected to be a file, "
                                 "but it is a directory.")
    
    args = update_args(args)
    pool = create_parallel_job(args)
    pool(main,
        logging_level=args.output_level,
        run_number=args.run_number,
        jf17_file=args.jf17_file,
        eta_min=args.eta_min,
        eta_max=args.eta_max,
        energy_min=args.energy_min,
        energy_max=args.energy_max
    )
      
 