#!/usr/bin/env python3

import argparse
import sys
import os

from math        import ceil
from typing      import List
from joblib      import Parallel, delayed
from evtgen      import Pythia8
from filters     import Particle, SingleParticle
from GenKernel   import EventTape
from GaugiKernel import get_argparser_formatter
from GaugiKernel import LoggingLevel
from GaugiKernel import GeV

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
    parser.add_argument('-p', '--particle',action='store', 
                        dest='particle', required = True, type=str,
                        help = f"The single particle type: {Particle.stringList()}")
    parser.add_argument('--energy-min', action='store', 
                        dest='energy_min', required = False, 
                        type=float, default=-1,
                        help = "Energy min in GeV.")
    parser.add_argument('--energy-max', action='store', 
                        dest='energy_max', required = False, 
                        type=float, default=-1,
                        help = "Energy max in GeV.")
    parser.add_argument('--energy', action='store', 
                        dest='energy', required = False, 
                        type=float, default=-1,
                        help = "Energy in GeV.")
    parser.add_argument('--eta', action='store', 
                        dest='eta', required = False, 
                        type=float, default=0.00,
                        help = "Eta position.")
    parser.add_argument('--phi', action='store', 
                        dest='phi', required = False, 
                        type=float, default=1.52,
                        help = "Phi position.")
    parser.add_argument('--do-eta-ranged', action='store', 
                        dest='do_eta_ranged', required = False, 
                        type=bool, default=False,
                        help = "Enable eta range.")
    parser.add_argument('--eta-min', action='store', 
                        dest='eta_min', required = False, 
                        type=float, default=-2.5,
                        help ="Minimum Eta.")
    parser.add_argument('--eta-max', action='store', 
                        dest='eta_max', required = False, 
                        type=float, default=2.5,
                        help = "Maximum Eta.")
    parser.add_argument('--do-phi-ranged', action='store', 
                        dest='do_phi_ranged', required = False, 
                        type=bool, default=False,
                        help = "Enable phi range.")
    parser.add_argument('--phi-min', action='store', 
                        dest='phi_min', required = False, 
                        type=float, default=-3.14,
                        help = "Minimum Phi.")
    parser.add_argument('--phi-max', action='store', 
                        dest='phi_max', required = False, 
                        type=float, default=3.14,
                        help = "Maximum Phi.")
    
    return merge_args(parser)


def main(events: List[int],
         logging_level: str,
         output_file: str,
         run_number: int,
         seed: int,
         particle: str,
         eta: float,
         phi: float, 
         eta_min: float,
         eta_max: float,
         phi_min: float,
         phi_max: float,
         energy: float,
         energy_min:float,
         energy_max:float,
         do_eta_ranged: bool,
         do_phi_ranged: bool):

    outputLevel = LoggingLevel.toC(logging_level)

    tape = EventTape("EventTape", OutputFile=output_file,
                     RunNumber=run_number)

  
    part = SingleParticle( particle,
                               Pythia8("Generator", Seed=seed),
                               Eta          = eta,
                               Phi          = phi,
                               EnergyMin    = energy_min*GeV,
                               EnergyMax    = energy_max*GeV,
                               Energy       = energy*GeV,
                               Particle     = Particle.fromstring(particle), 
                               DoRangedEta  = do_eta_ranged,
                               EtaMin       = eta_min,
                               EtaMax       = eta_max,
                               DoRangedPhi  = do_phi_ranged,
                               PhiMin       = phi_min,
                               PhiMax       = phi_max,
                               OutputLevel  = outputLevel)

    tape += part
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
        particle=args.particle,
        eta=args.eta,
        phi=args.phi, 
        eta_min=args.eta_min,
        eta_max=args.eta_max,
        phi_min=args.phi_min,
        phi_max=args.phi_max,
        energy=args.energy,
        energy_min=args.energy_min,
        energy_max=args.energy_max,
        do_eta_ranged=args.do_eta_ranged,
        do_phi_ranged=args.do_phi_ranged,
    )