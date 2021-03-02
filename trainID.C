#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/GeneticAlgorithm.h"
#include "TMVA/GeneticFitter.h"
#include "TMVA/IFitterTarget.h"
#include "TMVA/DataLoader.h"
//#include "TMVARegGui.C"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"

using namespace TMVA;



void trainID(TString mode, int region, int Nevents, int pthigh = 200, bool requirePix = 0){

	TString loadcuts;
	if(mode=="L") loadcuts ="I";
	if(mode=="M") loadcuts ="L";
	if(mode=="T") loadcuts ="M";
	if(mode=="S") loadcuts ="T";
	if(mode=="H") loadcuts ="S";

	int ptlow = 15;


//if you want to go back to the old way of reading pt formulas into TMVA
//this commented out stuff can do that if you provide a "string_replace" function

	//ifstream isoPfile;
	//ifstream isoNfile;
	//isoPfile.open(("HPT/isoP"+to_string(region)+".txt"));
	//isoNfile.open(("HPT/isoN"+to_string(region)+".txt"));
	//string isoP_formstring_x;
	//string isoN_formstring_x;
	//isoPfile>>isoP_formstring_x;
	//isoNfile>>isoN_formstring_x;
	//vector<string> justx{"x"};
	//string phoPt = "Ppt";
	
	//string isoP_formstring_ppt = string_replace(isoP_formstring_x, justx, phoPt);
	//string isoN_formstring_ppt = string_replace(isoN_formstring_x, justx, phoPt);

	//cout<<"isoP "<<isoP_formstring_x<<endl;
	//cout<<isoP_formstring_ppt<<endl;
	//cout<<"iso N"<<isoN_formstring_x<<endl;
	//cout<<isoN_formstring_ppt<<endl;


  TMVA::Tools::Instance();
  std::cout << "==> Start TMVARegression" << std::endl;

  ifstream myfile;
  myfile.open("InputCuts/Cuts_"+loadcuts+to_string(region)+".txt");

  ostringstream xcS,xcH,xcP,xcC,xcN;
  double xS,xH,xC,xN,xP;

  if(myfile.is_open()){
      myfile>>xH>>xS>>xC>>xN>>xP;
  cout<<" HSCNP "<<xH<<", "<<xS<<", "<<xC<<", "<<xN<<", "<<xP<<endl;
  }

  xcS<<xS;
  xcC<<xC;
  xcN<<xN;
  xcP<<xP;
  xcH<<xH;

  //Output file
  TString outfileName( "Cuts_"+mode+to_string(region)+"_TMVAout.root" );
  TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

  //Declaring the factory
  TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
					      "V:Color:DrawProgressBar" );
  factory->SetVerbose(true);
  //Declaring Input Variables
  auto dataloader = new TMVA::DataLoader("dataset");
  dataloader->AddVariable( "ToE",'F' );
  dataloader->AddVariable( "Sieie",'F');
  dataloader->AddVariable( "isoC",'F' );
  dataloader->AddVariable( "isoN",'F' );
  dataloader->AddVariable( "isoP",'F' );


  dataloader->AddSpectator( "Ppt",'F' );
  dataloader->AddSpectator("Pix", 'I');
  string fname = "/afs/cern.ch/work/e/eranken/private/space/CMSSW_9_4_13/src/electronIDv1/TrainIn/CutTMVAregion"+to_string(region)+".root";

 TFile* input = new TFile( fname.c_str() ,"READ");

  // --- Register the regression tree
  TTree *signal = (TTree*)input->Get("t_S");
  TTree *background = (TTree*)input->Get("t_B");

  //Just Some more settings
   Double_t signalWeight      = 1.0;
   Double_t backgroundWeight  = 1.0;

   // You can add an arbitrary number of regression trees
   dataloader->AddSignalTree( signal, signalWeight );
   dataloader->AddBackgroundTree( background , backgroundWeight );

  // Set initial selection cuts
  //
  TString cutString ="Ppt>"+to_string(ptlow)+"   && Ppt < "+to_string(pthigh);
  if (requirePix) cutString+=" && Pix==1";

  TCut mycuts =cutString.Data();
  TCut mycutb =cutString.Data();

   //this is where Ntrain, Ntest are included+set 
  dataloader->PrepareTrainingAndTestTree(mycuts,mycutb,"nTrain_Signal="+to_string(Nevents)+":nTrain_Background="+to_string(Nevents)+":nTest_Signal=0:nTest_Background=0");

   cout<<"pre weighT"<<endl;
   dataloader->SetBackgroundWeightExpression("weighT");
   dataloader->SetSignalWeightExpression("weighT");
   cout<<"post weighT"<< endl;

   TString methodName = "Cut_"+mode+to_string(region);
   TString methodOptions ="H:V:FitMethod=GA:EffMethod=EffSel";
   
   methodOptions +=":VarProp[0]=FMin:VarProp[1]=FMin:VarProp[2]=FMin:VarProp[3]=FMin:VarProp[4]=FMin";
   methodOptions +=":CutRangeMax[1]="+xcS.str();
   methodOptions +=":CutRangeMax[2]="+xcC.str();
   methodOptions +=":CutRangeMax[3]="+xcN.str();
   methodOptions +=":CutRangeMax[4]="+xcP.str();
   methodOptions +=":CutRangeMax[0]="+xcH.str();

   methodOptions +=":popsize=400:steps=50";
   
   cout<<"book the method"<<endl;
   factory->BookMethod(dataloader, TMVA::Types::kCuts,methodName,methodOptions);
   cout<<"train all methods"<<endl;
   factory->TrainAllMethods();
   cout<<"test all methods"<<endl;
   factory->TestAllMethods();
   cout<<"evaluate all methods"<<endl;
   factory->EvaluateAllMethods();

   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVARegression is done!" << std::endl;
   delete factory;
}
