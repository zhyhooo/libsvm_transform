#include "lambdaMart.h"


LambdaMart::LambdaMart(){
	
}

LambdaMart::LambdaMart(string& filename){
	load(filename);

}

LambdaMart::~LambdaMart(){
	free(treeArrays);
	free(weights);
}

int LambdaMart::idx = 0;

struct LambdaMart::Split{
	int featureID;
	int nChildren;
	float threshold;
	float avgLabel;
	Split* left;
	Split* right;
};

struct LambdaMart::Node{
	float threshold;
	unsigned short featureID;
	unsigned char leftChild;
	unsigned char rightChild;
};

class LambdaMart::RegressionTree{
public:
	RegressionTree(Split* root):
		root(root),
		nSplits(root->nChildren){
	}
	float eval(hash_map<int, float> &features){
		Split* n = root;
		while(n->featureID != -1)
			n = (features[n->featureID] <= n->threshold) ? n->left: n->right;
		return n->avgLabel;
	}
	int getNumSplits(){ return nSplits;}
	Split* getRoot(){ return root;}
private:
	Split* root;
	int nSplits;
};


// trim space before and after string
string LambdaMart::trim(const string& str){
	string::size_type pos = str.find_first_not_of(' ');
	if (pos == string::npos){
		return str;
	}
	string::size_type pos2 = str.find_last_not_of(' ');
	if (pos2 != string::npos){
		return str.substr(pos, pos2 - pos + 1);
	}
	return str.substr(pos);
}

// read text from xml, return a string
string LambdaMart::readText(string& filename){
	string content	= "";
	string model	= "";

	char *cfilename = new char[filename.length()+1];
	strcpy(cfilename, filename.c_str());
	ifstream fin(cfilename);

	if(fin.is_open() == false){
		cout<<"Fail to open file"<<endl;
		return model;
	}
	
	getline(fin, content);
	this->numTrees = atoi(content.c_str());
	getline(fin, content);
	this->numLeaves = atoi(content.c_str());

	while(getline(fin, content) ){		
		content = trim(content);
		if(content.length()==0)	continue;
		if(content.find("##") == 0)	continue;
		model.append(content);		
	}
	fin.close();
	return model;
}

LambdaMart::Split* LambdaMart::create(rapidxml::xml_node<>* node){
	
	char* str;
	Split* s = new Split();

	str = node->first_node()->name();
	if(strcmp(str, "feature")==0){
		str = node->first_node()->value();
		s->featureID = atoi(str);
		rapidxml::xml_node<>* threshold_node = node->first_node()->next_sibling();
		str = threshold_node->value();
		s->threshold = atof(str); 
		s->nChildren = 1;

		rapidxml::xml_node<>* left = threshold_node->next_sibling();
		rapidxml::xml_node<>* right = left->next_sibling();
		s->left = create(left);
		s->right = create(right);
		s->nChildren += (s->left->nChildren + s->right->nChildren);
		
	}
	else{ //this is a stump
		str = node->first_node()->value();
		s->avgLabel = atof(str);
		s->featureID = -1;
		s->nChildren = 1;
		s->threshold = 0.0;
	}
	return s;	
}


void LambdaMart::preOrder(Split* split, Node* treeArray){
	if(split->featureID != -1){
		int k = idx;
		treeArray[k].threshold = split->threshold;
		treeArray[k].featureID = split->featureID;
		treeArray[k].leftChild = ++idx;
		preOrder(split->left, treeArray);
        treeArray[k].rightChild = ++idx;
		preOrder(split->right, treeArray);
	}else{
		treeArray[idx].threshold = split->avgLabel;
		treeArray[idx].featureID = split->featureID;
		
	}
}

// convert XML file to tree structure
void LambdaMart::parse(string &text, Node** treeArrays, float* weights, int length){
	rapidxml::xml_document<> doc;
	char* modelXML = new char[text.size() + 1];	// Create char buffer to store string copy
	strcpy (modelXML, text.c_str());             // Copy string into char buffer
	doc.parse<0>(modelXML);

	rapidxml::xml_node<>* node = doc.first_node();  //.first_node();//去顶级结点 
	node = node->first_node("tree");


	int k = 0;
	RegressionTree** trees = (RegressionTree**)malloc(length*sizeof(RegressionTree*));
	while(node != NULL && k<length){
		Split* root = create(node->first_node());
		char* str_weight = node->first_attribute("weight")->value();
		weights[k] = atof(str_weight);

		trees[k++] = new RegressionTree(root);
		node = node->next_sibling("tree");
		if(node == 0) 
			break;
	}

	for(int i=0;i<length;i++){
		idx = 0;
		int nNode = trees[i]->getNumSplits();
		Node* treeArray = new Node[nNode];
		memset(treeArray, 0, sizeof(Node)*nNode);
		preOrder(trees[i]->getRoot(), treeArray);
		treeArrays[i] = treeArray;
    }


}

void LambdaMart::load(string& filename){
	string xmlcontent = readText(filename);
	if(numTrees<=0){
		cout<<"Number of trees is not valid!"<<endl;
		return;
	}
	treeArrays  = (Node**)malloc(numTrees*sizeof(Node*));
	weights		= (float*)malloc(numTrees*sizeof(float));
	parse(xmlcontent, treeArrays, weights, numTrees);
}

float LambdaMart::evaluate(hash_map<int, float>& features){
	float score = 0;
	if(treeArrays==NULL || weights==NULL) return score;

	for(int i=0; i<numTrees; ++i){
		Node* node = &treeArrays[i][0];
		while(node->featureID !=65535){
			node = (features[node->featureID] <= node->threshold)? 
			&treeArrays[i][node->leftChild] : &treeArrays[i][node->rightChild];
		}
		score += node->threshold * weights[i];

	}

	return score;
}
