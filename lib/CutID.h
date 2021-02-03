//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 21 10:33:25 2014 by ROOT version 5.34/10
// from TTree t1/EventsTree
// found on file: ../CompatV.root
//////////////////////////////////////////////////////////

#ifndef CutID_h
#define CutID_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>



// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class CutID {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types

   Int_t           NVtx;


   Float_t         Rh;


   Float_t         gedPhEta;
   Float_t         gedGenPt;
   Float_t         gedPhPhi;
   Float_t         gedPhPt;
   Float_t         gedPhSieie;

   Float_t         gedPhTower;
   Int_t           gedPhisPrompt;
   Float_t         gedPhoIso;
   Float_t         gedChgIso;
   Float_t         gedNeuIso;
   Int_t           gedPhPixSeed;
   Float_t         gedPhweightXS;

   Float_t ESC;
   Float_t R9;

   // List of branches

   TBranch        *b_Rh;
   TBranch        *b_NVtx;

   TBranch        *b_gedPhEta; //!
   TBranch        *b_gedGenPt;    //!
   TBranch        *b_gedPhPhi;   //!
   TBranch        *b_gedPhPt;   //!
   TBranch        *b_gedPhSieie;

   TBranch        *b_gedPhTower;   //!
   TBranch        *b_gedPhisPrompt;   //!
   TBranch        *b_gedPhoIso;   //!
   TBranch        *b_gedChgIso;   //!
   TBranch        *b_gedNeuIso;   //!
   TBranch        *b_gedPhPixSeed;   //!
   TBranch        *b_gedPhweightXS;

   TBranch *b_R9;
   TBranch *b_ESC;


   CutID(TTree *tree=0);
   CutID(TTree *tree, const string &inFilePath);
   virtual ~CutID();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   //   virtual void     SaCH(int reg);
   virtual void     ArCalc(double e_min,double e_max,int bin);
   virtual void     CutBasedID(int reg,double etaLow, double etaHigh, const TH2D *EAhist,string outdir, size_t stepsize =1);
};

#endif
