
//#define CUTID_cxx
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void CutID::CutBasedID(int reg, double etaLow, double etaHigh, const TH2D* EAhist, string mode, size_t stepsize=1){

  TFile *f1;
  f1 =  new TFile((mode+"/CutTMVAregion"+to_string(reg)+".root").c_str(),"recreate");

  TFormula isoP_form;
  TFormula isoN_form;
  TFormula HoE_form;

  if (mode!="HPT") {
	ifstream isoPfile;
  	ifstream isoNfile;
  	isoPfile.open(("HPT/isoP"+to_string(reg)+".txt"));
  	isoNfile.open(("HPT/isoN"+to_string(reg)+".txt"));
  	TString isoP_formstring;
  	TString isoN_formstring;
  	isoPfile>>isoP_formstring;
  	isoNfile>>isoN_formstring;

  	cout<<isoP_formstring<<endl;
  	cout<<isoN_formstring<<endl;

	isoP_form = TFormula(("isoPreg"+to_string(reg)).c_str(),isoP_formstring);
	isoN_form = TFormula(("isoNreg"+to_string(reg)).c_str(),isoN_formstring);

	
	if (mode=="HoEcorr") {
	   ifstream HoEfile;
  	   HoEfile.open(("HOE/hoe"+to_string(reg)+".txt"));
  	   TString HoE_formstring;
	   HoEfile>>HoE_formstring;
	   HoE_form = TFormula(("HoEreg"+to_string(reg)).c_str(),HoE_formstring);
	   cout<<HoE_formstring<<" "<<HoE_form.Eval(0.,1.,0.)<<endl;
	}

  }


  TTree *t_S = new TTree("t_S","CUT NEEDS THIS  ");
  TTree *t_B = new TTree("t_B","CUT NEEDS THIS ");

  TTree *t_R = new TTree("t_R","CUT NEEDS THIS ");


  TH1F *SieieSw = new TH1F("SieieSw","Sigma ie ie signal pt weighted",1000,0,1);
  TH1F *SieieBw = new TH1F("SieieBw","Sigma ie ie background pt weighted",1000,0,1);

  TH1F *TowSw = new TH1F("TowSw","H o E ie ie signal pt weighted",600,0,6);
  TH1F *TowBw = new TH1F("TowBw","H o E ie ie background pt weighted",600,0,6);

  TH1F *iSOPsw = new TH1F("iSOPsw","Iso gamma  signal pt weighted",300,0,30);
  TH1F *iSOPbw = new TH1F("iSOPbw","Iso gamma  background pt weighted",300,0,30);

  TH1F *iSOCsw = new TH1F("iSOCsw","Iso charge  signal pt weighted",300,0,30);
  TH1F *iSOCbw = new TH1F("iSOCbw","Iso charge  background pt weighted",300,0,30);

  TH1F *iSONsw = new TH1F("iSONsw","Iso neutral  signal pt weighted",300,0,30);
  TH1F *iSONbw = new TH1F("iSONbw","Iso neutral  background pt weighted",300,0,30);

  TH1F *pTs = new TH1F("pTs","Signal Pt ",185,15,200);
  TH1F *pTb = new TH1F("pTb","Bckground Pt",185,15,200);

  TH1F *etaS = new TH1F("etaS","Signal Eta   ",20,-5,5);
  TH1F *etaB = new TH1F("etaB","Bckground Eta",20,-5,5);
  TH1F *etaSw = new TH1F("etaSw","Signal Eta weighted  ",200,-5,5);
  TH1F *etaBw = new TH1F("etaBw","Bckground Eta weighted",200,-5,5);

  TH1F *pTsw = new TH1F("pTsw","Signal Ptpt weighted ",185,15,200);
  TH1F *pTbw = new TH1F("pTbw","Bckground Pt pt weighted",185,15,200);

  TH2D *etaPts = new TH2D("etaPts","Eta vs pt",200,-5,5,185,15,200);
  TH2D *etaPtb = new TH2D("etaPtb","Eta vs pt",200,-5,5,185,15,200);

  TH2D *etaPtsw = new TH2D("etaPtsw","Eta vs pt weighted",200,-5,5,185,15,200);
  TH2D *etaPtbw = new TH2D("etaPtbw","Eta vs pt weighted",200,-5,5,185,15,200);

  float genPt,isoN,isoC,isoP,Peta,Ppt,Pphi,ToE,Sieie,weighT,weighTXS,E_pho,rho;

  int Pix,Nvtx;


  t_S->Branch("Peta",&Peta,"Peta/F");
  t_S->Branch("Pphi",&Pphi,"Pphi/F");
  t_S->Branch("Ppt",&Ppt,"Ppt/F");
  t_S->Branch("isoN",&isoN,"isoN/F");
  t_S->Branch("isoP",&isoP,"isoP/F");
  t_S->Branch("isoC",&isoC,"isoC/F");

  t_S->Branch("Nvtx",&Nvtx,"Nvtx/I");
  t_S->Branch("genPt",&genPt,"genPt/F");
  t_S->Branch("Sieie",&Sieie,"Sieie/F");
  t_S->Branch("ToE",&ToE,"ToE/F");
  t_S->Branch("Pix",&Pix,"Pix/I");
  t_S->Branch("weighT",&weighT,"weightT/F");
  t_S->Branch("weighTXS",&weighTXS,"weightTXS/F");
  t_S->Branch("phoE",&E_pho,"phoE/F");
  t_S->Branch("Rh",&rho,"Rh/F");



  //---------Background Tree---------------

  t_B->Branch("Peta",&Peta,"Peta/F");
  t_B->Branch("Pphi",&Pphi,"Pphi/F");
  t_B->Branch("Ppt",&Ppt,"Ppt/F");
  t_B->Branch("isoN",&isoN,"isoN/F");
  t_B->Branch("isoP",&isoP,"isoP/F");
  t_B->Branch("isoC",&isoC,"isoC/F");




  t_B->Branch("Sieie",&Sieie,"Sieie/F");
  t_B->Branch("ToE",&ToE,"ToE/F");
  t_B->Branch("Pix",&Pix,"Pix/I");
  t_B->Branch("weighT",&weighT,"weightT/F");
  t_B->Branch("Nvtx",&Nvtx,"Nvtx/I");
  t_B->Branch("weighTXS",&weighTXS,"weightTXS/F");
  t_B->Branch("phoE",&E_pho,"phoE/F");

  t_B->Branch("Rh",&rho,"Rh/F");


  ///////////////////////////////////////////////////////////////////////////////

  //  ----------------  EVENT LOOP BEGINING --------------

  ///////////////////////////////////////////////////////////////////////////////

  // this first loop on the events is for deriving the weights of the trainning
  // so we are only interested on the pt distribution of back/ signal events


  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  //Long64_t nentries = 100;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++){
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    //Geetign the weights regarding the Pt for each event
    //Pt Cut
    //    if(gedPhPt < 15 || gedPhPt > 200)continue;
    if(gedPhPt< 15) continue; // || gedPhPt > 200)continue;
	//Eta barrel endcap def
	if(fabs(gedPhEta)<etaLow || fabs(gedPhEta)>etaHigh) continue;


    if(gedPhisPrompt){
      pTs->Fill(gedPhPt,gedPhweightXS);
      etaS->Fill(gedPhEta,gedPhweightXS);
      etaPts->Fill(gedPhEta,gedPhPt,gedPhweightXS);

    }
    if(gedPhisPrompt == 0){
      pTb->Fill(gedPhPt,gedPhweightXS);
      etaB->Fill(gedPhEta,gedPhweightXS);
      etaPtb->Fill(gedPhEta,gedPhPt,gedPhweightXS);
    }
  }
  cout<<"end of first loop on events"<<endl;

//  MAKE SMALL FILES HERE
    //for (Long64_t jentry=0; jentry<nentries;jentry++){

  
    for (Long64_t jentry=0; jentry<nentries;jentry+=stepsize){
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    //if(gedPhPt < 15 || gedPhPt > 200   ) continue;
    if(gedPhPt < 15)continue;
    //---------------------------------------------------------------------
    //      CALC OF THE RHO SUB - ISOLATION
    //---------------------------------------------------------------------
    double EAph = 0;
    double EAneu = 0;
    double EAch = 0;

    //Setting up the effective areas
    double eta = gedPhEta;
	int biin = EAhist->GetYaxis()->FindFixBin(fabs(eta));
	if (biin>EAhist->GetNbinsY()) {
		continue;
		// biin=EAhist->GetNbinsY();
	}




	EAch  = EAhist->GetBinContent(1,biin);
	EAneu = EAhist->GetBinContent(2,biin);
    EAph  = EAhist->GetBinContent(3,biin);
	//cout<<"eta: "<< to_string(fabs(eta)) << " biin: "<<to_string(biin)<< " isoP slope: "<<EAph<<endl;

    float PhI = gedPhoIso;
    float ChgI = gedChgIso;
    float NeuI = gedNeuIso;

    PhI = max( PhI - Rh*EAph,0.0);
    ChgI = max( ChgI - Rh*EAch,0.0);
    NeuI = max( NeuI - Rh*EAneu,0.0);

	if (mode!="HPT") {
		PhI = max( PhI +isoP_form.Eval(0.) -isoP_form.Eval(gedPhPt),0.0);
		NeuI = max( NeuI +isoN_form.Eval(0.) -isoN_form.Eval(gedPhPt),0.0);
	}



    //EOF THE ISOLATION CALCULATION



    // if(reg == 1 && fabs(gedPhEta) > 1.4442 ) continue;
    // if(reg == 2 && ( fabs(gedPhEta) < 1.566 || fabs(gedPhEta) > 3.  )) continue;
	//Eta barrel endcap def
	if(fabs(gedPhEta)<etaLow || fabs(gedPhEta)>etaHigh) continue;


    if(gedPhisPrompt){
      Peta=gedPhEta;
      Pphi=gedPhPhi;
      Ppt=gedPhPt;
      Pix = gedPhPixSeed;
      isoP = PhI;
      isoC = ChgI;
      isoN = NeuI;

      Sieie =gedPhSieie ;
      ToE = gedPhTower;
      genPt = gedGenPt;
      Nvtx = NVtx;

      rho = Rh;
      E_pho = gedPhPt*TMath::CosH(gedPhEta);

      
	if (mode=="HoEcorr") {
		ToE = max( ToE - HoE_form.Eval(rho, E_pho,1.),0.0);
	}

      int binx = etaPts->FindBin(gedPhEta,gedPhPt);
      weighT = gedPhweightXS*( ( etaPts->GetBinContent(binx) == 0  ) ? 0.0  : 1./etaPts->GetBinContent(binx));
      weighTXS = gedPhweightXS;
      SieieSw->Fill(Sieie,weighT);
      TowSw->Fill(ToE,weighT);
      iSOPsw->Fill(isoP,weighT);
      iSONsw->Fill(isoN,weighT);
      iSOCsw->Fill(isoC,weighT);
      pTsw->Fill(Ppt,weighT);
      etaSw->Fill(Peta,weighT);
      etaPtsw->Fill(Peta,Ppt,weighT);
      t_S->Fill();
    }


    if(gedPhisPrompt == 0){

      Peta = gedPhEta;
      Pphi = gedPhPhi;
      Ppt  = gedPhPt;
      Pix  = gedPhPixSeed;

      isoP = PhI;
      isoC = ChgI;
      isoN = NeuI;

      Sieie = gedPhSieie ;
      ToE   = gedPhTower;
      Nvtx  = NVtx;

      rho = Rh;
      E_pho = gedPhPt*TMath::CosH(gedPhEta);

	if (mode=="HoEcorr") {
		ToE = max( ToE - HoE_form.Eval(rho, E_pho,1.),0.0);
	}

      int binx = etaPtb->FindBin(gedPhEta,gedPhPt);
      weighT = gedPhweightXS*(( etaPtb->GetBinContent(binx) == 0  ) ? 0.0  : 1./etaPtb->GetBinContent(binx));
      weighTXS = gedPhweightXS;
      t_B->Fill();

      SieieBw->Fill(Sieie,weighT);
      TowBw->Fill(ToE,weighT);
      iSOPbw->Fill(isoP,weighT);
      iSONbw->Fill(isoN,weighT);
      iSOCbw->Fill(isoC,weighT);
      pTbw->Fill(Ppt,weighT);
      etaBw->Fill(Peta,weighT);
      etaPtbw->Fill(Peta,Ppt,weighT);

    }



  }//eof the event loop


  t_S->Write();
  t_B->Write();
  t_R->Write();


  SieieSw->Write();
  TowSw->Write();
  iSOPsw->Write();
  iSONsw->Write();
  iSOCsw->Write();

  SieieBw->Write();
  TowBw->Write();
  iSOPbw->Write();
  iSONbw->Write();
  iSOCbw->Write();

  pTsw->Write();
  pTbw->Write();

  etaSw->Write();
  etaBw->Write();

  etaPts->Write();
  etaPtb->Write();

  etaPtsw->Write();
  etaPtbw->Write();


  f1->Close();
}// loop function end
