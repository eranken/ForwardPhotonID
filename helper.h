#ifndef HELPER_H
#define HELPER_H
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iostream>

#include <TH1D.h>
#include <TH2D.h>
#include <TMatrixD.h>
#include <TDecompChol.h>
#include <TRandom3.h>
#include <TLorentzVector.h>
#include <TMath.h>

#include <sys/types.h>
#include <dirent.h>

//#include </usr/include/eigen3/Eigen/Core>

using namespace std;

//Path and Filenames
string FNbody(string filename);
string FNfilename(string filename);
string FNpath(string filename);
string FNnoext(string filename);

vector<string> dir_content(const string& dirname, unsigned char select = DT_REG|DT_DIR|DT_LNK);

vector<string> string_split(const string& in, const vector<string>& splits = {" "});

string string_join(const vector<string>& strings, const string& filler = "");

string string_replace(const string& in, const vector<string>& patterns, const string& replacement);
bool string_startswith(const string& in, const string& pattern);
int string_startswith(const string& in, const vector<string>& patterns);
bool string_endswith(const string& in, const string& pattern);
int string_endswith(const string& in, const vector<string>& patterns);

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
				std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
				std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}


double ProbToSigma(double prob);
double SigmaToProb(double sigma);

//Histgramm Collections

class TH1DCollection
{
	private:
		string prefix;
		unordered_map<string, TH1D*> hmap;
	public:
		TH1DCollection(string _prefix);
		TH1DCollection() {}
		TH1D* AddHist(const string& name, Int_t bins, Double_t min, Double_t max, const string& xlabel = "", const string& ylabel = "");
		TH1D* AddHist(const string& name, const vector<Double_t>& bins, const string& xlabel = "", const string& ylabel= "");

		TH1D* Get(const string& name);
		TH1D* operator[](const string& name);
};

class TH2DCollection
{
	private:
		string prefix;
		unordered_map<string, TH2D*> hmap;
	public:
		TH2DCollection(string _prefix);
		TH2DCollection() {}
		TH2D* AddHist(const string& name, Int_t xbins, Double_t xmin, Double_t xmax, Int_t ybins, Double_t ymin, Double_t ymax, const string& xlabel = "", const string& ylabel = "");
		TH2D* AddHist(const string& name, Int_t xbins, Double_t xmin, Double_t xmax, const vector<Double_t>& ybins, const string& xlabel = "", const string& ylabel = "");
		TH2D* AddHist(const string& name, const vector<Double_t>& xbins, Int_t ybins, Double_t ymin, Double_t ymax, const string& xlabel = "", const string& ylabel = "");
		TH2D* AddHist(const string& name, const vector<Double_t>& xbins, const vector<Double_t>& ybins, const string& xlabel = "", const string& ylabel = "");

		TH2D* Get(const string& name);
		TH2D* operator[](const string& name);
};

//Statistics

template<typename T> T ME(const T& m){return m;}

template<class InputIt, class UnaryFunction>
pair<double, double> mean_var(InputIt first, InputIt last, UnaryFunction f = ME)
{
    int n = 0;
    double mean = 0.;
    for(InputIt it = first ; it != last ; ++it)
    {
        mean += f(*it);
        n++;
    }
    if(n == 0) {return pair<double, double>(0., 0.);}
    mean /= n;
    if(n == 1) {return pair<double, double>(mean, 0.);}
    double var = 0.;
    for(InputIt it = first ; it != last ; ++it)
    {
        double tmp = (f(*it) - mean);
        var += tmp*tmp;
    }
    var /= n-1;
    return pair<double, double>(mean, var);
}

class Mean_Var
{
private:
	int n_ = 0;
	double sum_ = 0.;
	double sumq_ = 0.;

public:
	void Reset() {n_ = 0; sum_ = 0.; sumq_ = 0.;}
	void Add(double val) {n_++; sum_ += val; sumq_ += val*val;}
	int N() const {return n_;}
	double Mean() const {return sum_/n_;} 
	double Var() const {return (sumq_ - sum_*sum_/n_)/(n_ - 1.);} 
	double Var(double mu) const {return (sumq_ - 2*mu*sum_+ n_*mu*mu)/n_;} 
	double STD() const {return sqrt(Var());} 
	double STD(double mu) const {return sqrt(Var(mu));} 
};



template<typename T> T stringtotype(string s)
{   
    T i;
    istringstream(s) >> i;
    return(i);
}

//BINNER
class Bin
{
    private:
        double min_;
        double max_;
    public:
        Bin(double min, double max) : min_(min), max_(max) {}
        Bin(double val) : min_(val), max_(val) {}

        double min() const {return min_;}
        double max() const {return max_;}

};

bool operator<(const Bin& A, const Bin& B);


template <typename VEC>
double smooth(double x, const VEC& xs, const VEC& ys, const VEC& ws, double sq)
{
	double Sx = 0.;
	double Sxx = 0.;
	double Syx = 0.;
	double Sy = 0.;
	double S = 0.;

	for(size_t i = 0 ; i < (size_t)xs.size() ; ++i)
	{
		if(ws[i] == 0.) {continue;}

		double d = exp(-0.5*(xs[i]-x)*(xs[i]-x)/sq);
		double wq = ws[i]*ws[i];

		Sx+=d/wq*xs[i];
		Sxx+=d/wq*xs[i]*xs[i];
		Syx+=d/wq*ys[i]*xs[i];
		Sy+=d/wq*ys[i];
		S+=d/wq;
	}


	double det = Sxx*S-Sx*Sx;
	if(det == 0.) {return 0.;}

	double a = (Syx*S-Sx*Sy)/det;
	double b = (Sxx*Sy-Sx*Syx)/det;

	return (a*x+b);
}

void smooth(TH1D* hist, TH1D* histout, double width, double vmin = 0., double vmax = 0.);

class CorrelatedRNG
{   
    private:
        TMatrixD U_;
        TMatrixDSym Cov_;
        TMatrixD C_;
        mutable TMatrixD out_;
        bool ready_ = false;

		void setup();

    public:
		CorrelatedRNG(const TMatrixD& C, const TMatrixDSym& Cov);
        CorrelatedRNG(TH1D* hcentral, TH2D* hcov);
        
        const TMatrixD& getRNs(double sigma = 1.) const;

		const TMatrixDSym& GetCov() const {return Cov_;}
		const TMatrixD& GetCentral() const {return C_;}

};

Long64_t mem_usage();

template <typename T>
class ocontainer
{
	private:
		T* data_ = nullptr;
		size_t capacity_ = 0;
		int position_ = 0;
	public:
		void reset(size_t capacity)
		{
			if(capacity_ < capacity)
			{
				if(data_ != nullptr) {delete[] data_;}
				data_ = new T[capacity];
				capacity_ = capacity;
			}
			position_ = 0;
		}
		void push_back(const T& obj)
		{
			data_[position_] = obj;
			position_++;
		}

		T& back() {return data_[position_-1];}
		const T& back() const {return data_[position_-1];}
		T& front() {return data_[0];}
		const T& front() const {return data_[0];}

		T* begin() {return data_;}
		T* end() {return data_+position_;}
		const T* begin() const {return data_;}
		const T* end() const {return data_+position_;}

};


#endif
