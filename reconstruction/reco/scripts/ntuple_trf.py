#!/usr/bin/env python3
import argparse
import sys
import os

from pathlib            import Path
from joblib             import Parallel, delayed
from expand_folders     import expand_folders
from GaugiKernel        import LoggingLevel, get_argparser_formatter
from GaugiKernel        import ComponentAccumulator
from RootStreamBuilder  import RootStreamAODReader, recordable
from GaugiKernel        import ComponentAccumulator
from RootStreamBuilder  import RootStreamNtupleMaker 

from reco import merge_args, update_args, merge, get_input_output_job_pairs

def parse_args():
    # create the top-level parser
    parser = argparse.ArgumentParser(
        description='',
        formatter_class=get_argparser_formatter(),
        add_help=False)

    parser.add_argument('-i', '--input-file', action='store',
                        dest='input_file', required=False,
                        help="The event input file generated by the"
                        " Pythia event generator.")
    parser.add_argument('-o', '--output-file', action='store',
                        dest='output_file', required=False,
                        help="The reconstructed event file generated"
                        " by lzt/geant4 framework.")
    parser.add_argument('--nov', '--number-of-events', action='store',
                        dest='number_of_events', required=False,
                        type=int, default=-1,
                        help="The number of events to apply the"
                        " reconstruction.")
    parser.add_argument('-l', '--output-level', action='store',
                        dest='output_level', required=False,
                        type=str, default='INFO',
                        help="The output level messenger.")
    parser.add_argument('-nt', '--number-of-threads', action='store',
                        dest='number_of_threads', required=False,
                        type=int, default=1,
                        help="The number of threads")
    parser.add_argument('-m','--merge', action='store_true',
                        dest='merge', required=False,
                        help='Merge all files.')
    parser = merge_args(parser)

    return parser


def main(logging_level: str,
         input_file: str | Path,
         output_file: str | Path,
         number_of_events: int):

    if isinstance(input_file, Path):
        input_file = str(input_file)
    if isinstance(output_file, Path):
        output_file = str(output_file)

    outputLevel = LoggingLevel.toC(logging_level)

  
    acc = ComponentAccumulator("ComponentAccumulator", output_file)


  
    aod = RootStreamAODReader("AODReader", 
                              InputFile            = input_file,
                              OutputLevel          = outputLevel,
                              OutputEventKey       = recordable("Events"),
                              OutputTruthKey       = recordable("Particles"),
                              OutputClusterKey     = recordable("Clusters"),
                              OutputRingerKey      = recordable("Rings"),
                              OutputElectronKey    = recordable("Electrons"),
                              OutputSeedsKey       = recordable("Seeds"),
                              NtupleName           = "CollectionTree",
                              )

    aod.merge(acc)

    ntuple = RootStreamNtupleMaker("NtupleMaker",
                                  OutputLevel          = outputLevel,
                                  InputEventKey        = recordable("Events"),
                                  InputTruthKey        = recordable("Particles"),
                                  InputClusterKey      = recordable("Clusters"),
                                  InputRingerKey       = recordable("Rings"),
                                  InputElectronKey     = recordable("Electrons"),
                                  InputSeedsKey        = recordable("Seeds"),
                                  OutputNtupleName     = "physics",
                                  )

    acc+=ntuple

    acc.run(number_of_events)


def run(args):

    args.input_file = Path(args.input_file)
    if not args.input_file.exists():
        raise FileNotFoundError(f"Input file {args.input_file} not found.")
    if args.input_file.is_dir():
        args.input_file = expand_folders(os.path.abspath(args.input_file))
    else:
        args.input_file = [args.input_file]

    pool = Parallel(n_jobs=args.number_of_threads)
    pool(delayed(main)(
            logging_level=args.output_level,
            input_file=input_file,
            output_file=output_file,
            number_of_events=args.number_of_events
    )
        for input_file, output_file in get_input_output_job_pairs(args))
    
    files = [f"{os.getcwd()}/{f}" for _, f in get_input_output_job_pairs(args, force=True)]
    if args.merge:
        merge(args, files)
       

if __name__ == "__main__":
    parser=parse_args()
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)
    args = parser.parse_args()
    args = update_args(args)

    run(args)
