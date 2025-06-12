#!/usr/bin/env python3
import argparse
import sys
import os

from pathlib            import Path
from typing             import List
from expand_folders     import expand_folders
from CaloCellBuilder    import CaloCellBuilder
from ATLAS              import ATLASConstruction as ATLAS
from GaugiKernel        import LoggingLevel, get_argparser_formatter
from GaugiKernel        import ComponentAccumulator
from RootStreamBuilder  import RootStreamHITReader, recordable
from RootStreamBuilder  import RootStreamESDMaker

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

    parser = merge_args(parser)

    return parser


def main(events : List[int],
         logging_level: str,
         input_file: str | Path,
         output_file: str | Path,
         command: str,
        ):

    if isinstance(input_file, Path):
        input_file = str(input_file)
    if isinstance(output_file, Path):
        output_file = str(output_file)

    outputLevel = LoggingLevel.toC(logging_level)
    exec(command)

    acc = ComponentAccumulator("ComponentAccumulator", output_file)

    # the reader must be first in sequence
    reader = RootStreamHITReader("HITReader",
                                 InputFile=input_file,
                                 OutputHitsKey=recordable("Hits"),
                                 OutputEventKey=recordable("Events"),
                                 OutputTruthKey=recordable("Particles"),
                                 OutputSeedsKey=recordable("Seeds"),
                                 OutputLevel=outputLevel,
                                 )

    reader.merge(acc)

    # digitalization!

    calorimeter = CaloCellBuilder("CaloCellBuilder", ATLAS(),
                                  HistogramPath="Expert/Cells",
                                  OutputLevel=outputLevel,
                                  InputHitsKey=recordable("Hits"),
                                  OutputCellsKey=recordable("Cells"),
                                  OutputTruthCellsKey=recordable("TruthCells"),
    )
    calorimeter.merge(acc)

    ESD = RootStreamESDMaker("RootStreamESDMaker",
                             InputCellsKey=recordable("Cells"),
                             InputEventKey=recordable("Events"),
                             InputTruthKey=recordable("Particles"),
                             InputSeedsKey=recordable("Seeds"),
                             OutputLevel=outputLevel)
    acc += ESD

    acc.run(events)



    


if __name__ == "__main__":
    parser=parse_args()
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)
    args = parser.parse_args()
    args = update_args(args)
    pool  = create_parallel_job(args)
    pool( main, 
         logging_level    = args.output_level,
         command          = args.command
         )
