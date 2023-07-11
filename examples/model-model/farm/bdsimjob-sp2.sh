#!/bin/sh

source /cvmfs/beam-physics.cern.ch/bdsim/x86_64-centos7-gcc11-opt/bdsim-env-v1.7.0-rc-g4v10.7.2.3-ftfp-boost.sh

# echo inputs
echo $1
echo $2
echo $3
echo $4
echo $5
echo $6
INPUTFILE=$1
OUTPUTDIR=$2
SEEDVALUE=$3
ANALFILE=$4
NGEN=$5
echo "input file    = "${INPUTFILE}
echo "output dir    = "${OUTPUTDIR}
echo "analysis file = "${ANALFILE}
echo "seed value    = "${SEEDVALUE}
echo "ngenerate     = "${NGEN}
echo "skim file     = "${SKIMFILE}

date
echo ""
hostname
echo ""

# run bdsim
bdsim  --file=${INPUTFILE} --outfile=${SEEDVALUE}_raw --batch --seed=${SEEDVALUE} --ngenerate=${NGEN}
echo "job finished"
date

# run rebdsim analysis on output from bdsim
rebdsim ${ANALFILE} ${SEEDVALUE}_raw.root ${SEEDVALUE}_ana.root

# copy generated output to desired output directory
mkdir -p ${OUTPUTDIR}
cp *.root ${OUTPUTDIR}/.

# clean up
rm *.root

