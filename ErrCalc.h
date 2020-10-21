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

void ErrCalc(TH1D *HIST,int binxn,double perc,double & X_val, double & errXL,double & errXH){

  // double perc = 0.95;  // efficiency contour value
  //int binxn = 0;       // name of the bin to be scanned

  // Naming the output
  ostringstream gn;
  gn << binxn;
  string gnm = "Efficiency_CutVal"+gn.str()+".png";
  const char *graphname = gnm.c_str();
  //


  /*   TEST CODE
  TH1F *Gaus1 = new TH1F("Gaus1","Gaussian for test",100,0,100);

  TRandom r1;

  for(int i  = 0 ; i < 1000; i++){
    double x = r1.Gaus(30,4);
    Gaus1->Fill(x);
  }
  */

  TH1F *h1 = (TH1F*)HIST->Clone();
  int arsize = h1->GetXaxis()->GetNbins();

  if(h1->GetEntries() < 10 ) {cout<<"empty hist"<<endl; return;};

  // double *eff;
  // double *eff_err;
  // double *cutV;
  // double *cutV_err;

  double* eff      = new double[arsize];
  double* cutV     = new double[arsize];
  double* eff_err  = new double[arsize];
  double* cutV_err = new double[arsize];
  h1->Draw();
  int tot = h1->GetEntries();
  float tot_nz = tot - h1->GetBinContent(1);
  // cout<<tot<<endl;
    int integ = 0;
    for(int i  = 1; i < (h1->GetXaxis()->GetNbins() + 1); i++){
      double xCut = h1->GetXaxis()->GetBinLowEdge(i);
      integ += h1->GetBinContent(i);
      if(integ != 0 && tot != 0 ){
	eff[i -1] = (integ*1.0/tot);
	//eff_err[i -1] = ((integ*1.0/tot)*sqrt(pow(sqrt(tot)/tot,2) + pow(sqrt(integ)/integ,2) ));
	eff_err[i -1] = sqrt ( eff[i-1] * ( 1 - eff[i-1] ) / tot_nz ) ;
      }else{
	eff_err[i -1] = 0;
	eff[i -1] = 0;
      }

      cutV[i -1] = (xCut);
      cutV_err[i - 1] = 0;
      //  cout<<"bin :"<<i<<"  % :"<<integ*1.0/tot<<" cut val :"<<xCut <<endl;
    }


    gStyle->SetOptStat(1);

    //Now draw the resulting curve


    double x[2] = {h1->GetXaxis()->GetBinCenter(1),h1->GetXaxis()->GetBinCenter(arsize)};
    double y[2] = {perc,perc};

    TGraph *lineP =  new TGraph(2,x,y);
    lineP->SetLineColor(kRed);

    TGraphErrors *efC = new TGraphErrors(arsize ,cutV,eff,cutV_err,eff_err);
    efC->SetMarkerStyle(20);
    efC->SetMarkerSize(0.5);

    //   efC->Draw("AP");
    efC->GetXaxis()->SetTitle("Cut Value");
    efC->GetYaxis()->SetTitle("Efficiency");




    TMultiGraph *GRPhs = new TMultiGraph();
    GRPhs->Add(lineP,"l");
    GRPhs->Add(efC,"p");

	//How about let's not make 200 TCanvases
	// NO DRAW
	// TCanvas *c1 = new TCanvas("c1","The Eff- cut value plot",1200,600);
    // c1->Divide(2,1);
    // c1->cd(1);
    // GRPhs->Draw("APL");
    // c1->cd(2);
    // h1->Draw();
    // c1->SaveAs(graphname);

    double * err_up;
    double * err_down;

    err_up = new double [arsize];
    err_down = new double [arsize];

    for(int i = 0;  i < arsize ; i++){
      err_up[i] = eff[i] + eff_err[i];
      err_down[i] = eff[i] - eff_err[i];
      // cout<< "i: "<<i <<" "<<eff[i] <<" "<<err_up[i]<<"  "<<err_down[i]<<endl;
    }



    //Extrapolation method to find the CutValue errors
    int up = -99;
    int down = -99;
    for( int i  = 0; i < arsize ; i++){
      if( err_up [i] > perc){ up = i;
      	break;
      }
	  // cout<<"err "<<i<<" of "<<arsize<<" "<<err_up[i]<<endl;
    }
    for( int i  = 0; i < arsize ; i++){
      if( err_down [i] > perc){
	down = i;
	break;
      }
    }

    // here it returns the extrapolated result

    double Usl   = (err_up[up] - err_up[up-1])/(cutV[up] - cutV[up-1]);
	// cout<<"stupid EGM "<<up<<", "<<cutV[up] <<", "<<cutV[up-1]<<endl;
    double Ustom = err_up[up] - Usl*cutV[up];
    double err_1 = (perc - Ustom)/Usl;


    double Dsl   = (err_down[down] - err_down[down-1])/(cutV[down] - cutV[down-1]);
    double Dstom = err_down[down] - Dsl*cutV[down];
    double err_2 = (perc - Dstom)/Dsl;


    // cout<<"down err:"<<err_1<<endl;
    // cout<<"UP err:"<<err_2<<endl;
    //cout<<"cut value"<<(err_1+err_2)/2.;
    //cout<<"sym err:"<< (err_2-err_1)/2.;

    int xc = 0;

    for(int i = 0; i < arsize; i++){
      if( eff[i] > perc ){
	xc = i;
	break;
      }
    }



    if(fabs(eff[xc] - perc) > fabs(eff[xc - 1] - perc) ){
      X_val = cutV[xc -1 ];

    }else{
      X_val = cutV[xc ];

    }

    // X_val = (err_1+err_2)/2.;

    //checking case that the lines do not cross !

    if(up == -99){
      errXL = X_val - h1->GetXaxis()->GetBinCenter(1);
    }else{
      errXL = X_val - err_1;
    }

    if(down == -99){
      // errXL = ( h1->GetXaxis()->GetBinCenter(arsize) - X_val );
      int dif = 0;
      for(int i = 0; i < arsize; i++){
	if(eff[i] >  0.98){
	  dif = i;
	  break;
	}
      }
      errXH = (cutV[dif] - X_val );
    }else{
       errXH = err_2 - X_val;
    }

	if (!(X_val>0 || X_val<=0)) {
		cout <<"xval NAN"<<endl;
		X_val = -1.;
		errXL = 0.;
		errXH = 0.;
		return;
	}
	if (!(errXL>0 || errXL<=0)) {
		cout <<"xerr_L NAN"<<endl;
		X_val = -1.;
		errXL = 0.;
		errXH = 0.;
		return;
	}
	if (!(errXH>0 || errXH<=0)) {
		cout <<"xerr_H NAN"<<endl;
		X_val = -1.;
		errXL = 0.;
		errXH = 0.;
		return;
	}



    //Deleting all the dynamical arrays


    delete[] eff;
    delete[] cutV;

    delete[] eff_err;
    delete[] cutV_err;

    delete[] err_up;
    delete[] err_down;


}
