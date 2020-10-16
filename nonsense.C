#include <iostream>
#include <fstream>

void nonsense(){
  vector<double_t> etaBins{0,1.0, 1.479,5.0};
  // vector<double_t> iBins{0.,1.0, 2.0,3.0};
  int N_etaBins = etaBins.size()-1;

  TFile *outRoot=new TFile("EAout/EA.root","recreate");
  double* etaBins_array = &etaBins[0];
  TH2D *EAhist =new TH2D("EAhist","Eta vs pt",3,0.,3.,N_etaBins,etaBins_array);
  outRoot->Write();
  outRoot->Close();


}
