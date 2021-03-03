#include "cstdlib"
#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TSystem.h"
#include "lib/TableWriter.C"
#include "lib/ErrScanning.C"
#include "lib/ContourBuilder.C"
#include "lib/configParser.h"
/*
#include <cstdlib>
#include <TSystem.h>
#include <TableWriter.C>
#include <ErrScanning.C>
#include <ContourBuilder.C>
*/

void runEA(){
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
 // string baseDir = "root://eoscms.cern.ch//eos/cms/store/group/phys_egamma/PhotonIDtuning/CutBasedID/nTuples_94X_v2/GJetgjet_add.root";
  //string inFilePath = (baseDir+inFileName).c_str();
  gROOT->SetBatch();

  ConfigParser* CP = new ConfigParser("config.cfg");
  vector<double> etaBins = CP->GetVector<double>("EAbins");
  string inFilePath = CP->Get<string>("inputfile");

  //vector<double> etaBins{0,1.0, 1.479,2.0,2.2,2.3,2.4,2.5,2.66,2.87,3.0};
  //vector<double> etaBins{0,1.0, 1.479};
  size_t N_etaBins = etaBins.size()-1;

  vector<vector<double>> Fin;
  vector<vector<double>> FinEr;
  double in,ein,ip,eip,ic,eic, emin, emax;

  cout<<"------------------------------------------------------------"<<endl;
  cout<<"Creating the bins and retrieving the effective area for each"<<endl;
  cout<<"-----------------------------------------------------------"<<endl;

  for (size_t i = 0; i < N_etaBins; i++) {
	  cout<<"EA fitting bin "<<to_string(i+1)<<" of "<<to_string(N_etaBins)<<endl;
	  emin = etaBins[i];
	  emax = etaBins[i+1];
	  ContourBuilder(inFilePath,i+1,emin,emax,in,ein,ip,eip,ic,eic);
	  vector<double> slopeVec{ic,in,ip};
	  vector<double> errVec{eic,ein,eip};
	  Fin.push_back(slopeVec);
	  FinEr.push_back(errVec);
  }

  cout<<"--Writing text file"<<endl;

  TableWriter(etaBins,Fin,FinEr);

}
