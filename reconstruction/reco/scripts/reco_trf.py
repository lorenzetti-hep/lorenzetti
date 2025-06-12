#!/usr/bin/env python3
import argparse
import sys
import os

from pathlib            import Path
from typing             import List
from expand_folders     import expand_folders
from GaugiKernel        import LoggingLevel, get_argparser_formatter
from GaugiKernel        import ComponentAccumulator
from RootStreamBuilder  import RootStreamESDReader, recordable
from CaloClusterBuilder import CaloClusterMaker
from CaloRingsBuilder   import CaloRingsBuilderCfg
from EgammaBuilder      import ElectronBuilderCfg
from RootStreamBuilder  import RootStreamAODMaker

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
   
    return merge_args(parser)


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


    ESD = RootStreamESDReader("ESDReader", 
                              InputFile       = input_file,
                              OutputCellsKey  = recordable("Cells"),
                              OutputEventKey  = recordable("Events"),
                              OutputTruthKey  = recordable("Particles"),
                              OutputSeedsKey  = recordable("Seeds"),
                              OutputLevel     = outputLevel
                              )
    ESD.merge(acc)


    # build cluster for all seeds
    cluster = CaloClusterMaker( "CaloClusterMaker",
                                InputCellsKey        = recordable("Cells"),
                                InputSeedsKey        = recordable("Seeds"),
                                # output as
                                OutputClusterKey     = recordable("Clusters"),
                                # other configs
                                HistogramPath        = "Expert/Clusters",
                                OutputLevel          = outputLevel )



    rings = CaloRingsBuilderCfg( "CaloRingsBuilder",
                                InputClusterKey    = recordable("Clusters"),
                                OutputRingerKey    = recordable("Rings"),
                                HistogramPath      = "Expert/Rings",
                                OutputLevel        = outputLevel)


    hypo = ElectronBuilderCfg( "ElectronBuilder",
                               InputClusterKey    = recordable("Clusters"),
                               OutputElectronKey  = recordable("Electrons"),
                               OutputLevel        = outputLevel) 



    AOD = RootStreamAODMaker( "RootStreamAODMaker",
                              InputEventKey    = recordable("Events"),
                              InputSeedsKey    = recordable("Seeds"),
                              InputTruthKey    = recordable("Particles"),
                              InputCellsKey    = recordable("Cells"),
                              InputClusterKey  = recordable("Clusters"),
                              InputRingerKey   = recordable("Rings"),
                              InputElectronKey = recordable("Electrons"),
                              OutputLevel      = outputLevel)

    # sequence
    acc+= cluster
    acc+= rings
    acc+= hypo
    acc+= AOD

    acc.run(events)



   

if __name__ == "__main__":
    parser=parse_args()
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(1)
    args = parser.parse_args()
    args = update_args(args)
    pool = create_parallel_job(args)
    pool( main, 
         logging_level    = args.output_level,
         command          = args.command
         )
