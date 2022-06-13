#ifndef CONFIGPARSER
#define CONFIGPARSER
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <typeinfo>
#include "helper.C"

using namespace std;

string cleanspaces(const string& str)
{
	string nstr;
	for(char l : str)
	{
		if(!isspace(l))
		{
			nstr.push_back(l);
		}
	}
	return(nstr);
}

class ConfigParser
{
	private:

		map< string, string > info;
		vector<string> keys;

	public:

		ConfigParser(string filename)
		{
			fstream infile(filename, ios_base::in);
			string line;
			while(getline(infile, line))
			{
				size_t commentpos = line.find("#");
				if(commentpos != string::npos)
				{
					line = line.substr(0, commentpos);
				}

				size_t eqpos = line.find("=");
				if(eqpos == string::npos) {continue;}

				string parameter = line.substr(0, eqpos);
				string val = line.substr(eqpos+1);

				parameter = cleanspaces(parameter);
				val = cleanspaces(val);
				//cout << parameter << ":" << val << endl;
				info[parameter] = val;
				keys.push_back(move(parameter));
			}
			infile.close();
		}
		const vector<string>& GetKeys() const {return keys;}

		template<typename T>  T Get(string name)
		{
			T i;
			istringstream(info[name]) >> i;
			return(i);
		}
		template<typename T> vector<T> GetVector(string name, const vector<string>& sep = {","})
		{
			vector<T> res;
			vector<string> parts = string_split(info[name], sep);

			    for(string& part : parts)
			{
				trim(part);
				res.resize(res.size()+1);
				istringstream(part) >> res.back();
			}
			return(res);
		}


};






#endif
