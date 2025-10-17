#!/usr/bin/env python3

import argparse
import sys
import os
from math import ceil
from typing import List
from joblib import Parallel, delayed
from evtgen import Pythia8
from filters import Zee, OverlappedEvents, Particle

from GenKernel import EventTape
from GaugiKernel import get_argparser_formatter
from GaugiKernel import LoggingLevel
from GaugiKernel import GeV

from reco.gen_job import merge_args, update_args, create_parallel_job


datapath    = os.environ["LORENZETTI_EVTGEN_DATA_DIR"]
PILEUP_FILE = f'{datapath}/minbias_config.cmnd'
ZEE_FILE    = f'{datapath}/zee_config.cmnd'


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
                        dest='eta_max', required=False,
                        type=float, default=3.2,
                        help="The eta max used in generator.")
    parser.add_argument('--force-forward-electron', action='store_true',
                        dest='force_forward_electron', required=False,
                        help="Force at least one electron "
                        "into forward region.")
    parser.add_argument('--zero-vertex-particles', action='store_true',
                        dest='zero_vertex_particles', required=False,
                        help="Fix the z vertex position in "
                        "simulation to zero for all selected particles. "
                        "It is applied only at G4 step, not in generation.")
    parser.add_argument('--zee-file', action='store',
                        dest='zee_file', required=False,
                        type=str, default=ZEE_FILE,
                        help="The pythia zee file configuration.")
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
         zee_file: str,
         zero_vertex_particles: bool,
         force_forward_electron: bool,
         eta_max: float,
        ):

    outputLevel = LoggingLevel.toC(logging_level)

    tape = EventTape("EventTape", OutputFile=output_file,
                     RunNumber=run_number)

    zee = Zee("Zee",
              Pythia8("Generator",
                      File=zee_file,
                      Seed=seed),
              EtaMax=eta_max,
              MinPt=15*GeV,
              # calibration use only.
              ZeroVertexParticles=zero_vertex_particles,
              ForceForwardElectron=force_forward_electron,
              OutputLevel=outputLevel
              )
    tape += zee

    boostedElectron = OverlappedEvents("BoostedElectron",
                                        Pythia8("BoostedGenerator",
                                                Seed=seed),
                                        Particle=Particle.Electron,
                                        DeltaR=0.5,
                                        OutputLevel=outputLevel
                                        )

    tape += boostedElectron
    tape.run(events)




if __name__ == "__main__":
    parser=parse_args()
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)
    args = parser.parse_args()
    pool = create_parallel_job(args)
    pool(main,
        logging_level=args.output_level,
        run_number=args.run_number,
        zee_file=args.zee_file,
        zero_vertex_particles=args.zero_vertex_particles,
        force_forward_electron=args.force_forward_electron,
        eta_max=args.eta_max,
    )
    

