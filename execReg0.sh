#!/bin/bash
echo "inside jobsubmit"
source /cvmfs/cms.cern.ch/cmsset_default.sh 
echo "sourced"
pwd
export SCRAM_ARCH=slc7_amd64_gcc630
cd /afs/cern.ch/work/e/eranken/private/space/CMSSW_9_4_13/src/
eval `scramv1 runtime -sh`
#cmsenv
cd ${_CONDOR_SCRATCH_DIR}
pwd
ls
mkdir weights
hostname
root -l -b -q 'regtest.C("M",0,5000000)'
cp dataset/weights/* /afs/cern.ch/work/e/eranken/private/space/CMSSW_9_4_13/src/ForwardPhotonID/CondorOut/weights/
cp *.root /afs/cern.ch/work/e/eranken/private/space/CMSSW_9_4_13/src/ForwardPhotonID/CondorOut/
#cp -r dataset/weights/*.C CondorOut/
