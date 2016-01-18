#ifndef LAMDBAMART_H_
#define LAMDBAMART_H_

#include "rapidxml.hpp"
#include <iostream>
#include <fstream>
#include <ext/hash_map>
#include <stdlib.h>
#include <string>

using namespace std;
using namespace __gnu_cxx;

class LambdaMart{
public:
	LambdaMart();
	LambdaMart(string& filename);
	~LambdaMart();
	void  load(string& filename);
	float  evaluate(hash_map<int, float>& features);

	

private:

	struct Split;
	struct Node;
	class RegressionTree;
	
	string trim(const string& str);
	string readText(string& filename);
	void parse(string &, Node**, float*, int);
	Split* create(rapidxml::xml_node<>* node);
	void preOrder(Split* split, Node* treeArray);

	string filename;
	Node** treeArrays;
	float* weights;
	static int idx;
	int numTrees;
	int numLeaves;
};

#endif


