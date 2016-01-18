#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <iostream>
#include <string>
#include <vector>
#include <ext/hash_map>

using namespace __gnu_cxx;
using namespace std;

class DataPoint{

public:
	DataPoint();
	DataPoint(string text);
	hash_map<int, float>   features;
	

private:
	inline string getKey(string pair){ return pair.substr(0, pair.find(":"));}
	inline string getValue(string pair){ return pair.substr(pair.find(":")+1);}

};


#endif




