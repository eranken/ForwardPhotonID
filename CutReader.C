#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include "TH2.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

using namespace TMVA;

void CutReader(TString whichcut, int region){

	TString regionname= to_string(region);


  TMVA::Tools::Instance();


  //ofstream myfileT;
  //myfileT.open("TightR.txt");


  //ofstream myfileM;
  //myfileM.open("MediumR.txt");


  ofstream outfile;
  outfile.open("InputCuts/Cuts_"+whichcut+regionname+".txt");


  //Declaring the reader
  //TString methodName = "Cuts_Tight_r";
  //TString weightfile = "./weights/TMVAClassification_Cut_Tight_r.weights.xml";

  //TString methodName = "Cuts_Medium_r";
  //TString weightfile = "./weights/TMVAClassification_Cut_Medium_r.weights.xml";

  // TString methodName = "Cut_"+whichcut+regionname;
	TString methodName = "Cut_"+whichcut+regionname;

  TString weightfile = "./TrainOut/TMVAClassification_"+methodName+".weights.xml";

  TMVA::Reader *reader = new TMVA::Reader( "!Color" );
  float Sieie,ToE,isoC,isoN,isoP,pt;
  reader->AddVariable("ToE",&ToE);
  reader->AddVariable("Sieie",&Sieie);
  reader->AddVariable( "isoC",&isoC );
  reader->AddVariable( "isoN",&isoN );
  reader->AddVariable( "isoP",&isoP );

  //reader->AddVariable( "(isoN-(0.0139*Ppt+0.000025*Ppt*Ppt) > 0 ) ? isoN-(0.0139*Ppt+0.000025*Ppt*Ppt) : 0.0",&isoN );
  //reader->AddVariable( "(isoP-0.0034*Ppt > 0) ? isoP-0.0034*Ppt : 0.0 ",&isoP );


  reader->AddSpectator("Ppt",&pt);


  reader->BookMVA(methodName,weightfile);


  TMVA::MethodCuts* mcuts = dynamic_cast<TMVA::MethodCuts*>(reader->FindCutsMVA(methodName) );
  std::vector<Double_t> cutsMin;
  std::vector<Double_t> cutsMax;


  double SEF = whichcut=="L" ? 0.90 : (whichcut=="M" ? 0.8 : (whichcut == "T" ? 0.7 : 0.0));
  cout << SEF << endl;
  if(mcuts)mcuts->GetCuts(SEF, cutsMin, cutsMax );
  outfile<<cutsMax[0]<<endl<<cutsMax[1]<<endl<<cutsMax[2]<<endl<<cutsMax[3]<<endl<<cutsMax[4]<<endl;

  delete reader;
  outfile.close();
  cout<<"DONE READING CUTS"<<endl;

}
