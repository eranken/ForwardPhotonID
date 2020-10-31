import ROOT as r
from ROOT import TFile, TH1D, TH2D, TCanvas, TLegend, TTree, gPad, gStyle, gROOT

gROOT.SetBatch(True)

# numEvents=10000000
region = 2
etaMin = 2.5
etaMax = 2.66

fi = TFile('TrainIn/CutTMVAregion'+str(region)+'.root', 'READ')

t_S = fi.Get('t_S')
t_B = fi.Get('t_B')

Sieie_cut = .059
HOE_cut = .027
isoC_cut = 2.089
isoN_cut = 19.7
isoP_cut = 4.162

SigEff = 0.
SigTot = 0.
BgEff = 0.
BgTot = 0.



for entryNum in range(0,t_S.GetEntries()):
	t_S.GetEntry(entryNum)
	eta = getattr(t_S,"Peta")
	if eta < etaMin:
		continue
	if eta > etaMax:
		continue
	isoP = getattr(t_S ,"isoP")
	isoC = getattr(t_S ,"isoC")
	isoN = getattr(t_S ,"isoN")
	pt = getattr(t_S,"Ppt")
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
	if eta < etaMin:
		continue
	if eta > etaMax:
		continue
	isoP = getattr(t_B ,"isoP")
	isoC = getattr(t_B ,"isoC")
	isoN = getattr(t_B ,"isoN")
	pt = getattr(t_B,"Ppt")
	HOE = getattr(t_B,"ToE")
	Sieie = getattr(t_B,"Sieie")
	weight = getattr(t_B ,"weighT")
	if(weight<0):
		print "negative weight B !!!!!!!"

	BgTot+=weight
	if isoN<isoN_cut and isoP < isoP_cut and isoN<isoN_cut and Sieie<Sieie_cut and HOE < HOE_cut:
		BgEff+=weight

gStyle.SetOptStat(0000)


SigEff = SigEff/SigTot
BgEff = BgEff/BgTot

print "Sig Eff", SigEff
print "Bg Eff", BgEff
