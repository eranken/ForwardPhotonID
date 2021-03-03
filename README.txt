######
basics
######

The procedure to prepare for training the ID is divided into root Macros of form "runXXX.C" which must be run in a specific order. These files are minimal and exist to call methods from the modified photonID source code (in folder lib) in a flexible way. Some basic configuration options are taken directly as inputs to the macros. Configuration of things like \eta bins for effective area calculation, input file locations, or ID regions, can all be changed by altering the contents of the "config.cfg" file. In theory this should cover most ofthe things a user might want to change. Only for configuration of more technical options is it necessary to  alter the underlying methods in the "lib" folder.

##############
prepare inputs
##############

NOTE: steps 2 and 5 are time consuming and can be sent off to condor if desired.

-----------------
0.  ensure all necessary directories are present by running 

$ source ./setup.sh

-----------------
1.  set the input files and binning as desired in config.cfg .

-----------------
2. To calculate Effective Area slopes, run

$ root -l -q -b runEA.C

Outputs are stored in "EAout/"

-----------------
3. Unlike EA-adjustment, pT-adjustment is done in each the final ID regions. To make EA-adjusted Inputs used for pT-scaling, run

$ root -l -q -b 'runMakeInputs.C("HPT")'

the option "HPT" will make inputs from high-pt n-tuples specified as an input in runMakeInputs.C. Ntuples to examine pt dependence will be generated in the folder HPT/ . One file is generated for each region, the regions are numbered starting with "0".

-----------------
4. To carry out the pT-dependence fitting, run

$ root -l -q -b runPtFit.C

Which types of fit you wish to use can be altered in the config file between "lin", "quad", and "log". This generates text files in the folder "HPT/" which contain the resulting TFormulas for pT-dependence. These will be applied automaticall when making the final training inputs.

-----------------
5. The last major step before training the ID is to make inputs which have both EA-adjustment and pT-adjustment. Simply run

$ root -l -q -b runMakeInputs.C

this will make the final training inputs in the directory "TrainIn/"

-----------------
OPTIONAL: if you also want to include H over E pileup+noise corrections, currently the inputs must be regenerate. first, run

$ root -l -b -q runHoE.C

to fit the corrections which are stored as TFormulas in the directory HOE/ . then run 

$ root -l -q -b 'runMakeInputs.C("HoEcorr")'

to generate input root files with HoE corrections applied, in the HoEcorr directory. if you want to use these inputs, copy them to TrainIn/ . We could load the formulas into TMVA instead, but this is the current method.

-----------------
6. We must provide TMVA with a staring point. We can derive 99.9% efficiency WPs for all cut variables by running 

$ root -l -q -b run99.C

Which will generate files needed by TMVA in the directory "InputCuts/"

###############
training the ID
###############

The macro which runs TMVA to train the cut-based ID is "trainID.C". It accepts 3 inputs:

string WP = "L", "M", or "T"
int reg = 0, 1, ...
int Ntrain = number of signal and background events to use in training. 

Try running locally with a small Ntrain to make sure everything works. Call it via 

$ root -l -q -b 'trainID.C(WP,reg,Ntrain)'

You have to run the Loose WP before Medium, and so on. Each one requires knowledge of the previous cut results to be present in the directory "InputCuts/" (the files needed for the loose WP are generated in step 6 above). To do this, after running a given WP, move the output files from the default TMVA output directory "weights" to "TrainOut". Then call 

$ root -l -q -b 'CutReader.C(WP,reg)'

(for all applicable regions). This will generate the necessary inputs for the next WP. It also encodes the result of the previous working point, though you can read that off of the output xml file yourself in more detail.

The training can take a long time when run on large training sample sizes. Examples of ways to delegate this to condor are found in condorTrain.sub and condorTrain.sh, with a more sophisticated method in the jobsub.py script. This can be interfaced to the script readAllCuts.py, to efficiently read the outputs. However, these were made for my ease of use and should be customized for your setup/preferences.

Also note that trainID.C takes other optional inputs, such as allowing for a pixel seed requirement or specifying the full path of the input file. the latter is necessary for condor jobs.
