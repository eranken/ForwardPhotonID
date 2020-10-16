#include <iostream>
#include <fstream>

void TableWriter(vector<double> &etaBins, vector<vector<double>> &EAB, vector<vector<double>> &EABerr){
  ofstream myfile;
  myfile.open("EAout/Effective_areas.txt");
  ofstream myfile2;
  myfile2.open("EAout/EA.txt");

  myfile<<"| *bin* | *EA charged hadrons* | *EA neutral hadrons* | *EA photons* |"<<endl;
  for (size_t i = 0; i < EAB.size(); i++) {
  	myfile<<etaBins[i]<<" abs(&eta;) "<<etaBins[i+1]<<" | "<< EAB[i][0] <<" "<< EABerr[i][0] <<" | "<< EAB[i][1] <<" "<< EABerr[i][1] <<" | "<< EAB[i][2] <<" "<< EABerr[i][2]<<endl;
	myfile2<<etaBins[i]<<" "<<etaBins[i+1]<<" "<<EAB[i][0]<<" "<<EAB[i][1]<<" "<<EAB[i][2]<<endl;
  }

  myfile.close();
  myfile2.close();

  int N_etaBins = etaBins.size()-1;
  TFile *outRoot=new TFile("EAout/EA.root","recreate");
  double* etaBins_array = &etaBins[0];
  TH2D *EAhist =new TH2D("EAhist","Eta vs pt",3,0,3.,N_etaBins,etaBins_array);
  EAhist->GetXaxis()->SetBinLabel(1,"Iso C");
  EAhist->GetXaxis()->SetBinLabel(2,"Iso N");
  EAhist->GetXaxis()->SetBinLabel(1,"Iso P");
  for (size_t i = 0; i < EAB.size(); i++) {
	EAhist->SetBinContent(1,i+1,EAB[i][0]);
	EAhist->SetBinContent(2,i+1,EAB[i][1]);
	EAhist->SetBinContent(3,i+1,EAB[i][2]);
  }  
  outRoot->Write();
  outRoot->Close();


}
