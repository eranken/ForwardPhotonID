#include <stdlib.h>
#include <TSystem.h>
#include "CutID.C"

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

	string inputFilePath;
	if (mode=="HPT") {
		inputFilePath = "root://eoscms.cern.ch//eos/cms/store/group/phys_egamma/PhotonIDtuning/CutBasedID/nTuples_94X_v2/gjet_pt15to6000.root";
	}
	else inputFilePath = "root://eoscms.cern.ch//eos/cms/store/group/phys_egamma/PhotonIDtuning/CutBasedID/nTuples_94X_v2/GJetgjet_add.root";
	  vector<vector<double>> ptRegions;
	  ptRegions.push_back({0.,1.4442});
	  ptRegions.push_back({1.566,2.5});
	  ptRegions.push_back({2.5,2.87});
	  ptRegions.push_back({1.566,2.15});
	  ptRegions.push_back({2.15,2.87});
	  
	  //this lets you skip over events for regions with too many events
	  //TMVA appears to use system memory if your input trees have many more events than needed for the training
	  vector<size_t> region_skips;
	  if(mode=="HPT") region_skips = {1,1,1,1,1,1};
	  else region_skips = {4,2,1,1,1,1};

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
