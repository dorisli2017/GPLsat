/*
 * main.cpp
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */

#include "main.h"

int main(int argc, char *argv[]){
	parseOptions(argc, argv);
	readFile(argv[1]);
	int size; int rand;int flipVindex,flipCindex;
	for(unsigned int i = 0; i <maxTries;i++){
		initializeAssignment();
		for(unsigned int j = 0; j < maxFlips; j++){
			//debug();
			size =  unsatCs.size();
			if (size == 0){
				debug();
				test();
				return 0;
			}
			rand = randomIndex(size);
			flipCindex = unsatCs[rand];
			if(numP[flipCindex] > 0){
				unsatCs[rand]=unsatCs.back();
				unsatCs.pop_back();
				continue;
			}
			flipVindex = getFlipCandidate(clauses[flipCindex]);
			unsatCs[rand]=unsatCs.back();
			unsatCs.pop_back();
			flip(flipVindex);
		}
	}
	//test();
	debug();
    return 0;
}

void debug(){

	/* Testing code**********************************/
		//printOptions();
	   //printVariables();
	    printClauses();
	   	printAssignment();
	   	printUnsatCs();

	/*Testing code**********************************/


}
/*parse the argument (including options and filename)
 *using getopt_long to allow GNU-style long options as well as single-character options
 */
void parseOptions(int argc, char *argv[]){
	//opterr = 0;
	//The argument longopts must be an array of long option structures.
	struct option longopts[] ={{"output",no_argument,0,'o' },
							   {"help",no_argument,0,'h' },
							   {"caching",no_argument,0,'c' },
							   {"tabu",no_argument,0,'t' },
							   {"cb", required_argument,0, 'b'},
							   {"seed", required_argument,0, 's'},
							   {"cm", required_argument,0, 'm'},
							   {"Rounds", required_argument,0, 'r'},
							   {"Flips", required_argument,0, 'p'},
							   { "eps", required_argument, 0, 'e' },
							   {"function", required_argument,0, 'f'},
	                           //the opt array must terminate with a all zero element.
							   {0,0,0,0}
	                          };
	int result;
	int option_index = 0;
	while ((result = getopt_long(argc, argv, "ohcts:m:r:p:e:f:b:",longopts,&option_index)) != -1){
		switch (result) {
		case 'o': output_flag = true; break;
		case 'h':
			printUsage();
			exit(0);
			break;
		case 'c': caching_flag = true; break;
		case 's': {
			seed_flag = true;
			seed = atoi(optarg);
			break;
		}
		case 't': tabu_flag = true; break;
		case 'b': cb = atof(optarg); break;
		case 'm': cm = atof(optarg); break;
		case 'r': maxTries = strtoull(optarg,NULL,0); break;
		case 'p': maxFlips = strtoull(optarg,NULL,0); break;
		case 'e': eps = atof(optarg); break;
		case 'f': fkt = atoi(optarg); break;
		default:
			printUsage();
			exit(0);
		}
	}
	if(seed_flag)srand(seed);
	else srand (0);
	//else srand (time(NULL));
}
// construct the Problem with fill information of the input file
void readFile(const char* fileName){
	//cout<< "in readFile"<<endl;
	ifstream fp;
	//fp.open("Debug/instance.cnf",std::ios::in);
	fp.open(fileName,std::ios::in);
	if(!fp.is_open()){
		perror("read file fails");
		exit(EXIT_FAILURE);
	}
	string buff;
	char head;
   	getline(fp,buff);
   	// Get the p line
   	while(!fp.eof()){
		//cout<<buff<<endl;
		//todo:parseLine
   		if(buff.empty()) break;
		head =buff.at(0);
		if(head == 'p'){
			memAllocate(buff);
			break;
		}
	  getline(fp,buff);
	}
   	// Get the clauses
   	int index = -1;
   	int line = 0;
   	while(!fp.eof() && line < numCs){
   		index++;
		getline(fp,buff);
		if(buff.empty()) break;
		parseLine(buff, index);
		line++;
   	}
	//cout<< "out readFile"<<endl;
};
void memAllocate(string buff){
	parseLine(buff,-1);
	clauses = new C[numCs];
	variables = new V[numVs];
	numP = (int*) malloc(sizeof(int) * numCs);


}
void parseLine(string line,int indexC){
	char* str = strdup(line.c_str());
    const char s[2] = " ";
    if( indexC == -1){
    	strtok(str, s);
		strtok(NULL, s);
		numVs = atoi(strtok(NULL, s))+1;
		numCs = atoi(strtok(NULL, s));
		return;
    }// for the p line
    int numLits = -1;
    int lit;
    char* token = strtok(str, s);
    while(token != NULL){
    	++numLits;
		if(*token== '-'){
			lit = atoi(token);
		    clauses[indexC].lits.push_back(-lit);
		    variables[-lit].negC.push_back(indexC);
			token = strtok(NULL, s);
			continue;
		}
		if(*token == '0'){
		    clauses[indexC].numLits = numLits;
		    return;
		}
		lit = atoi(token);
	    clauses[indexC].lits.push_back(lit);
	    variables[lit].posC.push_back(indexC);
		token = strtok(NULL, s);
    }
	perror("a clause line does not terminates");
	exit(EXIT_FAILURE);
}
void printOptions(){
	printf("localSAT options: \n");
	cout<<"output_flag: "<<output_flag<<endl;
	cout<<"caching_flag: "<<caching_flag<<endl;
	cout<<"tabu_flag: "<<tabu_flag<<endl;
	cout<<"cb: "<<cb<<endl;
	cout<<"cm: "<<cm<<endl;
	cout<<"maxTries: "<<maxTries<<endl;
	cout<<"maxFlips: "<<maxFlips<<endl;
	cout<<"eps: "<<eps<<endl;
	cout<<"function: "<<fkt<<endl;
	cout<<"seed: "<<seed<<endl;
}
void printVariables(){
	cout<< "Variables "<< ": " <<endl ;
   	for(int i = 0; i < numVs; i++){
   		cout<< "Variable "<< i<< ": " ;
   		printVariable(variables[i]);
   	}
}
void printClauses(){
	cout<< "Clauses "<< ": " << endl ;
   	for(int i = 0; i < numCs; i++){
   		cout<< "Clause "<< i<< ": " ;
   		printClause(clauses[i]);
   	}
}
void printAssignment(){
	cout<< "Assignment: ";
	for(int i = 0; i < numVs; i++){
		cout <<variables[i].Assign<<" ";
	}
	cout <<endl ;
}

void printUnsatCs(){
	cout<< "Unsatisfied clauses ";
	printVector(unsatCs);
	cout <<endl ;
}
void initializeAssignment(){
   	for(int i = 0; i < numCs; i++){
   		numP[i] = 0;
   	}
   	for(int j = 0; j < numVs; j++){
   		variables[j].Assign = randomBoolean();
		if(variables[j].Assign == false){
	   		for (std::vector<int>::const_iterator i = variables[j].negC.begin(); i != variables[j].negC.end(); ++i){
	   			numP[*i]++;
	   		}
		}
		else{
			for (std::vector<int>::const_iterator i = variables[j].posC.begin(); i != variables[j].posC.end(); ++i){
	   			numP[*i]++;
			}
   		}
   	}
   	for(int i = 0; i < numCs; i++){
   		if(numP[i] == 0){
   			//cout<< "******************"<< i << "***************";
   			unsatCs.push_back(i);
   		}
   	}
}
int getFlipCandidate(C& clause){
	int size = clause.numLits;
	assert(size > 0);
	double f[size];
	double sum=0,rand;
	for(int i = 0; i < size;i++){
		sum+= func(i);
		f[i] = sum;
	}
	cout << endl;
	rand = randomDouble(sum);
	assert(rand >= 0);
	for(int i = 0; i < size;i++){
		if(f[i]> rand){
			return clause.lits[i];
		}
	}
/*	cout<< "the prefix array is ";
	for(int i = 0; i < size;i++){
		cout<< f[i]<<" ";
	}
	cout<< endl<<"the random is "<< rand <<endl;*/
	assert(false);
	return 0;
}
//todo:
double func(int index){
	return 1;
}
void flip(int j){
	std::vector<int>::const_iterator i;
	if(variables[j].Assign == false){
   		for (i = variables[j].negC.begin(); i != variables[j].negC.end(); ++i){
   			numP[*i]--;
   			if(numP[*i] == 0) unsatCs.push_back(*i);
   		}
		for (i = variables[j].posC.begin(); i != variables[j].posC.end(); ++i){
   			numP[*i]++;
		}

		variables[j].Assign = true;
	}
	else{
   		for (i = variables[j].negC.begin(); i != variables[j].negC.end(); ++i){
   			numP[*i]++;
   		}
		for (i = variables[j].posC.begin(); i != variables[j].posC.end(); ++i){
   			numP[*i]--;
   			if(numP[*i] == 0) unsatCs.push_back(*i);
		}
   			variables[j].Assign = false;
	}
}
void test(){
	int test[numCs]={0};
	std::vector<int>::const_iterator i;
	for(int j = 0; j < numVs; j++){
		if(variables[j].Assign == true){
			for (i = variables[j].posC.begin(); i != variables[j].posC.end(); ++i){
	   			test[*i]++;
			}
		}
		else{
			for (i = variables[j].negC.begin(); i != variables[j].negC.end(); ++i){
				test[*i]++;
			}
		}
	}
	for(int j = 0; j < numCs; j++){
	assert(test[j]>0);
	}
	cout<< "Satis"<< endl;
}
//todo:
int computeMakeScore(int index){
    int score = 0;
    vector<int>& occList = variables[index].Assign? variables[index].negC : variables[index].posC;
    for (int cid : occList) {
        if (numP[cid] == 0) {
            score++;
        }
    }
    return score;
}

int computeBreakScore(int index){
    int score = 0;
    vector<int>& occList =variables[index].Assign? variables[index].posC : variables[index].negC;
    for (int cid : occList) {
        if (numP[cid]== 1) {
            score++;
        }
    }
    return score;
}
