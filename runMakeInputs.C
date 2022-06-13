#include <stdlib.h>
#include <TSystem.h>
#include "lib/CutID.C"
#include "lib/configParser.h"

void runMakeInputs(string mode = "TrainIn"){
  //-----------------Macro Describtion----------------------------------------------------------
  //
  // This macro wraps the whole procedure in a single shell.
  // Initialy it caluclates the effective areas -macros (AreaCalc.C + Fitter.C)
  // and then feed them in a Tree Maker, that makes a Tree with the variables of interest
  // separated to background/signal and weight based on the pt, for possible MVA techinques
  // and to derive the cuts.
  // This file to run, needs the full construction of the CUTID class and also the extra macros
  // for each of the specified process.
  //
  //--------------------------------------------------------------------------------------------

//  gSystem->CompileMacro("CutID.C");
	//string mode = "HPT";
	//string mode = "TrainIn";
	int enoughEvents =10000000;
	string inputFilePath;
	ConfigParser* CP = new ConfigParser("config.cfg");
        vector<double> etaRegionBins = CP->GetVector<double>("regions");
        inputFilePath = CP->Get<string>("inputfile");

	if (mode=="HPT") {
               inputFilePath = CP->Get<string>("HPTfile");
	}
	else inputFilePath = CP->Get<string>("inputfile"); 
	
      	vector<vector<double>> ptRegions;


	for (size_t i = 0; i < etaRegionBins.size() ; i+=2)
	{
	    ptRegions.push_back({etaRegionBins[i],etaRegionBins[i+1]}); 
	}

	  //this lets you skip over events for regions with too many events
	  //TMVA appears to use system memory if your input trees have many more events than needed for the training
	  vector<size_t> region_skips;
	  if(mode=="HPT") region_skips = {1,1,1,1,1,1};
	  else region_skips = {2,1,1,1,1,1};

	  CutID t(0,inputFilePath);



  vector<vector<double>> Fin;
  vector<double> EAread;


  ifstream myfile;
  //myfile.open("/uscms_data/d3/asroy/PhotonIdTuning/CMSSW_7_3_5/src/CutBasedPhoID2016/merged/ChEA_ISOPT/EA.txt");
  // myfile.open("EAout/EA.txt");
  TFile *EAfile = new TFile("EAout/EA.root");
  TH2D *EAhist = (TH2D*)EAfile->Get("EAhist");



  cout<<"------------------------------------------------------------"<<endl;
  cout<<"                   Reading the EAS "<<endl;
  cout<<"-----------------------------------------------------------"<<endl;

  //
  // int i = 0;
  // if(myfile.is_open()){
  //   while(!myfile.eof()){
  //     double etaLow,etaHigh,iC,iN,iP;
  //     myfile>>etaLow>>etaHigh>>iC>>iN>>iP;
	//   EAread = {etaLow,etaHigh,iC,iN,iP};
	//   Fin.push_back(EAread);
  //
  //     cout<<i<<" "<<etaLow<<"-"<<etaHigh<<": "<<iC<<" "<<iN<<" "<<iP<<endl;
  //     i++;
  //
  //   }
  // }
  for (size_t i = 0; i < ptRegions.size(); i++) {
	  cout<<"----------------------doing region "<<to_string(i)<< endl;
	  double etaLow=ptRegions[i][0];
	  double etaHigh=ptRegions[i][1];
	  cout <<etaLow<<" < eta < "<<etaHigh<<endl;
	  t.CutBasedID(i,etaLow,etaHigh,EAhist,mode,region_skips[i]);
  }





}
