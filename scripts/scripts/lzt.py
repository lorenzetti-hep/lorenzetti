#!/usr/bin/env python

import os
import sys
import argparse

from pydoc       import importfile
from GaugiKernel import get_argparser_formatter

basepath           = os.environ["LORENZETTI_SCRIPTS_DIR"]
simu_trf           = importfile(f'{basepath}/reco/simu_trf.py')
digit_trf          = importfile(f'{basepath}/reco/digit_trf.py')
merge_trf          = importfile(f'{basepath}/reco/merge_trf.py')
reco_trf           = importfile(f'{basepath}/reco/reco_trf.py')
ntuple_trf         = importfile(f'{basepath}/reco/ntuple_trf.py')
gen_zee            = importfile(f'{basepath}/filters/gen_zee.py')
gen_jets           = importfile(f'{basepath}/filters/gen_jets.py')
gen_single         = importfile(f'{basepath}/filters/gen_single.py')
gen_minbias        = importfile(f'{basepath}/filters/gen_minbias.py')
gen_overlapped_zee = importfile(f'{basepath}/filters/gen_overlapped_zee.py')


def build_argparser():

    formatter_class = get_argparser_formatter()

    parser    = argparse.ArgumentParser(formatter_class=formatter_class)
    mode      = parser.add_subparsers(dest='mode')


    run_parent = argparse.ArgumentParser(formatter_class=formatter_class, add_help=False, )
    option = run_parent.add_subparsers(dest='option')
    option.add_parser("simu"    , parents = [simu_trf.parse_args()   ], help='Run the transformation from EVT to HIT.',formatter_class=formatter_class)
    option.add_parser("digit"   , parents = [digit_trf.parse_args()  ], help='Run the transformation from HIT to ESD',formatter_class=formatter_class)
    option.add_parser("merge"   , parents = [merge_trf.parse_args()  ], help='Merge minimum bias HIT events into the main HIT events.',formatter_class=formatter_class)
    option.add_parser("reco"    , parents = [reco_trf.parse_args()   ], help='Run the transformation from ESD to AOD',formatter_class=formatter_class)
    option.add_parser("ntuple"  , parents = [ntuple_trf.parse_args() ], help='Run the transformation from AOD to NTUPLE',formatter_class=formatter_class)
    mode.add_parser( "trf", parents=[run_parent], help="",formatter_class=formatter_class)
    

    gen_parent = argparse.ArgumentParser(formatter_class=formatter_class, add_help=False)
    option = gen_parent.add_subparsers(dest='option')
    option.add_parser("zee"         , parents = [gen_zee.parse_args()               ], help='Generate Zee events decay',formatter_class=formatter_class)
    option.add_parser("jets"        , parents = [gen_jets.parse_args()              ], help='Generate JF17 events',formatter_class=formatter_class)
    option.add_parser("single"      , parents = [gen_single.parse_args()            ], help='Generate single particle events',formatter_class=formatter_class)
    option.add_parser("minbias"     , parents = [gen_minbias.parse_args()           ], help='Generate minimum bias events',formatter_class=formatter_class)
    option.add_parser("zee_boosted" , parents = [gen_overlapped_zee.parse_args()    ], help='Generate Zee events with an overlapped electron event.',formatter_class=formatter_class)
    mode.add_parser( "gen", parents=[gen_parent], help="",formatter_class=formatter_class)


    return parser

def run_parser(args):
    if args.mode == "trf":
        if args.option == "simu":
            simu_trf.run(args)
        elif args.option == "digit":
            digit_trf.run(args)
        elif args.option == "merge":
            merge_trf.run(args)        
        elif args.option == "reco":
            reco_trf.run(args)   
        elif args.option == "ntuple":
            ntuple_trf.run(args)    
    elif args.mode == "gen":
        if args.option == "zee":
            gen_zee.run(args)
        elif args.option == "jets":
            gen_jets.run(args)
        elif args.option == "single":
            gen_single.run(args)        
        elif args.option == "minbias":
            gen_minbias.run(args)   
        elif args.option == "zee_boosted":
            gen_overlapped_zee.run(args)    

def run():
    parser = build_argparser()
    if len(sys.argv)==1:
        print(parser.print_help())
        sys.exit(1)
    args = parser.parse_args()
    run_parser(args)



if __name__ == "__main__":
  run()