#include <TMath.h>
#include <cstdlib>
#include <TRandom.h>
#include <TGraph.h>
#include <TString.h>
#include <vector>
#include <new>
#include "sstream"
#include <string>
#include <fstream>
#include <iostream>
#include "ErrCalc.h"


void ptFit(const string NorP, const string fittype, const int region){

  //TFile *f1 = new TFile("/uscms_data/d3/asroy/PhotonIdTuning/CMSSW_7_3_5/src/CutBasedPhoID2016/merged/ChEA_ISOPT/CutTMVABarrel90_HPT.root");
  TFile *f1 = new TFile(("HPT/CutTMVAregion"+to_string(region)+".root").c_str(),"READ");
  float genPt,ppt,peta,Sie_ie,iso_P,iso_C,iso_N,to_e,weighT;
  int nvtx;
  gStyle->SetOptStat(0);


TTree *t_S = (TTree*)f1->Get("t_S");
TTree *t_B = (TTree*)f1->Get("t_B");

  //Signal Tree
  t_S->SetBranchAddress("Sieie",&Sie_ie);
  t_S->SetBranchAddress("isoP",&iso_P);
  t_S->SetBranchAddress("isoC",&iso_C);
  t_S->SetBranchAddress("isoN",&iso_N);
  t_S->SetBranchAddress("ToE",&to_e);
  t_S->SetBranchAddress("weighT",&weighT);

  t_S->SetBranchAddress("Nvtx",&nvtx);
  t_S->SetBranchAddress("Peta",&peta);
  t_S->SetBranchAddress("Ppt",&ppt);
  t_S->SetBranchAddress("genPt",&genPt);

  //Background Tree
  t_B->SetBranchAddress("Sieie",&Sie_ie);
  t_B->SetBranchAddress("isoP",&iso_P);
  t_B->SetBranchAddress("isoC",&iso_C);
  t_B->SetBranchAddress("isoN",&iso_N);
  t_B->SetBranchAddress("ToE",&to_e);
  t_B->SetBranchAddress("weighT",&weighT);

  t_B->SetBranchAddress("Nvtx",&nvtx);
  t_B->SetBranchAddress("Peta",&peta);
  t_B->SetBranchAddress("Ppt",&ppt);

  t_S->SetBranchAddress("isoN",&iso_N);
  t_S->SetBranchAddress("isoP",&iso_P);


  int nbinsX = (NorP=="N" ? 250 : 100);
  TH2F *isoPptS = new TH2F("isoPptS",("Iso "+NorP+" vs Pt").c_str(),nbinsX,15,700,1000,0,100);

  float *relevant_iso = (NorP == "P" ? &iso_P : &iso_N);
  //used to be 25 GeV cut for iso P??
  float lowPtCut = ( region >0 && NorP =="P" ? 15. : 15. );

  for(int i  = 0; i < t_S->GetEntries();i++){
  // for(int i  = 0; i < 100;i++){

    t_S->GetEntry(i);
    // cout<<"ppt "<<to_string(ppt)<<" IsoP: "<<to_string(iso_P)<<" rel iso: "<<to_string(*relevant_iso)<<endl;
    if(ppt < lowPtCut || ppt > 1000) continue;
    if(iso_P == 0) continue;
    isoPptS->Fill(ppt,*relevant_iso);
  }

  cout<<"Built the 2d HISTOGRAM"<<endl;

  TH2F *his2 = (TH2F*)isoPptS->Clone(("test"+to_string(region)+NorP).c_str());

  int dim = his2->GetXaxis()->GetNbins();

  double* cutV   = new double[dim];
  double* errVH   = new double[dim];
  double* errVL   = new double[dim];
  double* binc   = new double[dim];
  double* bincerH = new double[dim];
  double* bincerL = new double[dim];

  for(int i  = 1; i < dim+1 ; i++){
    double xval = 0;
    double errXH = 0;
    double errXL = 0;

    TH1D *r22 = his2->ProjectionY(" ",i,i+1," ");

    TH1D *h1 = (TH1D*)r22->Clone(("test"+to_string(i)+NorP).c_str());
	// cout<<"hist ints :"<<his2->Integral()<<", "<<r22->Integral()<<", "<<h1->Integral()<<endl;
    ErrCalc(h1,i,0.950,xval,errXL,errXH);




    cutV[i-1]   = xval;
    errVL[i-1]   = errXL;
    errVH[i-1]   = errXH;
    binc[i-1]   = his2->GetXaxis()->GetBinCenter(i);

	cout<<"bin :"<<i<<" x: "<<binc[i-1]<<", y: "<<xval<<" -"<<errXL<<" +" << errXH<<endl;
	//I don't know why I keep getting NANs, this is a shitty fix.
	if (!(binc[i-1]>0 || binc[i-1]<=0))
	{
		cutV[i-1]   = 0.;
		errVL[i-1]   = 0.;
		errVH[i-1]   = 0.;
		binc[i-1] = -100.;
	}
	if (!(errVL[i-1]>0 || errVL[i-1]<=0))
	{
		cutV[i-1]   = 0.;
		errVL[i-1]   = 0.;
		errVH[i-1]   = 0.;
		binc[i-1] = -100.;
	}
	if (!(xval>0 || xval<=0))
	{
		cutV[i-1]   = 0.;
		errVL[i-1]   = 0.;
		errVH[i-1]   = 0.;
		binc[i-1] = -100.;
	}
	if (xval<0.)
	{
		cutV[i-1]   = 0.;
		errVL[i-1]   = 0.;
		errVH[i-1]   = 0.;
		binc[i-1] = -100.;
	}
    bincerL[i-1] = 0;
    bincerH[i-1] = 0;
  }
  // for(int i  = 1; i < dim ; i++){
	//   cout<<"bin :"<<i<<" x: "<<binc[i-1]<<", y: "<<xval<<" -"<<errXL<<" +" << errXH<<endl;
  // }
  TGraphAsymmErrors * IsoptScaling = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);
  TGraphAsymmErrors * IsoptScaling2 = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);

  // double DownL = his2->GetXaxis()->GetBinCenter(1);
  //double UpperL = his2->GetXaxis()->GetBinCenter(dim);

  gStyle->SetOptFit(1);
  TString fitResult;

  if (fittype=="lin" || fittype=="linear") {
	float fitmax = (region>=2 ? 400 : 700);
	string fitname = ("linfit"+NorP+to_string(region));
	TGraphAsymmErrors * IsoptScalingLin = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);
  	TF1 *fnlin = new TF1(fitname.c_str(),"[0]*x + [1]",20,fitmax);
	fnlin->SetParameters(0.1,4.);
	IsoptScalingLin->Fit(fitname.c_str(),"R");
	fitResult= fnlin->GetExpFormula("P");
     cout<<"doing fit "<< fitname<<endl;

	TCanvas *c3 = new TCanvas(("c"+fitname).c_str(),"Iso Pt",1200,600);
	c3->Divide(2,1);

	c3->cd(1);
	IsoptScalingLin->SetMarkerStyle(24);
	IsoptScalingLin->SetMarkerSize(0.4);
	IsoptScalingLin->Draw("AP");
	IsoptScalingLin->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
	IsoptScalingLin->GetYaxis()->SetTitle(("Iso "+NorP+" #rho-corrected").c_str());
	c3->cd(2);
	his2->Draw("colz");
	his2->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
	his2->GetYaxis()->SetTitle(("Iso "+NorP+" rho corr.").c_str());

	c3->SaveAs(("HPT/region"+to_string(region)+"_iso"+NorP+".png").c_str());
  }

  if (fittype=="quad" || fittype=="quadratic") {
  float fitmax = (region>=2 ? 500 : 700);
  if(region>=3) fitmax = 400;
  string fitname = ("quadfit"+NorP+to_string(region));
  TGraphAsymmErrors * IsoptScalingQuad = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);
  TF1 *fnquad = new TF1(fitname.c_str(),"[0]*x*x + [1]*x + [2]",20,fitmax);
  fnquad->SetParameters(0.2,0.1,4.);
  IsoptScalingQuad->Fit(fitname.c_str(),"R");
  fitResult= fnquad->GetExpFormula("P");
  cout<<"doing fit "<< fitname<<endl;



  TCanvas *c3 = new TCanvas(("c"+fitname).c_str(),"Iso Pt",1200,600);
  c3->Divide(2,1);

  c3->cd(1);
  IsoptScalingQuad->SetMarkerStyle(24);
  IsoptScalingQuad->SetMarkerSize(0.4);
  IsoptScalingQuad->Draw("AP");
  IsoptScalingQuad->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
  IsoptScalingQuad->GetYaxis()->SetTitle(("Iso "+NorP+" #rho-corrected").c_str());
  c3->cd(2);
  his2->Draw("colz");
  his2->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
  his2->GetYaxis()->SetTitle(("Iso "+NorP+" rho corr.").c_str());

  c3->SaveAs(("HPT/region"+to_string(region)+"_iso"+NorP+".png").c_str());
  }

  if (fittype=="log" || fittype=="logarithmic") {
  float fitmax = (region>=2 ? 500 : 700);
  if(region>=3) fitmax = 400;

  string fitname = ("logfit"+NorP+to_string(region));
  TGraphAsymmErrors * IsoptScalingLog = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);
  TF1 *fnlog = new TF1(fitname.c_str(),"[0]*log([1]+[2]*x)",20,fitmax);
  fnlog->SetParameters(0.2,0.1,4.);
  IsoptScalingLog->Fit(fitname.c_str(),"R");
  fitResult= fnlog->GetExpFormula("P");
  cout<<"doing fit "<< fitname<<endl;



  TCanvas *c3 = new TCanvas(("c"+fitname).c_str(),"Iso Pt",1200,600);
  c3->Divide(2,1);

  c3->cd(1);
  IsoptScalingLog->SetMarkerStyle(24);
  IsoptScalingLog->SetMarkerSize(0.4);
  IsoptScalingLog->Draw("AP");
  IsoptScalingLog->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
  IsoptScalingLog->GetYaxis()->SetTitle(("Iso "+NorP+" #rho-corrected").c_str());
  c3->cd(2);
  his2->Draw("colz");
  his2->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
  his2->GetYaxis()->SetTitle(("Iso "+NorP+" rho corr.").c_str());

  c3->SaveAs(("HPT/region"+to_string(region)+"_iso"+NorP+".png").c_str());
  }


  cout<<fitResult.Data()<<endl;
  cout<< "---------------"<<endl;

  ofstream myfile;
  myfile.open(("HPT/iso"+NorP+to_string(region)+".txt").c_str());
  myfile<<fitResult.Data();
  myfile.close();



}
