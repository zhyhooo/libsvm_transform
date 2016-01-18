#include "dataPoint.h"
#include <iostream>


int split(const string& str, vector<string>& ret_, string sep)
{
    if (str.empty())
    {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

DataPoint::DataPoint(){

}

DataPoint::DataPoint(string text){

	vector<string> *fs = new vector<string>;
	split(text, *fs, " ");
	int len = fs->size();
	for(int i=2; i<len; ++i){
		string key = getKey((*fs)[i]);
		string val = getValue((*fs)[i]);


		int f = atoi(key.c_str());
		features[f] = atof(val.c_str());
	}
}
	

