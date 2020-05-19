#!/bin/bash
python3 /parser.py $@

if [ $? -eq 0 ]
then
  echo "Successfully executed script"
  source setup_envs.sh
  source /command.sh
else
  # Redirect stdout from echo command to stderr.
  echo "Script exited with error." >&2
fi




