#!/bin/bash
POSITIONAL=()
MISSING_ARGS=()
print_usage () {
    echo "lorenzett_run.sh - Script for running Lorenzett through docker in an one-line script"
    echo " "
    echo "./lorenzett_run.sh [arguments]"
    echo " "
    echo "Required arguments:"
    echo "-h, --help                show this message"
    echo "-f, --filter              chooses which type of event you're interested"
    echo "-e, --event               sets the desired number of events"
    echo "-p, --pileup              sets the pileup average"
    echo "-o, --output              sets the output filename"
    echo "--bc_id_start             sets bc_id_start"
    echo "--bc_id_end               sets bc_id_end"
    echo " "
    echo "Optional arguments:"
    echo "-r, --reco                chooses recovery script"
    echo " "
    exit 0
}

print_missing () {
    echo " "
    echo "Missing arguments: ${MISSING_ARGS[@]}"
    echo " "
    echo "Usage: "
    print_usage
    exit 0
}

if [ -z "$1" ]
then
print_usage
fi

while [[ $# -gt 0 ]]
do
key="$1"

RECO_SCRIPT="default"

case $key in
    -h|--help)
    print_usage
    ;;
    -f|--filter)
    FILTER="$2"
    shift # past argument
    shift # past value
    ;;
    -e|--event)
    EVENT="$2"
    shift # past argument
    shift # past value
    ;;
    -p|--pileup)
    PILEUP="$2"
    shift # past argument
    shift # past value
    ;;
    -o|--output)
    OUTPUT="$2"
    shift # past argument
    shift # past value
    ;;
    --bc_id_start)
    BC_START="$2"
    shift # past argument
    shift # past value
    ;;
    --bc_id_end)
    BC_END="$2"
    shift # past argument
    shift # past value
    ;;
    -r|--reco)
    RECO_SCRIPT="$2"
    shift # past argument
    shift # past value
    ;;
    *)    # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

# Check for mandatory arguments
if [ -z "${FILTER}" ]
then
MISSING_ARGS+=("--filter")
fi
if [ -z "${EVENT}" ]
then
MISSING_ARGS+=("--event")
fi
if [ -z "${PILEUP}" ]
then
MISSING_ARGS+=("--pileup")
fi
if [ -z "${OUTPUT}" ]
then
MISSING_ARGS+=("--output")
fi
if [ -z "${BC_START}" ]
then
MISSING_ARGS+=("--bc_id_start")
fi
if [ -z "${BC_END}" ]
then
MISSING_ARGS+=("--bc_id_end")
fi
if  [ ! -z "$MISSING_ARGS" ]
then
print_missing
fi

# Parse filter
case ${FILTER} in
    "Zee")
    FILTER="Zee"
    CONFIG="/code/lorenzett/generator/PythiaGenerator/data/zee_config.cmnd"
    ;;
    "JF17")
    FILTER="JF17"
    CONFIG="/code/lorenzett/generator/PythiaGenerator/data/jet_config.cmnd"
    ;;
    *)
    echo "Unknown option for --event, please choose one of the following: Zee, JF17"
    exit 0
    ;;
esac

case ${RECO_SCRIPT} in
    "default")
    RECO_SCRIPT="reco_trf.py"
    ;;
    "digit")
    RECO_SCRIPT="reco_trf.py"
    ;;
    *)
    echo "Unknown option for --event, please choose one of the following: Zee, JF17"
    exit 0
    ;;
esac


# Run commands
CPU_N=$(grep -c ^processor /proc/cpuinfo)
source setup_envs.sh
prun_job.py -c "generator.py --filter $FILTER -i $CONFIG --outputLevel 6 --seed 0 --evt ${EVENT} --pileupAvg ${PILEUP} --bc_id_start ${BC_START} --bc_id_end ${BC_END}" -mt $CPU_N -n 10 -o /output/generator_${OUTPUT}
$RECO_SCRIPT -i /output/generator_${OUTPUT} --outputLevel 6 -nt $CPU_N -o /output/reco_${OUTPUT}
