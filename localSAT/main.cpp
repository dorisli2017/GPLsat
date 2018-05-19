/*
 * main.cpp
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */

#include "main.h"

int main(int argc, char *argv[]){
	parseOptions(argc, argv);
	readFile(fileName);
	int size;
	for(unsigned int i = 0; i <maxTries;i++){
		initializeAssignment();
		for(unsigned int j = 0; j < maxFlips; j++){
			size =  unsatCs.size();
			if (size == 0){
				/*debugAssign();
				test();*/
				cout<< "s SATISFIABLE"<< endl;
				printAssignment();
				return 0;
			}
			search();
		}
	}
	//test();
	debugAssign();
	//cout<<"out main" <<endl;
    return 0;
}
void debugProblem(){
	printOptions();
	printVariables();
	printClauses();
}
void debugAssign(){
	/* Testing code**********************************/
	   	printAssignment();
	   	printUnsatCs();
	   	printNumP();


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
							   {"tabu",no_argument,0,'t' },
							   {"cb", required_argument,0, 'b'},
							   {"seed", required_argument,0, 's'},
							   {"cm", required_argument,0, 'm'},
							   {"w", required_argument,0, 'w'},
							   {"Rounds", required_argument,0, 'r'},
							   {"Flips", required_argument,0, 'p'},
							   { "eps", required_argument, 0, 'e' },
							   {"function", required_argument,0, 'f'},
							   {"algorithm", required_argument,0, 'a'},
	                           //the opt array must terminate with a all zero element.
							   {0,0,0,0}
	                          };
	int result;
	int option_index = 0;
	while ((result = getopt_long(argc, argv, "ohts:m:w:r:p:e:f:a:b:",longopts,&option_index)) != -1){
		switch (result) {
		case 'o': output_flag = true; break;
		case 'h':
			printUsage();
			exit(0);
			break;
		case 's': {
			seed_flag = true;
			seed = atoi(optarg);
			break;
		}
		case 't': tabu_flag = true; break;
		case 'b': cb = atof(optarg); break;
		case 'm': cm = atof(optarg); break;
		case 'w': w = atof(optarg); break;
		case 'r': maxTries = strtoull(optarg,NULL,0); break;
		case 'p': maxFlips = strtoull(optarg,NULL,0); break;
		case 'e': eps = atof(optarg); break;
		case 'f': fct = atoi(optarg); break;
		case 'a': alg = atoi(optarg); break;
		default:
			printUsage();
			exit(0);
		}
	}
	if (optind == argc) {
		printf("ERROR: FILE is not given\n");
		printUsage();
		exit(0);
	}
	fileName = *(argv + optind);

//set the parameters
	// set seed
	if(seed_flag)srand(seed);
	else srand (0);
	//else srand (time(NULL));
	switch (fct){
	case 0:func = func_poly;break;
	case 1:func = func_exp_make;break;
	case 2:func = func_exp;break;
	default:func = func_equal;
	}
	switch (alg){
	case 0:search = search_prob;break;
	case 1:search = search_lawa;break;
	default:search = search_wa;
	}

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
		    clauses[indexC].vars.push_back(-lit);
		    variables[-lit].negC.push_back(indexC);
			token = strtok(NULL, s);
			continue;
		}
		if(*token == '0'){
		    clauses[indexC].numLits = numLits;
		    return;
		}
		lit = atoi(token);
	    clauses[indexC].vars.push_back(lit);
	    variables[lit].posC.push_back(indexC);
		token = strtok(NULL, s);
    }
	perror("a clause line does not terminates");
	exit(EXIT_FAILURE);
}
void printOptions(){
	printf("localSAT options: \n");
	cout<<"c output_flag: "<<output_flag<<endl;
	cout<<"c tabu_flag: "<<tabu_flag<<endl;
	cout<<"c cb: "<<cb<<endl;
	cout<<"c cm: "<<cm<<endl;
	cout<<"c w: "<<w<<endl;
	cout<<"c maxTries: "<<maxTries<<endl;
	cout<<"c maxFlips: "<<maxFlips<<endl;
	cout<<"c eps: "<<eps<<endl;
	cout<<"c algorithm: "<<alg<<endl;
	switch(fct){
	case 0:{
		cout<<"c polynomial function"<<endl;
		cout<<"c eps: "<<eps<<endl;
		cout<<"c cb: "<<cb<<endl;
		cout<< "pow((eps+break),-cb)" << endl;
		break;
		  }
	case 1:{
		cout<<"c exponential function with make"<<endl;
		cout<<"c cm: "<<cm<<endl;
		cout<<"c cb: "<<cb<<endl;
		cout<< "pow(cm,make)*pow(cb,-break)"<< endl;
		break;
		   }
	case 2:{
		cout<<"c equal function (== 1.0)"<<endl;
	       }
	}
	cout<<"c seed: "<<seed<<endl;
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
	cout<< "v ";
	for(int i = 1; i < numVs; i++){
		if(variables[i].Assign) cout <<i<<" ";
		else cout << -i<<" ";
	}
	cout <<endl ;
}
void printUnsatCs(){
	cout<< "Unsatisfied clauses ";
	printVector(unsatCs);
	cout <<endl ;
}
void printNumP(){
	cout<< "numP: ";
	for(int i = 0; i < numCs; i++){
		cout << numP[i]<< " ";
	}
	cout<<endl;
}
void initializeAssignment(){
   	for(int i = 0; i < numCs; i++){
   		numP[i] = 0;
   	}
   	for(int j = 0; j < numVs; j++){
   		variables[j].Assign = (rand()%2 ==1);
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
int getFlipCandidate(int cIndex){
	C& clause = clauses[cIndex];
    vector<int>& vList = clause.vars;
	int size = clause.numLits,j=0;
	assert(size > 0);
	double f[size];
	double sum=0,randD;
	for (std::vector<int>::const_iterator i = vList.begin(); i != vList.end(); ++i){
		sum+= func(*i);
		f[j]= sum;
		j++;
	}
	randD = ((double)rand()/RAND_MAX)*sum;
	assert(randD >= 0);
	for(int i = 0; i < size;i++){
		if(f[i]> randD){
			return clause.vars[i];
		}
	}
	assert(false);
	return 0;
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
	cout<< "s SATISFIABLE"<< endl;
}
int computeMakeScore(int index){
    int score = 0;
    vector<int>& occList = variables[index].Assign? variables[index].negC : variables[index].posC;
	//cout<< "in make "<<endl;
	for (std::vector<int>::const_iterator i = occList.begin(); i != occList.end(); ++i){
        if (numP[*i] == 0) {
            score++;
        }
    }
	//cout<< "out make "<<endl;
    return score;
}
int computeBreakScore(int index){
	//cout<< "in break "<<endl;
    int score = 0;
    vector<int>& occList =variables[index].Assign? variables[index].posC : variables[index].negC;
    for(std::vector<int>::const_iterator i = occList.begin(); i != occList.end(); ++i) {
        if (numP[*i]== 1) {
            score++;
        }
    }
	//cout<< "out make "<<endl;
    return score;
}
double func_equal(int index){
	return 1.0;
}
double func_exp_make(int index){
	//cout<< "in func_exp "<<endl;
	return pow(cm,computeMakeScore(index))*pow(cb,-computeBreakScore(index));
	//cout<< "out func_exp "<<endl;
};

double func_exp(int index){
	return pow(cb,-computeBreakScore(index));
}
double func_poly(int index){
	return pow((eps+computeBreakScore(index)),-cb);
}

void search_prob(){
	int randC = rand()%unsatCs.size();
	int flipCindex = unsatCs[randC];
	if(numP[flipCindex] > 0){
		unsatCs[randC]=unsatCs.back();
		unsatCs.pop_back();
		return;
	}
	int flipVindex = getFlipCandidate(flipCindex);
	unsatCs[randC]=unsatCs.back();
	unsatCs.pop_back();
	flip(flipVindex);
}

void search_lawa(){
	int randC = rand()%unsatCs.size();
	int flipCindex = unsatCs[randC];
	if(numP[flipCindex] > 0){
		unsatCs[randC]=unsatCs.back();
		unsatCs.pop_back();
		return;
	}
	if(clauses[flipCindex].numLits == 1){
		unsatCs[randC]=unsatCs.back();
		unsatCs.pop_back();
		flip(0);
		return;
	}
	int vRange = clauses[flipCindex].numLits;
    int id1 = rand() % vRange;
    int id2 = rand() % vRange;
    while(id1 == id2) {
        id1 = rand() % vRange;
        id2 = rand() % vRange;
    }
    int lit1 = clauses[flipCindex].vars[id1];
    int lit2 = clauses[flipCindex].vars[id2];
    int score1 = computeMakeScore(lit1) - computeBreakScore(lit1);
    int score2 = computeMakeScore(lit2) - computeBreakScore(lit2);
    if (score1 == score2) {
        flip(lit1);
        flip(lit2);
    } else {
        int toFlip = score1 > score2 ? lit1 : lit2;
        flip(toFlip);
    }
}

void search_wa(){
	int randC = 	rand()% unsatCs.size();
	int flipCindex = unsatCs[randC];
	if(numP[flipCindex] > 0){
		unsatCs[randC]=unsatCs.back();
		unsatCs.pop_back();
		return;
	}
	int flipVindex = getFlipCandidate_wa(flipCindex);
	unsatCs[randC]=unsatCs.back();
	unsatCs.pop_back();
	flip(flipVindex);

}

// todo: improvement still posssible.Break value calculated twice.
int getFlipCandidate_wa(int cIndex){
	C& clause = clauses[cIndex];
    vector<int>& vList = clause.vars;
	for (std::vector<int>::const_iterator i = vList.begin(); i != vList.end(); ++i){
		if(computeBreakScore(*i)==0){
			return *i;
		}
	}
	if(((double)rand()/RAND_MAX) < w) 	return rand()% clause.numLits;
	int maxIndex;
	double max = -1, candidate = -1;
	for (std::vector<int>::const_iterator i = vList.begin(); i != vList.end(); ++i){
		candidate = func(*i);
		if(candidate> max){
		max = candidate;
		maxIndex = *i;
		}
	}
	return maxIndex;
}



