#include "helper.h"
#include <iostream>
#include <numeric>
#include <fstream>
#include <TMath.h>

using namespace TMath;

string FNbody(string filename)
{
	string file(FNfilename(filename));
	return file.substr(0, file.find_first_of("."));
}

string FNfilename(string filename)
{
	return filename.substr(filename.find_last_of("/")+1);
}

string FNpath(string filename)
{
	return filename.substr(0, filename.find_last_of("/"));
}

string FNnoext(string filename)
{
	return filename.substr(0, filename.find_first_of("."));
}

vector<string> dir_content(const string& dirname, unsigned char select)
{
	DIR* dirhandle = opendir(dirname.c_str());

	if(!dirhandle)
	{
		cerr << "Directory " << dirname << " cannot be opened." << endl;
		exit(-1);
	}

	vector<string> filenames;
	struct dirent* dirinfo;
	while((dirinfo = readdir(dirhandle)) != 0)
	{
		//if(dirinfo->d_type & select)
		{
			string filename(dirinfo->d_name);
			filenames.push_back(filename);
		}
	}
	return filenames;
}

vector<string> string_split(const string& in, const vector<string>& splits)
{
	vector<pair<size_t, size_t> > positions;
	positions.push_back(pair<size_t, size_t>(0, 0));
	for(size_t s = 0 ; s < splits.size() ; ++s)
	{
		size_t lastpos = 0;
		while(lastpos < in.size())
		{
			lastpos = in.find(splits[s], lastpos);
			if(lastpos == string::npos)
			{
				break;
			}
			else
			{
				positions.push_back(pair<size_t, size_t>(lastpos, splits[s].size()));
				//lastpos += splits[s].size()+1;
				lastpos += splits[s].size();
			}
		}

	}
	positions.push_back(pair<size_t, size_t>(in.size(), 0));
	sort(positions.begin(), positions.end(), [](const pair<size_t, size_t>& A, const pair<size_t, size_t>& B){return A.first < B.first;});
	vector<string> result;
	for(size_t p = 0 ; p < positions.size()-1 ; ++p)
	{
		size_t begin = positions[p].first + positions[p].second;
		size_t end = positions[p+1].first; 
		if(end != begin)result.push_back(in.substr(begin, end-begin));
	}
	return result;
}

string string_join(const vector<string>& strings, const string& filler)
{
	if(strings.size() == 0) {return string("");}
	size_t newsize = (strings.size()-1)*filler.size();
	for(const string& s : strings) {newsize += s.size();} 
	string newstring(strings[0]);
	newstring.reserve(newsize);
	for(vector<string>::const_iterator it = strings.begin()+1 ; it != strings.end() ; ++it)
	{
		newstring += filler+(*it);
	}
	return newstring;
}

bool string_startswith(const string& in, const string& pattern)
{
	if(pattern.size() > in.size()) {return false;}

	return in.compare(0, pattern.size(), pattern) == 0;

}

int string_startswith(const string& in, const vector<string>& patterns)
{
	int u = 0;
	for(const string& p : patterns)
	{
		if(string_startswith(in, p)) {return u;}
		u++;
	}
	return -1;
}

bool string_endswith(const string& in, const string& pattern)
{
	if(pattern.size() > in.size()) {return false;}

	return in.compare(in.size()-pattern.size(), pattern.size(), pattern) == 0;

}

int string_endswith(const string& in, const vector<string>& patterns)
{
	int u = 0;
	for(const string& p : patterns)
	{
		if(string_endswith(in, p)) {return u;}
		u++;
	}
	return -1;
}

string string_replace(const string& in, const vector<string>& patterns, const string& replacement)
{
	vector<string> ss = string_split(in, patterns);
	if(string_startswith(in, patterns) != -1)
	{
		return replacement + string_join(ss, replacement);
	}
	if(string_endswith(in, patterns) != -1)
	{
		return string_join(ss, replacement)+replacement;
	}
	return string_join(ss, replacement);
}

double ProbToSigma(double prob)
{
	return(TMath::Sqrt(2.) * TMath::ErfInverse(prob));
}

double SigmaToProb(double sigma)
{
	return(TMath::Erf(sigma/TMath::Sqrt(2.)));
}

TH1DCollection::TH1DCollection(string _prefix) : prefix(_prefix) {}

TH1D* TH1DCollection::AddHist(const string& name, Int_t bins, Double_t min, Double_t max, const string& xlabel, const string& ylabel)
{
	string hname(name);
	if(prefix.size() != 0) hname = prefix+"_"+name;
	hmap[name] = new TH1D(hname.c_str(), hname.c_str(), bins, min, max);
	hmap[name]->Sumw2();
	hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
	hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
	return hmap[name];
}

TH1D* TH1DCollection::AddHist(const string& name, const vector<Double_t>& bins, const string& xlabel, const string& ylabel)
{
	string hname(name);
	if(prefix.size() != 0) hname = prefix+"_"+name;
	hmap[name] = new TH1D(hname.c_str(), hname.c_str(), bins.size()-1, bins.data());
	hmap[name]->Sumw2();
	hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
	hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
	return hmap[name];
}

TH1D* TH1DCollection::Get(const string& name)
{
	auto it_pl = hmap.find(name);
	if(it_pl != hmap.end()) return it_pl->second; 
	cerr << "TH1DCollection: Request for unknown hist: " << prefix << " " << name << endl;
	return nullptr;
}

TH1D* TH1DCollection::operator[](const string& name)
{
	return Get(name) ;
}



TH2DCollection::TH2DCollection(string _prefix) : prefix(_prefix) {}

TH2D* TH2DCollection::AddHist(const string& name, Int_t xbins, Double_t xmin, Double_t xmax, Int_t ybins, Double_t ymin, Double_t ymax, const string& xlabel, const string& ylabel)
{
	string hname(name);
	if(prefix.size() != 0) hname = prefix+"_"+name;
	hmap[name] = new TH2D(hname.c_str(), hname.c_str(), xbins, xmin, xmax, ybins, ymin, ymax);
	hmap[name]->Sumw2();
	hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
	hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
	return hmap[name];
}

TH2D* TH2DCollection::AddHist(const string& name, Int_t xbins, Double_t xmin, Double_t xmax, const vector<Double_t>& ybins, const string& xlabel, const string& ylabel)
{
	string hname(name);
	if(prefix.size() != 0) hname = prefix+"_"+name;
	hmap[name] = new TH2D(hname.c_str(), hname.c_str(), xbins, xmin, xmax, ybins.size()-1, ybins.data());
	hmap[name]->Sumw2();
	hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
	hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
	return hmap[name];
}

TH2D* TH2DCollection::AddHist(const string& name, const vector<Double_t>& xbins, Int_t ybins, Double_t ymin, Double_t ymax, const string& xlabel, const string& ylabel)
{
	string hname(name);
	if(prefix.size() != 0) hname = prefix+"_"+name;
	hmap[name] = new TH2D(hname.c_str(), hname.c_str(), xbins.size()-1, xbins.data(), ybins, ymin, ymax);
	hmap[name]->Sumw2();
	hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
	hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
	return hmap[name];
}

TH2D* TH2DCollection::AddHist(const string& name, const vector<Double_t>& xbins, const vector<Double_t>& ybins, const string& xlabel, const string& ylabel)
{
	string hname(name);
	if(prefix.size() != 0) hname = prefix+"_"+name;
	hmap[name] = new TH2D(hname.c_str(), hname.c_str(), xbins.size()-1, xbins.data(), ybins.size()-1, ybins.data());
	hmap[name]->Sumw2();
	hmap[name]->GetXaxis()->SetTitle(xlabel.c_str());
	hmap[name]->GetYaxis()->SetTitle(ylabel.c_str());
	return hmap[name];
}

TH2D* TH2DCollection::Get(const string& name)
{
	auto it_pl = hmap.find(name);
	if(it_pl != hmap.end()) return it_pl->second; 
	cerr << "TH2DCollection: Request for unknown hist: " << prefix << " " << name << endl;
	return nullptr;
}

TH2D* TH2DCollection::operator[](const string& name)
{
	return Get(name);
}

bool operator<(const Bin& A, const Bin& B)
{
    if(B.min() == B.max() && (A.min() <= B.min() && A.max() > B.min()))
    {
        return(false);
    }
    else if(A.min() == A.max() && (B.min() <= A.min() && B.max() > A.min()))
    {
        return(false);
    }
    return A.min() < B.min();
}


////// BEGIN VarianceCalculator ////////////////
VarianceCalculator::VarianceCalculator(int size) : cen_(size, 0.), var_(size)
{
	for(vector<double>& vec : var_) {vec.resize(size, 0.);}
}

void VarianceCalculator::Add(TH1D* hist)
{
	N++;
	for(size_t ba = 0 ; ba < cen_.size() ; ++ba)
	{
		cen_[ba]+= hist->GetBinContent(ba+1);
		for(size_t bb = 0 ; bb < cen_.size() ; ++bb)
		{
			var_[ba][bb] += hist->GetBinContent(ba+1)*hist->GetBinContent(bb+1);
		}
	}
}

void VarianceCalculator::Add(const TMatrixD& vec)
{
	N++;
	for(size_t ba = 0 ; ba < cen_.size() ; ++ba)
	{
		cen_[ba]+= vec(ba, 0);
		for(size_t bb = 0 ; bb < cen_.size() ; ++bb)
		{
			var_[ba][bb] += vec(ba, 0)*vec(bb, 0);
		}
	}
}

void VarianceCalculator::Add(const Eigen::MatrixXd& vec)
{
	N++;
	for(size_t ba = 0 ; ba < cen_.size() ; ++ba)
	{
		cen_[ba]+= vec(ba, 0);
		for(size_t bb = 0 ; bb < cen_.size() ; ++bb)
		{
			var_[ba][bb] += vec(ba, 0)*vec(bb, 0);
		}
	}
}

void VarianceCalculator::Add(const vector<double>& vec)
{
	N++;
	for(size_t ba = 0 ; ba < cen_.size() ; ++ba)
	{
		cen_[ba]+= vec[ba];
		for(size_t bb = 0 ; bb < cen_.size() ; ++bb)
		{
			var_[ba][bb] += vec[ba]*vec[bb];
		}
	}
}

TMatrixD VarianceCalculator::GetMean() const
{
	TMatrixD mean(cen_.size(), 1);
	for(size_t ba = 0 ; ba < cen_.size() ; ++ba)
	{
		mean(ba, 0) = cen_[ba]/N;
	}
	return mean;
}

TH1D* VarianceCalculator::GetMean(const string& name, const TH1D* hist) const
{
	TMatrixD mean(GetMean());
	TMatrixD var(GetVar());
	TH1D* res = nullptr;
	if(hist == nullptr)
	{
		res = new TH1D(name.c_str(), name.c_str(), cen_.size(), 0, cen_.size());
	}
	else
	{
		res = new TH1D(*hist);
		res->SetName(name.c_str());
		res->Reset();
	}
	for(size_t ba = 0 ; ba < cen_.size() ; ++ba)
	{
		res->SetBinContent(ba+1, mean(ba, 0));
		res->SetBinError(ba+1, sqrt(var(ba, ba)));
	}
	return res;
}

TMatrixD VarianceCalculator::GetVar() const 
{
	TMatrixD var(cen_.size(), cen_.size());
	for(size_t ba = 0 ; ba < cen_.size() ; ++ba)
	{
		for(size_t bb = 0 ; bb < cen_.size() ; ++bb)
		{
			var(ba, bb) =  (var_[ba][bb] - cen_[ba]*cen_[bb]/N)/(N-1);
		}
	}
	return var;
}

TH2D* VarianceCalculator::GetVar(const string& name) const
{
	TMatrixD var(GetVar());
	TH2D* res = new TH2D(name.c_str(), name.c_str(), cen_.size(), 0, cen_.size(), cen_.size(), 0, cen_.size());
	for(size_t ba = 0 ; ba < cen_.size() ; ++ba)
	{
		for(size_t bb = 0 ; bb < cen_.size() ; ++bb)
		{
			res->SetBinContent(ba+1, bb+1, var(ba, bb));
		}
	}
	return res;
}

TH2D* VarianceCalculator::GetCor(const string& name) const
{
	TMatrixD var(GetVar());
	TH2D* res = new TH2D(name.c_str(), name.c_str(), cen_.size(), 0, cen_.size(), cen_.size(), 0, cen_.size());
	for(size_t ba = 0 ; ba < cen_.size() ; ++ba)
	{
		for(size_t bb = 0 ; bb < cen_.size() ; ++bb)
		{
			res->SetBinContent(ba+1, bb+1, var(ba, bb)/sqrt(var(ba,ba)*var(bb,bb)));
		}
	}
	return res;
}
////// END VarianceCalculator ////////////////

//double smooth(double x, const vector<double>& xs, const vector<double>& ys, const vector<double>& ws, double sq)

void smooth(TH1D* hist, TH1D* histout, double width, double vmin , double vmax)
{
	vector<double> xs(hist->GetNbinsX());
	vector<double> ys(hist->GetNbinsX());
	vector<double> ws(hist->GetNbinsX());
	
	int bmin = 1;
	int bmax = hist->GetNbinsX();
	double range = hist->GetXaxis()->GetXmax() - hist->GetXaxis()->GetXmin();
	if(vmin != vmax)
	{
		bmin = hist->FindFixBin(vmin);
		bmax = hist->FindFixBin(vmax);
		range = vmax-vmin;
	}

	for(int b = bmin ; b <= bmax ; ++b)
	{
		xs[b-1] = hist->GetBinCenter(b);
		ys[b-1] = hist->GetBinContent(b);
		ws[b-1] = hist->GetBinError(b);
	}

	double wstep = range/2.;
	double w = wstep;		
	double prob = 1.1;
	double probpre = 1.1;
	while(true)
	{
		double chi2 = 0.;
		int dof = 0;
		for(int b = bmin ; b <= bmax ; ++b)
		{
			double ysm = smooth(histout->GetBinCenter(b), xs, ys, ws, w*w);
			if(hist->GetBinError(b) != 0) {chi2 += pow((ysm - hist->GetBinContent(b))/hist->GetBinError(b),2); dof++;}
//cout << b << ": " << abs(ysm - hist->GetBinContent(b))/hist->GetBinError(b) << endl;
			histout->SetBinContent(b, ysm);
		}
		probpre = prob;
		prob = TMath::Prob(chi2, dof);
	//	cout << chi2 << " " << prob << "/" << dof<< " " << w << endl;
		if(abs(prob - width) < 0.01*width || abs(probpre-prob) < 0.001*prob) {break;}
		wstep/=2.;
		if(prob > width) {w += wstep;}
		else{w -= wstep;}
	}
	//for(int b = bmin ; b <= bmax ; ++b)
	//{
	//	histout->SetBinContent(b, smooth(histout->GetBinCenter(b), xs, ys, ws, width*width));
	//}
}

///////BEGIN CorrelatedRNG /////////////////////////////

void CorrelatedRNG::setup()
{
	TDecompChol M_; 
	M_.SetMatrix(Cov_);
	ready_ = M_.Decompose();
	if(!ready_)
	{
		cerr << "CorrelatedRNG: decomposition not possible." << endl;
		double vmax = 0.;
		for(int r = 0 ; r < out_.GetNrows() ; ++r)
		{   
			vmax = max(vmax, Cov_(r, r));
		}
		for(int r = 0 ; r < out_.GetNrows() ; ++r)
		{   
			if(Cov_(r, r) < vmax/1.E10)
			{
				Cov_(r, r) = vmax/1.E10;
			}
		}
		M_.SetMatrix(Cov_);
		ready_ = M_.Decompose();
		if(!ready_)
		{
			cerr << "CorrelatedRNG: Failure: Add " << vmax/1.E10 << endl;
		}
		cerr << "CorrelatedRNG: Warning: Add " << vmax/1.E10 << endl;
	}
	U_.Transpose(M_.GetU());

}

CorrelatedRNG::CorrelatedRNG(const TMatrixD& C, const TMatrixDSym& Cov) : U_(C.GetNrows(), C.GetNrows()), Cov_(Cov),  C_(C), out_(C.GetNrows(),1)
{
	setup();
}

CorrelatedRNG::CorrelatedRNG(TH1D* hcentral, TH2D* hcov) : U_(hcentral->GetNbinsX(), hcentral->GetNbinsX()), Cov_(hcentral->GetNbinsX()),  C_(hcentral->GetNbinsX(),1), out_(hcentral->GetNbinsX(),1)
{   
	for(int r = 0 ; r < hcentral->GetNbinsX() ; ++r)
	{   
		C_(r, 0) = hcentral->GetBinContent(r+1);
		for(int c = 0 ; c < hcentral->GetNbinsX() ; ++c)
		{   
			Cov_(r, c) = hcov->GetBinContent(r+1, c+1);
		}

	}
	setup();
}

const TMatrixD& CorrelatedRNG::getRNs(double sigma) const
{   
	for(int r = 0 ; r < out_.GetNrows() ; ++r)
	{   
		out_(r, 0) = gRandom->Gaus(0, sigma);
	}

	out_ = U_*out_;
	out_ += C_;

	return out_;
}

///////END CorrelatedRNG /////////////////////////////

Long64_t mem_usage()
{
	ifstream stat_stream("/proc/self/stat",ios_base::in);

	// dummy vars for leading entries in stat that we don't care about
	//
	string pid, comm, state, ppid, pgrp, session, tty_nr;
	string tpgid, flags, minflt, cminflt, majflt, cmajflt;
	string utime, stime, cutime, cstime, priority, nice;
	string O, itrealvalue, starttime;

	// the two fields we want
	//
	unsigned long vsize;
	Long64_t rss;

	stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
		>> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
		>> utime >> stime >> cutime >> cstime >> priority >> nice
		>> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

	stat_stream.close();
	return(rss*sysconf(_SC_PAGE_SIZE));
}
