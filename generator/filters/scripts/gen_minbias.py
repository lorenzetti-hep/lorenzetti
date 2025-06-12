#!/usr/bin/env python3

import argparse
import random
import sys
import os

from math           import ceil
from typing         import List
from joblib         import Parallel, delayed
from evtgen         import Pythia8
from filters        import FixedRegion
from GenKernel      import EventTape
from GaugiKernel    import get_argparser_formatter
from GaugiKernel    import LoggingLevel

from reco.gen_job import merge_args, update_args, create_parallel_job


datapath    = os.environ["LORENZETTI_EVTGEN_DATA_DIR"]
PILEUP_FILE = f'{datapath}/minbias_config.cmnd'

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
    parser.add_argument('--eta-max', action='store', 
                        dest='eta_max', required = False, 
                        type=float, default=3.2,
                        help = "Maximum Eta.")
    parser.add_argument('--pileup-per-bunch-crossing', action='store',
                        dest='pileup_per_bunch_crossing', required=False,
                        type=float, default=-1,
                        help="The fixed number of pileup for each bunch crossing.")
    parser.add_argument('--bc-id-start', action='store',
                        dest='bc_id_start', required=False,
                        type=int, default=-21,
                        help="The bunch crossing id start.")
    parser.add_argument('--bc-id-end', action='store',
                        dest='bc_id_end', required=False,
                        type=int, default=4,
                        help="The bunch crossing id end.")
    parser.add_argument('--bc-duration', action='store',
                        dest='bc_duration', required=False,
                        type=int, default=25,
                        help="The bunch crossing duration (in nanoseconds).")
    parser.add_argument('--pileup-file', action='store',
                        dest='pileup_file', required=False,
                        type=str, default=PILEUP_FILE,
                        help="The pythia pileup file configuration.")
    
    return merge_args(parser)



def main(events: List[int],
         logging_level: str,
         output_file: str,
         run_number: int,
         seed: int,
         eta_max: float,
         pileup_per_bunch_crossing : float,
         mb_file: str,
         bc_id_start: int,
         bc_id_end: int):

    outputLevel = LoggingLevel.toC(logging_level)

    tape = EventTape("EventTape", OutputFile=output_file,
                     RunNumber=run_number)

    # dummy seed to store all pileup particles
    tape+=FixedRegion("Seed", Eta=0, Phi=0 )
  
    from filters import Pileup
    pileup = Pileup("Pileup",
                    Pythia8("MBGenerator", 
                            File=mb_file,
                            Seed=seed),
                    EtaMax=eta_max,
                    Select=2,
                    PileupAvg=0,
                    PileupSigma=0,
                    PileupPerBunch=pileup_per_bunch_crossing,
                    BunchIdStart=bc_id_start,
                    BunchIdEnd=bc_id_end,
                    OutputLevel=outputLevel,
                    DeltaEta=999,
                    DeltaPhi=999,
    )
    tape += pileup
    tape.run(events)



if __name__ == "__main__":
    parser=parse_args()
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)
    args = parser.parse_args()
    args = update_args(args)    
    job  = create_parallel_job(args)
    job(main, 
        logging_level=args.output_level,
        run_number=args.run_number,
        eta_max=args.eta_max,
        pileup_per_bunch_crossing=args.pileup_per_bunch_crossing,
        mb_file=args.pileup_file,
        bc_id_start=args.bc_id_start,
        bc_id_end=args.bc_id_end
    )