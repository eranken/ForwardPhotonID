#include <TMath.h>
#include <cstdlib>
#include <TRandom.h>
#include <TGraph.h>
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
  float genPt,ppt,peta,Sie_ie,iso_P,iso_C,iso_N,to_e,weighT,relevant_iso;
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
  if (NorP == "N") {
  	t_S->SetBranchAddress("isoN",&relevant_iso);
  }
  else t_S->SetBranchAddress("isoP",&relevant_iso);

  TH2F *isoPptS = new TH2F("isoPptS",("Iso "+NorP+" vs Pt").c_str(),250,0,1000,1000,0,100);

  // float *relevant_iso = (NorP == "P" ? &iso_P : &iso_N);
  float lowPtCut = ( region >0 && NorP =="P" ? 25. : 15. );

  for(int i  = 0; i < t_S->GetEntries();i++){
    t_S->GetEntry(i);

    if(ppt < lowPtCut || ppt > 1000) continue;
    if(iso_P == 0) continue;
	if(ppt< 15.) cout<<"WTF!!!"<<endl;

    isoPptS->Fill(ppt,relevant_iso);
  }

  cout<<"Built the 2d HISTOGRAM"<<endl;

  TH2F *his2 = (TH2F*)isoPptS->Clone();

  int dim = his2->GetXaxis()->GetNbins();

  double * cutV;
  double * errVH;
  double * errVL;
  double * binc;
  double * bincerH;
  double * bincerL;
  cutV   = new double[dim];
  errVH   = new double[dim];
  errVL   = new double[dim];
  binc   = new double[dim];
  bincerH = new double[dim];
  bincerL = new double[dim];

  for(int i  = 1; i < dim ; i++){
    double xval = 0;
    double errXH = 0;
    double errXL = 0;

    TH1D *r22 = his2->ProjectionY(" ",i,i+1," ");

    TH1D *h1 = (TH1D*)r22->Clone();
    ErrCalc(h1,i,0.950,xval,errXL,errXH);

    cout<<"bin :"<<i<<" "<<xval<<"-"<<errXL<<"+ " << errXH<<endl;

    cutV[i-1]   = xval;

    errVL[i-1]   = errXL;
    errVH[i-1]   = errXH;

    binc[i-1]   = his2->GetXaxis()->GetBinCenter(i);
    bincerL[i-1] = 0;
    bincerH[i-1] = 0;
  }
  TGraphAsymmErrors * IsoptScaling = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);
  TGraphAsymmErrors * IsoptScaling2 = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);

  // double DownL = his2->GetXaxis()->GetBinCenter(1);
  //double UpperL = his2->GetXaxis()->GetBinCenter(dim);

  gStyle->SetOptFit(1);

  if (fittype=="lin" || fittype=="linear") {
	string fitname = ("linfit"+NorP+to_string(region));
	TGraphAsymmErrors * IsoptScalingLin = new TGraphAsymmErrors(dim,binc,cutV,bincerL,bincerH,errVL,errVH);
  	TF1 *fnlin = new TF1(fitname.c_str(),"[0]*x + [1]",20,700);
	fnlin->SetParameters(0.1,4.);
	IsoptScalingLin->Fit(fitname.c_str(),"R");

	TCanvas *c3 = new TCanvas(("c"+fitname).c_str(),"Iso Pt",1200,600);
	c3->Divide(2,1);

	c3->cd(1);
	IsoptScalingLin->SetMarkerStyle(24);
	IsoptScalingLin->SetMarkerSize(0.4);
	IsoptScalingLin->Draw("AP");
	IsoptScalingLin->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
	IsoptScalingLin->GetYaxis()->SetTitle("Isolation Contour PF::h0 rho corr.");
	c3->cd(2);
	his2->Draw("colz");
	his2->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
	his2->GetYaxis()->SetTitle(("Iso "+NorP+" rho corr.").c_str());

	c3->SaveAs(("HPT/region"+to_string(region)+"_iso"+NorP+".png").c_str());
  }

  // TF1 *fn1 = new TF1("fn1","exp([0]*x + [1])",20,700);
  // TF1 *fn2 = new TF1("fn2","[1]*x + [2]*x*x + [0]",20,700);
  //
  // IsoptScaling->Fit("fn1","R");
  // IsoptScaling2->Fit("fn2","R");
  //
  //
  //
  // //lets fit this graph
  //
  // TCanvas *c4 = new TCanvas("c4","Iso Pt",1200,600);
  // c4->Divide(2,1);
  //
  // c4->cd(1);
  // IsoptScaling2->SetMarkerStyle(24);
  // IsoptScaling2->SetMarkerSize(0.4);
  // IsoptScaling2->Draw("AP");
  // IsoptScaling2->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
  // IsoptScaling2->GetYaxis()->SetTitle("Isolation Contour PF::h0 rho corr.");
  //
  // c4->cd(2);
  // IsoptScalingLin->SetMarkerStyle(24);
  // IsoptScalingLin->SetMarkerSize(0.4);
  // IsoptScalingLin->Draw("AP");
  // IsoptScalingLin->SetTitle("Linear pT Scaling");
  // IsoptScalingLin->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
  // IsoptScalingLin->GetYaxis()->SetTitle("Isolation Contour (PF::h0 #rho-corrected)");
  //
  // c4->SaveAs("HPT/POl.png");
  //
  // TCanvas *c6 = new TCanvas("c6","Iso Pt",1200,600);
  // c6->Divide(2,1);
  // c6->cd(1);
  // IsoptScaling2->SetMarkerStyle(24);
  // IsoptScaling2->SetMarkerSize(0.4);
  // IsoptScaling2->Draw("AP");
  // IsoptScaling2->SetTitle("Quadratic pT Scaling");
  // IsoptScaling2->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
  // IsoptScaling2->GetYaxis()->SetTitle("Isolation Contour (PF::h0 #rho-corrected)");
  // c6->cd(2);
  // his2->Draw("colz");
  // his2->GetXaxis()->SetTitle("Photon Pt GeVc^{-1}");
  // his2->GetYaxis()->SetTitle("Isolation PF::h0  rho corr.");
  // c6->SaveAs("HPT/POL2.png");
  // c6->SaveAs("POL2.C");




}
