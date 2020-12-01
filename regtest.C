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

#include "helper.cc"

#include "TMVA/GeneticAlgorithm.h"
#include "TMVA/GeneticFitter.h"
#include "TMVA/IFitterTarget.h"
#include "TMVA/DataLoader.h"
//#include "TMVARegGui.C"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"

using namespace TMVA;



void regtest(){
	int region= 0;

	ifstream isoPfile;
	ifstream isoNfile;
	isoPfile.open(("HPT/isoP"+to_string(region)+".txt"));
	isoNfile.open(("HPT/isoN"+to_string(region)+".txt"));
	string isoP_formstring_x;
	string isoN_formstring_x;
	isoPfile>>isoP_formstring_x;
	isoNfile>>isoN_formstring_x;
	vector<string> justx{"x"};
	string phoPt = "Ppt";

	string isoP_formstring_ppt = string_replace(isoP_formstring_x, justx, phoPt);
	string isoN_formstring_ppt = string_replace(isoN_formstring_x, justx, phoPt);

	cout<<isoP_formstring_x<<endl;
	cout<<isoP_formstring_ppt<<endl;
	cout<<isoN_formstring_x<<endl;
	cout<<isoN_formstring_ppt<<endl;


  TMVA::Tools::Instance();
  std::cout << "==> Start TMVARegression" << std::endl;

  ifstream myfile;
  myfile.open(("99per"+to_string(region)+".txt").c_str());

  ostringstream xcS,xcH,xcP,xcC,xcN;
  double xS,xH,xC,xN,xP;

  if(myfile.is_open()){
   ///// while(!myfile.eof()){
      myfile>>xS>>xH>>xC>>xN>>xP;
  cout<<" SHCNP "<<xS<<", "<<xH<<", "<<xC<<", "<<xN<<", "<<xP<<endl;
   ///// }
  }

  xcS<<xS;
  xcC<<xC;
  xcN<<xN;
  xcP<<xP;
  xcH<<xH;

  //Output file
  TString outfileName( "Loose_region_"+to_string(region)+"_TMVAout.root" );
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

  //factory->AddVariable( "(isoN-(0.014*Ppt+0.000019*Ppt*Ppt) > 0 ) ? isoN-(0.014*Ppt+0.000019*Ppt*Ppt) : 0.0 ",'F' );
  //factory->AddVariable( "(isoP-0.0053*Ppt > 0 ) ? isoP-0.0053*Ppt : 0.0 ",'F' );

  dataloader->AddSpectator( "Ppt",'F' );

  //dataloader->AddVariable( "isoC",'F' );
  //TString fname = "../../CutTMVABarrel90.root";
  string fname = "/afs/cern.ch/work/e/eranken/private/space/CMSSW_9_4_13/src/ForwardPhotonID/TrainIn/CutTMVAregion"+to_string(region)+".root";

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

   TCut mycuts ="Ppt>15   && Ppt < 200";
   TCut mycutb ="Ppt>15   && Ppt < 200";
   //factory->PrepareTrainingAndTestTree(mycuts,mycutb,"");
   //factory->PrepareTrainingAndTestTree(mycuts,mycutb,"nTrain_Signal=300000:nTrain_Background=300000:nTest_Signal=300000:nTest_Background=300000");
//original:::
//  dataloader->PrepareTrainingAndTestTree(mycuts,mycutb,"nTrain_Signal=5000000:nTrain_Background=5000000:nTest_Signal=5000000:nTest_Background=5000000");

  dataloader->PrepareTrainingAndTestTree(mycuts,mycutb,"nTrain_Signal=2000000:nTrain_Background=2000000:nTest_Signal=0:nTest_Background=0");
 //factory->PrepareTrainingAndTestTree(mycuts,mycutb,"nTrain_Signal=1000:nTrain_Background=1000:nTest_Signal=1000:nTest_Background=1000");
   //nTrain_Signal=3000:nTrain_Background=6000:nTest_Signal=3000:nTest_Background=3000");

   cout<<"pre weighT"<<endl;
   dataloader->SetBackgroundWeightExpression("weighT");
   dataloader->SetSignalWeightExpression("weighT");
   cout<<"post weighT"<< endl;

   TString methodName = "Cut_Loose_r"+to_string(region);
   //TString methodName = "CutsGA";
   TString methodOptions ="H:V:FitMethod=GA:EffMethod=EffSel";
   methodOptions +=":VarProp[0]=FMin:VarProp[1]=FMin:VarProp[2]=FMin:VarProp[3]=FMin:VarProp[4]=FMin";
//  methodOptions +=":VarProp[0]=FMax:VarProp[1]=FMax:VarProp[2]=FMax:VarProp[3]=FMax";
//old cut method
   methodOptions +=":CutRangeMax[1]="+xcS.str();
   methodOptions +=":CutRangeMax[2]="+xcC.str();
   methodOptions +=":CutRangeMax[3]="+xcN.str();
   methodOptions +=":CutRangeMax[4]="+xcP.str();
   methodOptions +=":CutRangeMax[0]="+xcH.str();

     //methodOptions +=":CutRangeMax[0]=0.5";
     //methodOptions +=":CutRangeMax[1]=0.5";
     //methodOptions +=":CutRangeMax[1]=10.";
//  
//   methodOptions +=":CutRangeMax[3]=10.";
	   //   methodOptions +=":CutRangeMax[4]="+xcP.str(); //!!!!!!!!!!

//   methodOptions +=":CutRangeMin[0]=-10.0"; 
//   methodOptions +=":CutRangeMin[1]=-10.0";
 //  methodOptions +=":CutRangeMin[2]=-10.0";
  // methodOptions +=":CutRangeMin[3]=-10.0";
   // methodOptions +=":CutRangeMin[4]=-1.0";

//   methodOptions +=":nTrain_Signal=10000:nTrain_Background=10000:nTest_Signal=10000:nTest_Background=100000";
   //************
   methodOptions +=":popsize=400:steps=50";
   
   cout<<"gonna book the method"<<endl;
   factory->BookMethod(dataloader, TMVA::Types::kCuts,methodName,methodOptions);
   cout<<"gonna train all methods"<<endl;
   factory->TrainAllMethods();
   cout<<"gonna test all methods"<<endl;
   factory->TestAllMethods();
   cout<<"gonna evaluate all methods"<<endl;
   factory->EvaluateAllMethods();

   // --------------------------------------------------------------
   // Save the output
   outputFile->Close();



   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVARegression is done!" << std::endl;
   delete factory;
}
