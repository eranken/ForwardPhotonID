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


void hoeFit(const int region){

  //TFile *f1 = new TFile("/uscms_data/d3/asroy/PhotonIdTuning/CMSSW_7_3_5/src/CutBasedPhoID2016/merged/ChEA_ISOPT/CutTMVABarrel90_HPT.root");
  TFile *f1 = new TFile(("HOE/CutTMVAregion"+to_string(region)+".root").c_str(),"READ");
  float genPt,ppt,peta,Sie_ie,iso_P,iso_C,iso_N,ToE,weighT,phoE,hE,rho;
  int nvtx;
  gStyle->SetOptStat(0);


TTree *t_S = (TTree*)f1->Get("t_S");
TTree *t_B = (TTree*)f1->Get("t_B");

  //Signal Tree
  t_S->SetBranchAddress("Sieie",&Sie_ie);
  t_S->SetBranchAddress("isoP",&iso_P);
  t_S->SetBranchAddress("isoC",&iso_C);
  t_S->SetBranchAddress("isoN",&iso_N);
  t_S->SetBranchAddress("ToE",&ToE);
  t_S->SetBranchAddress("weighT",&weighT);
  t_S->SetBranchAddress("phoE",&phoE);
  t_S->SetBranchAddress("Rh",&rho);

  t_S->SetBranchAddress("Nvtx",&nvtx);
  t_S->SetBranchAddress("Peta",&peta);
  t_S->SetBranchAddress("Ppt",&ppt);
  t_S->SetBranchAddress("genPt",&genPt);
  t_S->SetBranchAddress("genPt",&genPt);

  //Background Tree
  t_B->SetBranchAddress("Sieie",&Sie_ie);
  t_B->SetBranchAddress("isoP",&iso_P);
  t_B->SetBranchAddress("isoC",&iso_C);
  t_B->SetBranchAddress("isoN",&iso_N);
  t_B->SetBranchAddress("ToE",&ToE);
  t_B->SetBranchAddress("weighT",&weighT);
  t_B->SetBranchAddress("phoE",&phoE);
  t_B->SetBranchAddress("Rh",&rho);


  t_B->SetBranchAddress("Nvtx",&nvtx);
  t_B->SetBranchAddress("Peta",&peta);
  t_B->SetBranchAddress("Ppt",&ppt);

  t_S->SetBranchAddress("isoN",&iso_N);
  t_S->SetBranchAddress("isoP",&iso_P);


  int nbinsX = 40;
  int fitmin = 10;
  TH2F *hist2d = new TH2F("hist2d","H vs rho",nbinsX,10,50,1000,0,50);


  //low pt cut used to be 25 GeV cut for iso P??
  float lowPtCut = 15;

  for(int i  = 0; i < t_S->GetEntries();i++){

    t_S->GetEntry(i);
    if(ppt < lowPtCut || ppt > 200) continue;
    if(iso_P == 0) continue;
    hist2d->Fill(rho,ToE*phoE);
  }

  cout<<"Built the 2d HISTOGRAM"<<endl;

  TH2F *his2 = (TH2F*)hist2d->Clone(("test"+to_string(region)).c_str());

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

    TH1D *h1 = (TH1D*)r22->Clone(("test"+to_string(i)).c_str());
    ErrCalc(h1,i,0.90,xval,errXL,errXH);

    cutV[i-1]   = xval;
    errVL[i-1]  = errXL;
    errVH[i-1]  = errXH;
    binc[i-1]   = his2->GetXaxis()->GetBinCenter(i);



	if (!(binc[i-1]>0 || binc[i-1]<=0))
	{
    cout<<"bin :"<<i<<" x: "<<binc[i-1]<<", y: "<<xval<<" -"<<errXL<<" +" << errXH<<endl;
		cutV[i-1]   = 0.;
		errVL[i-1]   = 0.;
		errVH[i-1]   = 0.;
		binc[i-1] = -100.;
	}
	if (!(errVL[i-1]>0 || errVL[i-1]<=0))
	{
    cout<<"bin :"<<i<<" x: "<<binc[i-1]<<", y: "<<xval<<" -"<<errXL<<" +" << errXH<<endl;
		cutV[i-1]   = 0.;
		errVL[i-1]   = 0.;
		errVH[i-1]   = 0.;
		binc[i-1] = -100.;
	}
	if (!(xval>0 || xval<=0))
	{
    cout<<"bin :"<<i<<" x: "<<binc[i-1]<<", y: "<<xval<<" -"<<errXL<<" +" << errXH<<endl;
		cutV[i-1]   = 0.;
		errVL[i-1]   = 0.;
		errVH[i-1]   = 0.;
		binc[i-1] = -100.;
	}
	if (xval<0.)
	{
    cout<<"bin :"<<i<<" x: "<<binc[i-1]<<", y: "<<xval<<" -"<<errXL<<" +" << errXH<<endl;
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
  TString fitResult1;

	float fitmax = 40;
	string fitname = ("linfit"+to_string(region));
	TGraphAsymmErrors * ScalingLin = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);
  	TF1 *fnlin = new TF1(fitname.c_str(),"[0]*x + [1]",fitmin,fitmax);
	fnlin->SetParameters(0.1,4.);
	ScalingLin->Fit(fitname.c_str(),"R");
	fitResult1= fnlin->GetExpFormula("P");
     cout<<"doing fit "<< fitname<<endl;
  float puCorr = fnlin->GetParameter(0);

	TCanvas *c3 = new TCanvas(("c"+fitname).c_str(),"Iso Pt",1200,600);
	c3->Divide(2,1);

	c3->cd(1);
	ScalingLin->SetMarkerStyle(24);
	ScalingLin->SetMarkerSize(0.4);
	ScalingLin->Draw("AP");
  ScalingLin->SetTitle(("region "+to_string(region)).c_str());
	ScalingLin->GetXaxis()->SetTitle("#rho");
	ScalingLin->GetYaxis()->SetTitle("HCal deposit 90% contour ");
	c3->cd(2);
	his2->Draw("colz");
	his2->GetXaxis()->SetTitle(" #rho ");
	his2->GetYaxis()->SetTitle("HCal deposit (approx) ");

	c3->SaveAs(("HOE/region"+to_string(region)+"_hoe"+".png").c_str());

  TH2F *hist2d_noise= new TH2F("secondcorrhist","noise removal",19,20,400,1000,0,50);

  for(int i  = 0; i < t_S->GetEntries();i++){

    t_S->GetEntry(i);
    if(ppt < lowPtCut || ppt > 200) continue;
    if(iso_P == 0) continue;
    hist2d_noise->Fill(phoE,ToE*phoE-puCorr*rho);
  }

  TProfile *xprof = hist2d_noise->ProfileX();




  // float fitmax = (region>=2 ? 500 : 700);
  // if(region>=3) fitmax = 400;
  // string fitname = ("quadfit"+to_string(region));
  // TGraphAsymmErrors * IsoptScalingQuad = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);
  TF1 *noisefit = new TF1("noisefit","[0] + [1]*x",0,300);
  noisefit->SetParameters(1.,0.1);
  xprof->Fit("noisefit","R");
  TString fitResult2= noisefit->GetExpFormula("P");
  cout<<"doing fit "<< fitname<<endl;



  TCanvas *c3b = new TCanvas("c_noise","Iso Pt",1200,600);
  c3b->Divide(2,1);

  c3b->cd(1);
  xprof->SetMarkerStyle(24);
  xprof->SetMarkerSize(0.4);
  xprof->GetXaxis()->SetTitle("E");
  xprof->GetYaxis()->SetTitle("H - puCorr * #rho");
  xprof->Draw("AP");
  c3b->cd(2);
  hist2d_noise->Draw("colz");
  hist2d_noise->GetXaxis()->SetTitle("E");
  hist2d_noise->GetYaxis()->SetTitle("H - puCorr * #rho");
  //
  c3b->SaveAs(("HOE/hoe_region"+to_string(region)+"_noise.png").c_str());
  float noiseCorr = noisefit->GetParameter(0);

  cout<<fitResult1.Data()<<endl;
  cout<<fitResult2.Data()<<endl;
  cout<< "---------------"<<endl;



  TFormula outform = TFormula("formula","[0]*x/y +[1]/y");
  outform.SetParameters(puCorr,noiseCorr);
  cout<<"final: "<<outform.GetExpFormula("P").Data()<<endl;

  ofstream myfile;
  myfile.open(("HOE/hoe"+to_string(region)+".txt").c_str());
  myfile<<outform.GetExpFormula("P").Data();
  myfile.close();

}
