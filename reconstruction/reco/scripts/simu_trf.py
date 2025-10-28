#!/usr/bin/env python3

import argparse
import sys
import traceback
import multiprocessing

from multiprocessing        import Process
from pathlib                import Path
from GaugiKernel.constants  import MINUTES
from GaugiKernel            import LoggingLevel, get_argparser_formatter
from G4Kernel               import ComponentAccumulator, EventReader
from RootStreamBuilder      import recordable
from ATLAS                  import ATLASConstruction as ATLAS
from CaloCellBuilder        import CaloHitBuilder
from RootStreamBuilder      import RootStreamHITMaker

from reco import merge_args, update_args


def parse_args():
    parser = argparse.ArgumentParser(
        description='',
        add_help=False,
        formatter_class=get_argparser_formatter())

    parser.add_argument('--enable-magnetic-field', action='store_true',
                        dest='enable_magnetic_field', required=False,
                        help="Enable the magnetic field.")
    parser.add_argument('-t', '--timeout', action='store',
                        dest='timeout', required=False, type=int, default=120,
                        help="Event timeout in minutes")
    parser.add_argument('-l', '--output-level', action='store',
                        dest='output_level', required=False,
                        type=str, default='INFO',
                        help="The output level messenger.")
    parser.add_argument('-c', '--command', action='store',
                        dest='command', required=False, default="''",
                        help="The preexec command")
    parser.add_argument('--save-all-hits', action='store_true',
                        dest='save_all_hits', required=False,
                        help="Save all hits into the output file.")

    return merge_args(parser)


def main(logging_level: str,
         input_file: str | Path,
         output_file: str | Path,
         command: str,
         enable_magnetic_field: bool,
         save_all_hits : bool,
         timeout: int,
         number_of_events: int,
         number_of_threads: int):

    if isinstance(input_file, Path):
        input_file = str(input_file)
    if isinstance(output_file, Path):
        output_file = str(output_file)

    print(f"number of threads: {number_of_threads}")
    exec(command)
    outputLevel = LoggingLevel.toC(logging_level)

    # Build the ATLAS detector
    detector = ATLAS(UseMagneticField=enable_magnetic_field)

    acc = ComponentAccumulator("ComponentAccumulator", detector,
                               NumberOfThreads=number_of_threads,
                               OutputFile=output_file,
                               Timeout=timeout * MINUTES)

    gun = EventReader("EventReader", input_file,
                      # outputs
                      OutputEventKey=recordable("Events"),
                      OutputTruthKey=recordable("Particles"),
                      OutputSeedKey=recordable("Seeds"),
                      )

    calorimeter = CaloHitBuilder("CaloHitBuilder",
                                 HistogramPath="Expert/Hits",
                                 OutputLevel=outputLevel,
                                 InputEventKey=recordable("Events"),
                                 OutputHitsKey=recordable("Hits")
                                 )
    gun.merge(acc)
    calorimeter.merge(acc)

    HIT = RootStreamHITMaker("RootStreamHITMaker",
                             OutputLevel=outputLevel,
                             OnlyRoI= not save_all_hits,
                             # input from context
                             InputHitsKey=recordable("Hits"),
                             InputEventKey=recordable("Events"),
                             InputTruthKey=recordable("Particles"),
                             InputSeedsKey=recordable("Seeds"),
                             )
    acc += HIT
    acc.run(number_of_events)





def run(args):

    if isinstance(args.input_file, list) and not isinstance(args.input_file[0], Path):
        args.input_file = [Path(inp) for inp in args.input_file if inp.endswith('.root')]

    if not all(inp.exists() for inp in args.input_file):
        raise FileNotFoundError(f"Input file {args.input_file} not found.")

    splitted_output_filename = args.output_file.split(".")
    for i, input_file in enumerate(args.input_file):
        output_file = splitted_output_filename.copy()
        if len(args.input_file) > 1:
            output_file.insert(-1, str(i))
        output_file = Path('.'.join(output_file))
        if output_file.exists():
            print(f"{i} - Output file {output_file} already exists. Skipping.")
            continue

        kwargs ={
            'logging_level'         : args.output_level,
            'input_file'            : input_file,
            'output_file'           : output_file,
            'command'               : args.command,
            'enable_magnetic_field' : args.enable_magnetic_field,
            'save_all_hits'         : args.save_all_hits,
            'timeout'               : args.timeout,
            'number_of_events'      : args.number_of_events,
            'number_of_threads'     : args.number_of_threads
        }

        def run_proc(kwargs):
            try:
                main(**kwargs)
                sys.exit(0)
            except Exception:
                traceback.format_exc()
                sys.exit(1)

        # NOTE: Run this in a separated process to avoid geant segmentation fault
        proc = Process(target=run_proc, args=(kwargs,))   
        proc.start()
        proc.join()
        if proc.exitcode==1:
            break

  



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
    run(args)
