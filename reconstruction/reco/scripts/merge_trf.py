#!/usr/bin/env python3
import argparse
import sys
import os

from pathlib            import Path
from typing             import List
from expand_folders     import expand_folders
from GaugiKernel        import LoggingLevel, get_argparser_formatter
from GaugiKernel        import ComponentAccumulator
from RootStreamBuilder  import RootStreamHITReader, recordable
from CaloCellBuilder    import PileupMerge
from RootStreamBuilder  import RootStreamHITMaker

from reco.reco_job import merge_args, update_args, create_parallel_job



def parse_args():
    # create the top-level parser
    parser = argparse.ArgumentParser(
        description='',
        formatter_class=get_argparser_formatter(),
        add_help=False)

    parser.add_argument('-l', '--output-level', action='store',
                        dest='output_level', required=False,
                        type=str, default='INFO',
                        help="The output level messenger.")
    parser.add_argument('-c', '--command', action='store',
                        dest='command', required=False, default="''",
                        help="The preexec command")
    parser.add_argument('--low-pileup-files', action='store', 
                        dest='low_pileup_files', required = True,
                        help = "The event HIT file to be merged (pileup).")
    parser.add_argument('--high-pileup-files', action='store', 
                        dest='high_pileup_files', required = True,
                        help = "The event HIT file to be merged (pileup).")
    parser.add_argument('--pileup-avg', action='store',
                        dest='pileup_avg', required=True,
                        type=int,
                        help="The pileup average.")
    parser.add_argument('--pileup-sigma', action='store',
                        dest='pileup_sigma', required=True,
                        type=int,
                        help="The pileup sigma.")

    return merge_args(parser)


def main(events: List[int],
         input_file: str | Path,
         output_file: str | Path,
         logging_level: str,
         low_pileup_files: List[str],
         high_pileup_files: List[str],
         pileup_avg : int,
         pileup_sigma : int,
         command: str):

    if isinstance(input_file, Path):
        input_file = str(input_file)
    if isinstance(output_file, Path):
        output_file = str(output_file)

    outputLevel = LoggingLevel.toC(logging_level)

    exec(command)

    acc = ComponentAccumulator("ComponentAccumulator", output_file)

    reader = RootStreamHITReader("HITReader", 
                                  InputFile       = input_file,
                                  OutputHitsKey   = recordable("Hits"),
                                  OutputEventKey  = recordable("Events"),
                                  OutputTruthKey  = recordable("Particles"),
                                  OutputSeedsKey  = recordable("Seeds"),
                                  OutputLevel     = outputLevel,
                                )
    reader.merge(acc)

    pileup = PileupMerge( "PileupMerge", 
                          LowPileupInputFiles = low_pileup_files,
                          HighPileupInputFiles= high_pileup_files,
                          PileupAvg           = pileup_avg,
                          PileupSigma         = pileup_sigma,
                          InputHitsKey        = recordable("Hits"),
                          InputEventKey       = recordable("Events"),
                          OutputHitsKey       = "Hits_Merged",
                          OutputEventKey      = "Events_Merged",
                          OutputLevel         = outputLevel
                        )
    acc += pileup

    HIT = RootStreamHITMaker( "RootStreamHITMaker",
                               # input from context
                               InputHitsKey    = "Hits_Merged",
                               InputEventKey   = "Events_Merged",
                               InputTruthKey   = recordable("Particles"),
                               InputSeedsKey   = recordable("Seeds"),
                               # output to file
                               OutputHitsKey   = recordable("Hits"),
                               OutputEventKey  = recordable("Events"),
                               OutputLevel     = outputLevel)
    acc += HIT
    acc.run(events)



def run(args):

    args.low_pileup_files  = Path(args.low_pileup_files)
    args.high_pileup_files = Path(args.high_pileup_files)

    if not args.low_pileup_files.exists():
        raise FileNotFoundError(f"Low Pileup input files {args.low_pileup_files} not found.")
    if args.low_pileup_files.is_dir():
        args.low_pileup_files = expand_folders(os.path.abspath(args.low_pileup_files))
    else:
        args.low_pileup_files = [args.low_pileup_files]
   
    if not args.high_pileup_files.exists():
        raise FileNotFoundError(f"High Pileup input files {args.high_pileup_files} not found.")
    if args.high_pileup_files.is_dir():
        args.high_pileup_files = expand_folders(os.path.abspath(args.high_pileup_files))
    else:
        args.high_pileup_files = [args.high_pileup_files]

    pool = create_parallel_job(args)
    pool( main, 
         logging_level    = args.output_level,
         low_pileup_files = args.low_pileup_files,
         high_pileup_files= args.high_pileup_files,
         pileup_avg       = args.pileup_avg,
         pileup_sigma     = args.pileup_sigma,
         command          = args.command
         )
    
   



if __name__ == "__main__":
    parser=parse_args()
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)
    args = parser.parse_args()
    args = update_args(args)
    run(args)
