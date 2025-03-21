#!/usr/bin/env python3
from GaugiKernel import get_argparser_formatter
from GaugiKernel import LoggingLevel
from GaugiKernel import GeV
from filters import Particle

import argparse
import sys,os,traceback



def build_argparser():

  parser = argparse.ArgumentParser(description = '', formatter_class=get_argparser_formatter(), add_help=False )

  parser.add_argument('-e','--event-numbers', action='store', dest='event_numbers', required = False, type=str, default=None,
                      help = "The event number list separated by ','. e.g. --event-numbers '0,1,2,3'")
  parser.add_argument('-o','--output-file', action='store', dest='output_file', required = True,
                      help = "The event file generated by pythia.")
  parser.add_argument('--nov','--number-of-events', action='store', dest='number_of_events', required = False, type=int, default=1,
                      help = "The number of events to be generated.")
  parser.add_argument('--run-number', action='store', dest='run_number', required = False, type=int, default = 0,
                      help = "The run number.")
  parser.add_argument('-s','--seed', action='store', dest='seed', required = False, type=int, default=0,
                      help = "The pythia seed (zero is the clock system)")
  parser.add_argument('--output-level', action='store', dest='output_level', required = False, type=str, default="INFO",
                      help = "The output level messenger.")
  parser.add_argument('-p', '--particle',action='store', dest='particle', required = True, type=str,
                      help = f"The single particle type: {Particle.stringList()}")
  parser.add_argument('--energy-min', action='store', dest='energy_min', required = False, type=float, default=-1,
                      help = "Energy min in GeV.")
  parser.add_argument('--energy-max', action='store', dest='energy_max', required = False, type=float, default=-1,
                      help = "Energy max in GeV.")
  parser.add_argument('--energy', action='store', dest='energy', required = False, type=float, default=-1,
                      help = "Energy in GeV.")
  parser.add_argument('--eta', action='store', dest='eta', required = False, type=float, default=0.00,
                      help = "Eta position.")
  parser.add_argument('--phi', action='store', dest='phi', required = False, type=float, default=1.52,
                      help = "Phi position.")
  parser.add_argument('--do-eta-ranged', action='store', dest='do_eta_ranged', required = False, type=bool, default=False,
                      help = "Enable eta range.")
  parser.add_argument('--eta-min', action='store', dest='eta_min', required = False, type=float, default=-2.5,
                      help ="Minimum Eta.")
  parser.add_argument('--eta-max', action='store', dest='eta_max', required = False, type=float, default=2.5,
                      help = "Maximum Eta.")
  parser.add_argument('--do-phi-ranged', action='store', dest='do_phi_ranged', required = False, type=bool, default=False,
                      help = "Enable phi range.")
  parser.add_argument('--phi-min', action='store', dest='phi_min', required = False, type=float, default=-3.14,
                      help = "Minimum Phi.")
  parser.add_argument('--phi-max', action='store', dest='phi_max', required = False, type=float, default=3.14,
                      help = "Maximum Phi.")
  parser.add_argument('--pileup-avg', action='store', dest='pileup_avg', required = False, type=int, default=0,
                      help = "The pileup average (default is zero).")
  parser.add_argument('--pileup-sigma', action='store', dest='pileup_sigma', required = False, type=int, default=0,
                      help = "The pileup sigma (default is zero).")
  parser.add_argument('--bc-id-start', action='store', dest='bc_id_start', required = False, type=int, default=-21,
                      help = "The bunch crossing id start.")
  parser.add_argument('--bc-id-end', action='store', dest='bc_id_end', required = False, type=int, default=4,
                      help = "The bunch crossing id end.")
  parser.add_argument('--bc-duration', action='store', dest='bc_duration', required = False, type=int, default=25,
                      help = "The bunch crossing duration (in nanoseconds).")
  return parser



def run(args):

  outputLevel = LoggingLevel.toC(args.output_level)
  try:

    from evtgen import Pythia8 
    from filters import SingleParticle, Particle
    from GenKernel import EventTape

    if not args.particle in Particle.stringList():
        raise RuntimeError("Particle type not supported. Supported particles are: %s" % Particle.stringList())
    
    tape = EventTape( "EventTape", OutputFile = args.output_file, RunNumber=args.run_number )

    single_part_file   = os.environ['LZT_PATH']+'/generator/evtgen/data/single_particle_config.cmnd'

    # Create the seed
    electron = SingleParticle( "Electron",
                               Pythia8("Generator", Seed=args.seed),
                               Eta          = args.eta,
                               Phi          = args.phi,
                               EnergyMin    = args.energy_min*GeV,
                               EnergyMax    = args.energy_max*GeV,
                               Energy       = args.energy*GeV,
                               Particle     = Particle.fromstring(args.particle), 
                               DoRangedEta  = args.do_eta_ranged,
                               EtaMin       = args.eta_min,
                               EtaMax       = args.eta_max,
                               DoRangedPhi  = args.do_phi_ranged,
                               PhiMin       = args.phi_min,
                               PhiMax       = args.phi_max,
                               OutputLevel  = outputLevel)

    tape+=electron

    if args.pileup_avg > 0:

      mb_file   = os.environ['LZT_PATH']+'/generator/evtgen/data/minbias_config.cmnd'

      pileup = Pileup("Pileup",
                     Pythia8("Generator", File=mb_file, Seed=args.seed),
                     EtaMax         = 3.2,
                     Select         = 2,
                     PileupAvg      = args.pileup_avg,
                     BunchIdStart   = args.bc_id_start,
                     BunchIdEnd     = args.bc_id_end,
                     OutputLevel    = outputLevel,
                     DeltaEta       = 0.22,
                     DeltaPhi       = 0.22,
                    )

      tape+=pileup

  
    # Run!
    evts = [int(evt) for evt in args.event_numbers.split(",")] if args.event_numbers else args.number_of_events
    tape.run(evts)

    sys.exit(0)
  except  Exception as e:
    traceback.print_exc()
    sys.exit(1)



if __name__ == "__main__":
  parser = build_argparser()
  if len(sys.argv)==1:
    parser.print_help()
    sys.exit(1)
  args = parser.parse_args()
  run(args)

