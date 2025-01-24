#!/usr/bin/env python3

import argparse
import numpy as np
import sys,os,json
from GaugiKernel import get_argparser_formatter, chunks
from GaugiKernel import Logger


    
def run():
    
    mainLogger = Logger.getModuleLogger("zee")
    parser = argparse.ArgumentParser(description = '', formatter_class=get_argparser_formatter() )

    parser.add_argument('--production-card','-p', action='store', dest='production_card', 
                        required = False, type=str, default=None,
                        help = "The production card")
    
    parser.add_argument('--output','-o', action='store', dest='output',
                        required = False, type=str, default=f'jobs',
                        help = "The production card")

    if len(sys.argv)==1:
      parser.print_help()
      sys.exit(1)

    args = parser.parse_args()
    prod = json.load(open(args.production_card,'r'))
    chunk_size = prod["run"]["nov_per_job"]
    seed = prod["run"]["seed"]
    os.makedirs(args.output, exist_ok=True)
    for idx, evts in enumerate(chunks(np.arange( 0, prod["run"]["nov"]).tolist(), chunk_size)):
        with open( f"{args.output}/job.{idx}.json", 'w') as f:
            d = prod | {"job":{"event_numbers":evts, "seed":seed*(idx+1),"job_id":idx}}
            json.dump(d,f ,indent = 4)
        
if __name__ == "__main__":
    run()