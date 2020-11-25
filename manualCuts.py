import ROOT as r
from ROOT import TFile, TH1D, TH2D, TCanvas, TLegend, TTree, gPad, gStyle, gROOT

gROOT.SetBatch(True)

# numEvents=10000000
region = 0
etaMin =0.0
etaMax = 3.0

ptMin = 15.
ptMax = 200.

fi = TFile('TrainIn/CutTMVAregion'+str(region)+'.root', 'READ')

t_S = fi.Get('t_S')
t_B = fi.Get('t_B')

#reg 0
HOE_cut = 0.100481
Sieie_cut = 0.0104794
isoC_cut = 3.84761
isoN_cut = 4.90855
isoP_cut = 0.505464

#POG cuts
HOE_cut = 0.04596
Sieie_cut = 0.0106
isoC_cut = 1.694
isoN_cut = 24.032
isoP_cut = 2.876

#HOE_cut = 0.0513
#Sieie_cut = 0.010267
#isoC_cut = 3.681
#isoN_cut = 8.888
#isoP_cut = 0.538

#reg1
#HOE_cut = .059
#Sieie_cut = .0272
#isoC_cut = 2.089
#isoN_cut = 19.722
#isoP_cut = 4.162

SigEff = 0.
SigTot = 0.
BgEff = 0.
BgTot = 0.



for entryNum in range(0,t_S.GetEntries()):
	t_S.GetEntry(entryNum)
	eta = getattr(t_S,"Peta")
	pt = getattr(t_S,"Ppt")
	if eta < etaMin:
		continue
	if eta > etaMax:
		continue
	if pt < ptMin:
		continue
	if pt > ptMax:
		continue
	isoP = getattr(t_S ,"isoP")
	isoC = getattr(t_S ,"isoC")
	isoN = getattr(t_S ,"isoN")
	HOE = getattr(t_S,"ToE")
	Sieie = getattr(t_S,"Sieie")
	weight = getattr(t_S ,"weighT")
	if(weight<0):
		print "negative weight S !!!!!!!"

	SigTot+=weight
	if isoN<isoN_cut and isoP < isoP_cut and isoN<isoN_cut and Sieie<Sieie_cut and HOE < HOE_cut:
		SigEff+=weight


#for entryNum in range(0,numEvents):
for entryNum in range(0,t_B.GetEntries()):
	t_B.GetEntry(entryNum)
	eta = getattr(t_B,"Peta")
	pt = getattr(t_B,"Ppt")
	if eta < etaMin:
		continue
	if eta > etaMax:
		continue
	if pt < ptMin:
		continue
	if pt > ptMax:
		continue
	isoP = getattr(t_B ,"isoP")
	isoC = getattr(t_B ,"isoC")
	isoN = getattr(t_B ,"isoN")
	HOE = getattr(t_B,"ToE")
	Sieie = getattr(t_B,"Sieie")
	weight = getattr(t_B ,"weighT")
	if(weight<0):
		print "negative weight B !!!!!!!"

	BgTot+=weight
	if isoN<isoN_cut and isoP < isoP_cut and isoC<isoC_cut and Sieie<Sieie_cut and HOE < HOE_cut:
		BgEff+=weight

gStyle.SetOptStat(0000)


SigEff = SigEff/SigTot
BgEff = BgEff/BgTot
ratio = SigEff/BgEff

print "Sig Eff", SigEff
print "Bg Eff", BgEff
print "ratio", ratio
