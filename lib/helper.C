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
