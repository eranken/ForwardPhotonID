import sys, os, time, subprocess, re, argparse

parser = argparse.ArgumentParser()

parser.add_argument('--name',action='store', type=str)
parser.add_argument('--pix',action='store', type=str, default = "0")
parser.add_argument('--WP',action='store', type=str)
parser.add_argument('--region',action='store', type=str)
parser.add_argument('--Ntrain',action='store', type=str, default = "100000")
parser.add_argument('--hours',action='store', type=int, default = 24) 
parser.add_argument('--ptcutoff',action='store', type=str, default = "200") 

args = parser.parse_args()

name = args.name
WP = args.WP
reg = args.region
Ntrain = args.Ntrain
highpt = args.ptcutoff
pixreq = args.pix
hours = args.hours

thisdir = os.getcwd()
infile = os.path.join(thisdir,'TrainIn','CutTMVAregion'+str(reg)+'.root')
print infile 

fullname = name+'_'+WP+reg
namedir = os.path.join(thisdir,'CondorOut',name)
jobdir = os.path.join(thisdir,'CondorOut',name,fullname)
#jobdir = 'CondorOut/'+fullname

print namedir
print jobdir

scram='slc7_amd64_gcc630'
cmssw_loc= '/afs/cern.ch/work/e/eranken/private/space/CMSSW_9_4_13/src/'

if os.path.isdir(jobdir):
	print jobdir, 'exists: EXIT'
	sys.exit(-1)

if not os.path.isdir(namedir):
	os.mkdir(namedir)
	os.mkdir(os.path.join(namedir,'weights'))
os.mkdir(jobdir)

jobflavor = 'workday'
maxruntime = hours*3600
exec_name = 'exec.sh'

inputcuts = os.path.join(thisdir,'InputCuts')
macropath = os.path.join(thisdir,'trainID.C')

condor_filetext =  '''#x509userproxy = /tmp/x509up_u48825
universe = vanilla
Executable = {0}
Notification         = never
WhenToTransferOutput = On_Exit
ShouldTransferFiles  = yes 
on_exit_remove       = (ExitBySignal == FALSE && (ExitCode == 0 || ExitCode == 42 || NumJobStarts>3))
+IsFastQueueJob      = True
getenv = true
request_memory       = 4000MB
request_disk         = 4000MB
Transfer_Input_Files = {1},{2}
output               = {3}/$(Cluster)_$(Process)_job.out
error                = {3}/$(Cluster)_$(Process)_job.err
log                  = {3}/$(Cluster)_$(Process)_job.log
+MaxRuntime = {4}
Queue'''.format(exec_name,inputcuts,macropath,jobdir,str(maxruntime))

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
root -l -b -q 'trainID.C("{2}",{3},{4},{5},{6},"{7}")'
cp dataset/weights/* {8}/weights/'''.format(scram,cmssw_loc,WP,reg,Ntrain,highpt,pixreq,infile,namedir)

condor_file = open(os.path.join(jobdir,'condor.sub'),'w')
condor_file.write(condor_filetext)
condor_file.close()

exec_file = open(os.path.join(jobdir,exec_name),'w')
exec_file.write(exec_filetext)
exec_file.close()

os.system('cd ' + jobdir + ' && condor_submit condor.sub')
#os.system('condor_submit condor.sub')
