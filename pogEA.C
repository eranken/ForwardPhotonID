#include <iostream>
#include <fstream>

void pogEA(){

  vector<vector<double>> EAB;
  vector<double> etaBins;

  etaBins.push_back(0.0);
  etaBins.push_back(1.0);
  etaBins.push_back(1.479);
  etaBins.push_back(2.0);
  etaBins.push_back(2.2);
  etaBins.push_back(2.3);
  etaBins.push_back(2.4);
  etaBins.push_back(3.0);

  EAB.push_back({0.0112,0.0668,0.1113});
  EAB.push_back({0.0108,0.1054,0.0953});
  EAB.push_back({0.0106,0.0786,0.0619});
  EAB.push_back({0.01002,0.0233,0.0837});

  EAB.push_back({0.0098,0.0078,0.1070});
  EAB.push_back({0.0089,0.0028,0.1212});
  EAB.push_back({0.0087,0.0137,0.1466});




  ofstream myfile2;
  myfile2.open("EAout/manualEA.txt");

  for (size_t i = 0; i < EAB.size(); i++) {
	myfile2<<etaBins[i]<<" "<<etaBins[i+1]<<" "<<EAB[i][0]<<" "<<EAB[i][1]<<" "<<EAB[i][2]<<endl;
  }

  myfile2.close();

  int N_etaBins = etaBins.size()-1;
  TFile *outRoot=new TFile("EAout/EA.root","recreate");
  double* etaBins_array = &etaBins[0];
  TH2D *EAhist =new TH2D("EAhist","Eta vs pt",3,0,3.,N_etaBins,etaBins_array);
  EAhist->GetXaxis()->SetBinLabel(1,"Iso C");
  EAhist->GetXaxis()->SetBinLabel(2,"Iso N");
  EAhist->GetXaxis()->SetBinLabel(3,"Iso P");
  for (size_t i = 0; i < EAB.size(); i++) {
	EAhist->SetBinContent(1,i+1,EAB[i][0]);
	EAhist->SetBinContent(2,i+1,EAB[i][1]);
	EAhist->SetBinContent(3,i+1,EAB[i][2]);
  }
  outRoot->Write();
  outRoot->Close();


}
