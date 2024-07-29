#!/bin/bash
source setup_envs.sh
cd build
make -j4 
cd ..
source setup.sh
cd production
