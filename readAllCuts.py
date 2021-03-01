import sys, os, time, subprocess, re

name = sys.argv[1]
thisdir = os.getcwd()

inpath = "/eos/cms/store/group/phys_smp/aleko/Run1n2v08m/"

filepath = os.path.join("CondorOut",name)
files = os.listdir(filepath)
files = [x for x in files if x.startswith(name)]

print files

files = [x.replace(name+'_','').replace('.root','') for x in files]

print files

os.system('rm InputCuts/Cuts_L*')
os.system('rm InputCuts/Cuts_M*')
os.system('rm InputCuts/Cuts_T*')
os.system('rm InputCuts/Cuts_S*')

os.system('cp '+filepath+'/weights/*.xml TrainOut')

for f in files:
	os.system('''root -l -q -b 'CutReader.C("'''+f[0]+'''",'''+f[1]+''')' ''')
