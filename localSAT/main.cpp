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
	initLookUpTable();
	//debugProblem();
	int size;
	for(unsigned int i = 0; i <maxTries;i++){
		initializeAssignment();
		for(unsigned int j = 0; j < maxFlips; j++){
			size =  unsatCs.size();
			if (size == 0){
				//debugAssign();
				//test();
				cout<< "s SATISFIABLE"<< endl;
				//printAssignment();
				return 0;
			}
			search_prob();
		}
	}
	//test();
	debugAssign();
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
	                           //the opt array must terminate with a all zero element.
							   {0,0,0,0}
	                          };
	int result;
	int option_index = 0;
	while ((result = getopt_long(argc, argv, "ohts:m:w:r:p:e:f:b:",longopts,&option_index)) != -1){
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
	case 0:initLookUpTable = initLookUpTable_poly;break;
	default:initLookUpTable = initLookUpTable_exp;break;
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
	clauses = new vector<int>[numCs];
	variables = new V[numVs];
	numP = (int*) malloc(sizeof(int) * numCs);
	probs = (double*)malloc(sizeof(double) * numVs);
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
		if(*token== '-'){
			lit = atoi(token);
		    clauses[indexC].push_back(-lit);
		    variables[-lit].negC.push_back(indexC);
			token = strtok(NULL, s);
			continue;
		}
		if(*token == '0'){
		    return;
		}
		lit = atoi(token);
	    clauses[indexC].push_back(lit);
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
   		printVector(clauses[i]);
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
	vector<int>&  vList = clauses[cIndex];
	int j=0,bre;
	double sum=0,randD;
	for (std::vector<int>::const_iterator i = vList.begin(); i != vList.end(); ++i){
		bre = computeBreakScore(*i);
		if(bre == 0) return *i;
		sum+= lookUpTable[bre];
		probs[j]= sum;
		j++;
	}
	randD = ((double)rand()/RAND_MAX)*sum;
	assert(randD >= 0);
	for(int i = 0; i < j;i++){
		if(probs[i]> randD){
			return vList[i];
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
	int test[numCs];
	memset(test, 0, sizeof(test));
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


void printVector(vector<int>& vec){
	for (std::vector<int>::const_iterator i = vec.begin(); i != vec.end(); ++i){
		cout << *i << ' ';
	}
	cout << endl;
}

/*print the manual of the program*/
void printUsage(){
	printf("\n");
	printf("\n");
	printf("\n");
	printf("\n--------localSAT, a stochastic local search solving SAT problem-----------------\n");
	printf("\nautor: Guangping Li\n");
	printf("references: Engineering a lightweight and efficient local search sat solver \n");

	printf("\n-------------------Usage--------------------------------------------------------\n");
	printf("\n");
	printf("./locolSAT  <DIMACS CNF instance> [options]\n");
	printf("\n--------------------------------------------------------------------------------\n");

	printf("\n");
	printf("\n-------------------Options------------------------------------------------------\n");
	printf("\n");
	printf("**for the potential flipping function:\n");
	printf("   --function, -f: 0 =  polynomial; 1 = exponential with make; 2 = exponential only with break: 3 = equal  [default = 0]\n");
	printf("   --eps, -e <double value> [0.0,1.0] :  [default = 1.0]\n");
	printf("   --cb, -b <double value>  [2.0,10.0] : constant for break\n");
	printf("   --cm, -m <double value>  [-2.0,2.0] : constant for make\n");
	printf("   --w, -w <double value>  [0.0,1.0] : [default = 0.3] the probability used only in the WALKSAT\n");
	printf("\n");
	printf("**for the process:\n");
	printf("   --caching, -c  : 0 =  no caching ; 1 = caching  for scores\n");
	printf("   --rounds, -r <long long int_value>,   : maximum number of tries \n");
	printf("   --flips, -p  <long long int_value>,   : maximum number of flips per try \n");
	printf("   --seed, -s : seed used for Randomness\n");
	printf("\n");
	printf("**further options:\n");
	printf("   --output, -o : output the final assignment\n");
	printf("   --help, -h : output this help\n");
	printf("\n");
	printf("---------------------------------------------------------------------------------\n");
}


void printVariable(V& variable){
cout<< variable.Assign << endl;
cout<< "positiv clauses: " << endl;
printVector(variable.posC);
cout<< "negative clauses: " << endl;
printVector(variable.negC);
cout<<endl;
}

void initLookUpTable_exp(){
	lookUpTable = (double*)malloc(sizeof(double) * numCs);
	for(int i = 0; i < numCs;i++){
		lookUpTable[i] = pow(cb,-i);
	}
}

void initLookUpTable_poly(){
	lookUpTable = (double*)malloc(sizeof(double) * numCs);
	for(int i = 0; i < numCs;i++){
		lookUpTable[i] = pow((eps+i),-cb);
	}
}

