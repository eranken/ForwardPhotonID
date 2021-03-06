import sys, os, time, subprocess, re

name = sys.argv[1]
WP = sys.argv[2]
reg = sys.argv[3]
Ntrain = sys.argv[4]

thisdir = os.getcwd()

fullname = name+'_'+WP+reg
namedir = os.path.join(thisdir,'CondorOut',name)
jobdir = os.path.join(thisdir,'CondorOut',fullname)
#jobdir = 'CondorOut/'+fullname

print namedir
print jobdir

scram='slc7_amd64_gcc630'
cmssw_loc= '/afs/cern.ch/work/e/eranken/private/space/CMSSW_9_4_13/src/'

if os.path.isdir(jobdir):
	print jobdir, 'exists: EXIT'
	sys.exit(-1)
os.mkdir(jobdir)

if not os.path.isdir(namedir):
	os.mkdir(namedir)

exec_name = 'exec.sh'

condor_filetext =  '''#x509userproxy = /tmp/x509up_u48825
universe = vanilla
Executable = {0}
Notification         = never
WhenToTransferOutput = On_Exit
ShouldTransferFiles  = yes 
on_exit_remove       = (ExitBySignal == FALSE && (ExitCode == 0 || ExitCode == 42 || NumJobStarts>3))
+IsFastQueueJob      = True
getenv = true
request_memory       = 16000MB
request_disk         = 10000MB
Transfer_Input_Files = InputCuts,trainID.C
output               = {1}/$(Cluster)_$(Process)_job.out
error                = {1}/$(Cluster)_$(Process)_job.err
Log                  = {1}/$(Cluster)_$(Process)_job.log
+JobFlavour = "testmatch"
Queue'''.format(exec_name,jobdir)

exec_filetext='''#!/bin/bash
echo "inside jobsubmit"
source /cvmfs/cms.cern.ch/cmsset_default.sh 
echo "sourced"
pwd
export SCRAM_ARCH={0}
cd {1}
eval `scramv1 runtime -sh`
cd $_CONDOR_SCRATCH_DIR
pwd
ls
mkdir weights
hostname
root -l -b -q 'trainID.C("{2}",{3},{4})'
cp dataset/weights/* {5}/weights/
cp *.root {5}/'''.format(scram,cmssw_loc,WP,reg,Ntrain,namedir)

condor_file = open(os.path.join(jobdir,'condor.sub'),'w')
condor_file.write(condor_filetext)
condor_file.close()

exec_file = open(os.path.join(jobdir,exec_name),'w')
exec_file.write(exec_filetext)
exec_file.close()

os.system('cd ' + jobdir + ' && condor_submit condor.sub')
