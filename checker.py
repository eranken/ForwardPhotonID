import ROOT as r
from ROOT import TFile, TH1D, TH2D, TCanvas, TLegend, TTree, gPad, gStyle, gROOT

numevents=10000000
region = '2'

fi = TFile('TrainIn/CutTMVAregion'+region+'.root', 'READ')

t_S = fi.Get('t_S')
t_B = fi.Get('t_B')

histP_S=TH1D('IsoP_Shist','Iso P',100,0,6)
histC_S=TH1D('IsoC_Shist','Iso C',100,0,6)
histN_S=TH1D('IsoN_Shist','Iso N',100,0,6)
histHOE_S=TH1D('HOE_Shist','H over E',100,0,0.1)
histSieie_S=TH1D('Sieie_Shist','Sigma i#etai#eta',100,0,0.06)


histP_B=TH1D('IsoP_Bhist','IsoP_B',100,0,6)
histC_B=TH1D('IsoC_Bhist','IsoC_B',100,0,6)
histN_B=TH1D('IsoN_Bhist','IsoN_B',100,0,6)
histHOE_B=TH1D('HOE_Bhist','HOE_B',100,0,0.1)
histSieie_B=TH1D('Sieie_Bhist','Sieie_B',100,0,0.06)

# for entryNum in range(0,t_S.GetEntries()):
for entryNum in range(0,numevents):
	t_S.GetEntry(entryNum)
	eta_S = getattr(t_S,"Peta")
	isoP_S = getattr(t_S ,"isoP")
	isoC_S = getattr(t_S ,"isoC")
	isoN_S = getattr(t_S ,"isoN")
	weight_S = getattr(t_S ,"weighT")
	histC_S.Fill(isoC_S,weight_S)
	histN_S.Fill(isoN_S,weight_S)
	histP_S.Fill(isoP_S,weight_S)
	pt_S = getattr(t_S,"Ppt")
	HOE_S = getattr(t_S,"ToE")
	Sieie_S = getattr(t_S,"Sieie")
	histHOE_S.Fill(HOE_S,weight_S)
	histSieie_S.Fill(Sieie_S,weight_S)

for entryNum in range(0,numevents):
	t_B.GetEntry(entryNum)
	eta_B = getattr(t_B,"Peta")
	isoP_B = getattr(t_B ,"isoP")
	isoC_B = getattr(t_B ,"isoC")
	isoN_B = getattr(t_B ,"isoN")
	weight_B = getattr(t_B ,"weighT")
	histC_B.Fill(isoC_B,weight_B)
	histN_B.Fill(isoN_B,weight_B)
	histP_B.Fill(isoP_B,weight_B)
	pt_B = getattr(t_B,"Ppt")
	HOE_B = getattr(t_B,"ToE")
	Sieie_B = getattr(t_B,"Sieie")
	histHOE_B.Fill(HOE_B,weight_B)
	histSieie_B.Fill(Sieie_B,weight_B)
gStyle.SetOptStat(0000)

cC = TCanvas()
histC_S.Draw('hist')
histC_S.SetLineColor(r.kRed)
histC_S.SetFillColor(r.kRed)
histC_B.Draw('hist same')
histC_B.SetLineColor(r.kBlue)
cC.SetLogy()
cC.SaveAs('tests/isoC_lowEta'+region+'.pdf')

cP = TCanvas()
histP_S.Draw('hist')
histP_S.SetLineColor(r.kRed)
histP_S.SetFillColor(r.kRed)
cP.SetLogy()
histP_B.Draw('hist same')
histP_B.SetLineColor(r.kBlue)
cP.SaveAs('tests/isoP_lowEta'+region+'.pdf')

cN = TCanvas()
histN_S.Draw('hist')
histN_S.SetLineColor(r.kRed)
histN_S.SetFillColor(r.kRed)
cN.SetLogy()
histN_B.Draw('hist same')
histN_B.SetLineColor(r.kBlue)
cN.SaveAs('tests/isoN_lowEta'+region+'.pdf')

cHOE = TCanvas()
histHOE_S.Draw('hist')
histHOE_S.SetLineColor(r.kRed)
histHOE_S.SetFillColor(r.kRed)
cHOE.SetLogy()
histHOE_B.Draw('hist same')
histHOE_B.SetLineColor(r.kBlue)
cHOE.SaveAs('tests/HoE_lowEta'+region+'.pdf')

cSieie = TCanvas()
histSieie_S.Draw('hist')
histSieie_S.SetLineColor(r.kRed)
histSieie_S.SetFillColor(r.kRed)
histSieie_B.Draw('hist same')
histSieie_B.SetLineColor(r.kBlue)
cSieie.SaveAs('tests/Sieie_lowEta'+region+'.pdf')
