import ROOT as r
from ROOT import TFile, TH1D, TH2D, TCanvas, TLegend, TTree, gPad, gStyle, gROOT

gROOT.SetBatch(True)

# numEvents=10000000

ptMin = 15.
ptMax = 200.

vars ={}
vars['eta']=['Peta',120,-3.0,3.0]
vars['pt']=['Ppt',37,15,200]
vars['nvtx']=['Nvtx',79,1,80]

# S iC iP iN H

cutvals = {}
#mine
#cutvals['0']=[0.01048,3.84761,0.538,4.90855,.100481]
#pog
cutvals['0']=[0.0106,1.694,2.876,24.032,0.04596]


cuts ={}
#reg 0
outfile = TFile('POGeffs/POGeff.root','RECREATE')

for region in cutvals:
	fi = TFile('TrainIn/CutTMVAregion'+str(region)+'.root', 'READ')

	histsSpass ={}
	histsBpass ={}
	histsSall ={}
	histsBall ={}

	CVhistsSpass ={}
	CVhistsBpass ={}
	CVhistsSall ={}
	CVhistsBall ={}

	cuts['Sieie']=cutvals[region][0]
	cuts['isoC']=cutvals[region][1]
	cuts['isoP']=cutvals[region][2]
	cuts['isoN']=cutvals[region][3]
	cuts['ToE']=cutvals[region][4]

	Sieie_cut = cutvals[region][0]
	isoC_cut = cutvals[region][1]
	isoP_cut = cutvals[region][2]
	isoN_cut = cutvals[region][3]
	HOE_cut = cutvals[region][4]

	for key in vars:
		histsSpass[key]= TH1D(key+'_'+region+'_'+'S','efficiency vs. '+key+', region '+region,vars[key][1],vars[key][2],vars[key][3])
		histsSall[key]= TH1D(key+'_'+region+'_'+'S_all',key+'_'+region+'_'+'S_all',vars[key][1],vars[key][2],vars[key][3])
		histsBpass[key]= TH1D(key+'_'+region+'_'+'B','efficiency vs. '+key+', region '+region,vars[key][1],vars[key][2],vars[key][3])
		histsBall[key]= TH1D(key+'_'+region+'_'+'B_all',key+'_'+region+'_'+'B_all',vars[key][1],vars[key][2],vars[key][3])
		histsSpass[key].GetYaxis().SetTitle('Efficiency')
		histsBpass[key].GetYaxis().SetTitle('Efficiency')
		histsSpass[key].GetXaxis().SetTitle(key)
		histsBpass[key].GetXaxis().SetTitle(key)
		for key2 in cuts:
			histsSpass[key+key2]= TH1D(key+'_'+key2+'only_'+region+'S',key2+'-only efficiency vs. '+key+', region '+region,vars[key][1],vars[key][2],vars[key][3])
			histsSall[key+key2]= TH1D(key+'_'+key2+'only_'+region+'S_all',key+key2+region+'S_all',vars[key][1],vars[key][2],vars[key][3])
			histsBpass[key+key2]= TH1D(key+'_'+key2+'only_'+region+'B',key2+'-only efficiency vs. '+key+', region '+region,vars[key][1],vars[key][2],vars[key][3])
			histsBall[key+key2]= TH1D(key+'_'+key2+'only_'+region+'B_all',key+key2+region+'B_all',vars[key][1],vars[key][2],vars[key][3])
			histsSpass[key+key2].GetYaxis().SetTitle('Efficiency')
			histsBpass[key+key2].GetYaxis().SetTitle('Efficiency')
			histsSpass[key+key2].GetXaxis().SetTitle(key)
			histsBpass[key+key2].GetXaxis().SetTitle(key)

	t_S = fi.Get('t_S')
	t_B = fi.Get('t_B')

	SigEff = 0.
	SigTot = 0.
	BgEff = 0.
	BgTot = 0.

	varvals = {}
	cutvarvals = {}

	for entryNum in range(0,t_S.GetEntries()):
		t_S.GetEntry(entryNum)
		for key in vars:
			varvals[key]=getattr(t_S,vars[key][0])
		# if varvals["pt"] < ptMin:
		# 	continue
		# if varvals["pt"] > ptMax:
		# 	continue

		for cutvar in cuts:
			cutvarvals[cutvar]=getattr(t_S ,cutvar)
		isoP = getattr(t_S ,"isoP")
		isoC = getattr(t_S ,"isoC")
		isoN = getattr(t_S ,"isoN")
		HOE = getattr(t_S,"ToE")
		Sieie = getattr(t_S,"Sieie")
		weight = getattr(t_S ,"weighT")
		if(weight<0):
			print "negative weight S !!!!!!!"

		for key in vars:
			histsSall[key].Fill(varvals[key],weight)
			for key2 in cuts:
				histsSall[key+key2].Fill(varvals[key],weight)

		if isoN<isoN_cut and isoP < isoP_cut and isoC<isoC_cut and Sieie<Sieie_cut and HOE < HOE_cut:
			for key in vars:
				histsSpass[key].Fill(varvals[key],weight)

		for key2 in cuts:
			if cutvarvals[key2]<cuts[key2]:
				for key in vars:
					histsSpass[key+key2].Fill(varvals[key],weight)

	for key in histsSpass:
		histsSpass[key].Divide(histsSpass[key],histsSall[key],1.,1.,"B")
		histsSpass[key].SetDirectory(outfile)
		# histsSpass[key].Write(outfile)
	#for entryNum in range(0,numEvents):
	for entryNum in range(0,t_B.GetEntries()):
		t_B.GetEntry(entryNum)
		for key in vars:
			varvals[key]=getattr(t_B,vars[key][0])

		for cutvar in cuts:
			cutvarvals[cutvar]=getattr(t_B ,cutvar)
		isoP = getattr(t_B ,"isoP")
		isoC = getattr(t_B ,"isoC")
		isoN = getattr(t_B ,"isoN")
		HOE = getattr(t_B,"ToE")
		Sieie = getattr(t_B,"Sieie")
		weight = getattr(t_B ,"weighT")
		if(weight<0):
			print "negative weight B !!!!!!!"

		for key in vars:
			histsBall[key].Fill(varvals[key],weight)
			for key2 in cuts:
				histsBall[key+key2].Fill(varvals[key],weight)

		if isoN<isoN_cut and isoP < isoP_cut and isoC<isoC_cut and Sieie<Sieie_cut and HOE < HOE_cut:
			for key in vars:
				histsBpass[key].Fill(varvals[key],weight)

		for key2 in cuts:
			if cutvarvals[key2]<cuts[key2]:
				for key in vars:
					histsBpass[key+key2].Fill(varvals[key],weight)
	for key in histsBpass:
		histsBpass[key].Divide(histsBpass[key],histsBall[key],1.,1.,"B")
		histsBpass[key].SetDirectory(outfile)
	# for key in vars:
	# 	histsBpass[key].Divide(histsBpass[key],histsBall[key],1.,1.,"B")
	# 	histsBpass[key].SetDirectory(outfile)
		# histsBpass[key].Write(outfile)

	gStyle.SetOptStat(0000)

	for key in histsSpass:
		print key
		canv= TCanvas(key)
		histsSpass[key].Draw()
		histsSpass[key].SetLineColor(r.kRed)
		histsSpass[key].GetYaxis().SetRangeUser(0.,1.2)
		histsBpass[key].Draw('same')
		histsBpass[key].SetLineColor(r.kBlue)

		leg = TLegend(0.7,0.78,0.9,0.9)
		leg.AddEntry(histsSpass[key],"signal","lep")
		leg.AddEntry(histsBpass[key],"background","lep")
		leg.Draw()
		canv.SaveAs('POGeffs/'+key+region+'POG.png')
		canv.SaveAs('POGeffs/'+key+region+'POG.pdf')




outfile.Write()
outfile.Close()
