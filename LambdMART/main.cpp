#include "lambdaMart.h"
#include "dataPoint.h"
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define NUMBER 100

int main(){
	
	string modelfile = "data/rl-model-0520-0616.ERR1000";
	LambdaMart model = LambdaMart(modelfile);

	char datafile[] = "data/rl-test-0520-0616.txt";
	ifstream fin(datafile);
	DataPoint dps[NUMBER];
	string strline;
	for(int i=0;i<NUMBER;i++){
		getline(fin, strline);
		dps[i] = DataPoint(strline);
	}
	

	cout<<"start here..."<<endl;
	struct timeval timeBegin, timeEnd;
	gettimeofday(&timeBegin, NULL);
	
	for(int i=0;i<NUMBER;i++){
		cout<<model.evaluate(dps[i].features)<<endl;
	}	
	gettimeofday(&timeEnd, NULL);
	int seconds = timeEnd.tv_sec - timeBegin.tv_sec;
	int useconds = timeEnd.tv_usec - timeBegin.tv_usec;
	int mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;


	cout<<"milleseconds: "<<mtime<<endl;


	return 0;
}





