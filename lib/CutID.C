#define CutID_cxx
#include "CutID.h"
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "AreaCalc.C"
#include "CutB.C"

CutID::CutID(TTree *tree, const string &inFilePath) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
     //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("sp15_4th.root");
    TFile *f = new TFile(inFilePath.c_str());
	f->GetObject("t1",tree);


   }
   Init(tree);
}

CutID::~CutID()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t CutID::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t CutID::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void CutID::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
   fChain->SetBranchAddress("Rh", &Rh, &b_Rh);

   fChain->SetBranchAddress("gedPhEta", &gedPhEta, &b_gedPhEta);

   fChain->SetBranchAddress("gedGenPt", &gedGenPt, &b_gedGenPt);


   fChain->SetBranchAddress("gedPhPhi", &gedPhPhi, &b_gedPhPhi);
   fChain->SetBranchAddress("gedPhPt", &gedPhPt, &b_gedPhPt);
   fChain->SetBranchAddress("gedPhSieie", &gedPhSieie, &b_gedPhSieie);

   fChain->SetBranchAddress("gedPhTower", &gedPhTower, &b_gedPhTower);
   fChain->SetBranchAddress("gedPhisPrompt", &gedPhisPrompt, &b_gedPhisPrompt);
   fChain->SetBranchAddress("gedPhoIso", &gedPhoIso, &b_gedPhoIso);
   fChain->SetBranchAddress("gedChgIso", &gedChgIso, &b_gedChgIso);
   fChain->SetBranchAddress("gedNeuIso", &gedNeuIso, &b_gedNeuIso);
   fChain->SetBranchAddress("gedPhPixSeed", &gedPhPixSeed, &b_gedPhPixSeed);
   fChain->SetBranchAddress("gedPhweightXS", &gedPhweightXS, &b_gedPhweightXS);
   fChain->SetBranchAddress("R9", &R9, &b_R9);
   fChain->SetBranchAddress("ESC", &ESC, &b_ESC);
   Notify();
}

Bool_t CutID::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void CutID::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t CutID::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


void CutID::Loop()
{



   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
   }
}
