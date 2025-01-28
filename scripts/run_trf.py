#!/usr/bin/env python3

import argparse
import numpy as np
import multiprocessing
import sys,os,traceback,json
from GaugiKernel import get_argparser_formatter, chunks
from GaugiKernel import LoggingLevel, Logger



from GaugiKernel import get_argparser_formatter, chunks
from GaugiKernel import Logger



def build_argparser_create_jobs():
    parser = argparse.ArgumentParser(description = '', formatter_class=get_argparser_formatter() , add_help=False)

    parser.add_argument('--production-card','-p', action='store', dest='production_card', 
                        required = False, type=str, default=None,
                        help = "The production card")
    
    parser.add_argument('--output','-o', action='store', dest='output',
                        required = False, type=str, default=f'jobs',
                        help = "The production card")
    return parser

def run_create_jobs(args):
    prod = json.load(open(args.production_card,'r'))
    chunk_size = prod["run"]["nov_per_job"]
    seed = prod["run"]["seed"]
    os.makedirs(args.output, exist_ok=True)
    for idx, evts in enumerate(chunks(np.arange( 0, prod["run"]["nov"]).tolist(), chunk_size)):
        with open( f"{args.output}/job.{idx}.json", 'w') as f:
            d = prod | {"job":{"event_numbers":evts, "seed":seed*(idx+1),"job_id":idx}}
            json.dump(d,f ,indent = 4)





def build_argparser():

    from digit_trf import build_argparser as digit_parser
    from merge_trf import build_argparser as merge_parser
    from reco_trf  import build_argparser as reco_parser
    from simu_trf  import build_argparser as simu_parser
    
    formatter_class = get_argparser_formatter()
    
    parser    = argparse.ArgumentParser(formatter_class=formatter_class)
    mode = parser.add_subparsers(dest='mode')


    run_parent = argparse.ArgumentParser(formatter_class=formatter_class, add_help=False, )
    option = run_parent.add_subparsers(dest='option')
    option.add_parser("simu"   , parents = [simu_parser()]   ,help='Run as runner',formatter_class=formatter_class)
    option.add_parser("digit"  , parents = [digit_parser()]  ,help='Run as runner',formatter_class=formatter_class)
    option.add_parser("merge"  , parents = [merge_parser()]  ,help='Run as runner',formatter_class=formatter_class)
    option.add_parser("reco"   , parents = [reco_parser()]   ,help='Run as runner',formatter_class=formatter_class) 
    mode.add_parser( "run", parents=[run_parent], help="",formatter_class=formatter_class)
    
    job_parent = argparse.ArgumentParser(formatter_class=formatter_class, add_help=False, )
    option = job_parent.add_subparsers(dest='option')
    option.add_parser("create", parents = [build_argparser_create_jobs()]   ,help='Run as runner',formatter_class=formatter_class)
    mode.add_parser( "job", parents=[job_parent], help="",formatter_class=formatter_class)
    return parser
    
    
    
    
def run_parser(args):
    if args.mode == "run":
        
        if args.option == "simu":
            from simu_trf import run
            run(args)
        elif args.option == "digit":
            from digit_trf import run
            run(args)
        elif args.option == "merge":
            from merge_trf import run
            run(args)
        elif args.option == "reco":
            from reco_trf import run
            run(args)
        else:
            print("Option not implemented")
    elif args.mode == "job":
        if args.option == "create":
            run_create_jobs(args)
        else:
            print("Option not implemented")
  

def run():
    parser = build_argparser()
    if len(sys.argv)==1:
        print(parser.print_help())
        sys.exit(1)

    args = parser.parse_args()
    run_parser(args)

        

if __name__ == "__main__":
  run()