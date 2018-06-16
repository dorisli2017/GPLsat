/*
Author      : Ping
Version     :
Copyright   : Your copyright notice
Description : Hello World in C++, Ansi-style
*/
#include "l.h"
using namespace std;


int main(int argc, char *argv[]) {
	parseOptions(argc, argv);
	std::ifstream fp;
	readFile(fileName);
	//debugProblem();
	#pragma omp parallel num_threads(2)
	{
		Process process = Process();
	}
	if(satG) cout<< "SAT" << endl;
}


/*parse the argument (including options (ouput and help) and filename)
 *using getopt_long to allow GNU-style long options as well as single-character options
 */
void parseOptions(int argc, char *argv[]){
	//opterr = 0;
	//The argument longopts must be an array of long option structures.
	struct option longopts[] ={{"help",no_argument,0,'h' },
							   {"seed", required_argument,0, 's'},
							   {"numberOfThreads", required_argument,0, 't'},
	                           //the opt array must terminate with a all zero element.
							   {0,0,0,0}
	                          };
	int result;
	int option_index = 0;
	while ((result = getopt_long(argc, argv, "hs:t:",longopts,&option_index)) != -1){
		switch (result) {
		case 'h':
			printUsage();
			exit(0);
			break;
		case 's':
			seed_flag = true;
			seed = atoi(optarg);
			break;
		case 't':
			numberOfThreads =  atoi(optarg);
			break;
		default:
			printUsage();
			exit(0);
		}
	}
	if(seed_flag)srand(seed);
	else srand (time(NULL));
	assert(optind < argc &&"ERROR: FILE is not given, -h for help\n");

	fileName = *(argv + optind);
}
/*print the manual of the program*/
void printUsage(){
	printf("\n--------parallelSAT, a parallel stochastic local search solving SAT problem-----------------\n");
	printf("\nautor: Guangping Li\n");
	printf("references: Engineering a lightweight and efficient local search sat solver \n");

	printf("\n-------------------Usage--------------------------------------------------------\n");
	printf("./locolSAT  <DIMACS CNF instance> [options]\n");

	printf("\n-------------------Options------------------------------------------------------\n");
	printf("   --output, -o : output the final assignment\n");
	printf("   --help, -h : output this help\n");
	printf("---------------------------------------------------------------------------------\n");
}
/*check the options*/
void printOptions(){
	printf("localSAT options: \n");
	cout << fileName<< endl;
	cout<<"seed: "<<seed<<endl;
}

/* construct the Problem
 * only read clauses as the number of clauses.
 * */
void readFile(const char* fileName){
	ifstream fp;
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
   	assert(line ==numCs &&"wrong clause number");
   	vector<int>().swap(clauseT);
// allocate the vectors in the second round
	for(int i = 0; i < numVs; i++){
			if(posOc[i]> maxOcc) maxOcc = posOc[i];
			if(negOc[i]> maxOcc) maxOcc = negOc[i];
			posC[i].reserve(posOc[i]);
			negC[i].reserve(negOc[i]);
		}
	for(int j = 0; j < numCs; j++){
		for (std::vector<int>::const_iterator i = clauses[j].begin(); i != clauses[j].end(); ++i){
			if(*i < 0) negC[-(*i)].push_back(j);
			else  posC[(*i)].push_back(j);
		}
	}
};
/* allocate the memory accoring to the numVs and numCs*/
void memAllocate(string buff){
	parseLine(buff,-1);
	clauses = new vector<int>[numCs];
	posC= new vector<int>[numVs];
	negC= new vector<int>[numVs];
	posOc = (int*) malloc(sizeof(int) * numVs);
	for(int i = 0; i < numVs; i++){
		posOc[i] = 0;
	}
	negOc = (int*) malloc(sizeof(int) * numVs);
	for(int i = 0; i < numVs; i++){
		negOc[i] = 0;
	}
	clauseT.reserve(numVs);
	tabuG = (int*)malloc(sizeof(int) * numVs);
	for(int i = 0; i < numVs; i++){
		tabuG[i] = 0;
	}
}
/* parse the lines in file.
 * regnize p line using indexC as -1
 * clause as a vector of int values. positiv literal as positive int. Negative literal as negativ int
 * no including variable 0 in clause
 * */
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
    int lit;
    int size;
    char* token = strtok(str, s);
    while(token != NULL){
		if(*token== '-'){
			lit = atoi(token);
			assert(-lit <= numVs && "wrong number of variables ");
			clauseT.push_back(lit);
		    negOc[-lit]++;
			token = strtok(NULL, s);
			size++;
			continue;
		}
		if(*token == '0'){
			clauses[indexC] = clauseT;
			clauseT.clear();
		    return;
		}
		lit = atoi(token);
		assert(lit <= numVs && "wrong number of variables ");
		clauseT.push_back(lit);
	    posOc[lit]++;
	    size++;
		token = strtok(NULL, s);
    }
    assert(false && "a clause line does not terminates");
}
/*check the global options and the problem  */
void debugProblem(){
	printOptions();
	printVariables();
	printClauses();
}
/*print the variables <number of occurences><in which clauses>*/
void printVariables(){
	cout<< "Variables "<< ": " <<endl ;
   	for(int i = 1; i < numVs; i++){
   		cout<< "Variable "<<i << ": " <<endl;
   		cout<< posOc[i]<< " ";
   		printVector(posC[i]);
   		cout<< negOc[i]<< " ";
   		printVector(negC[i]);
   	}
}
/*print the clauses using negative symbol for negative literals*/
void printClauses(){
	cout<< "Clauses "<< ": " << endl ;
   	for(int i = 0; i < numCs; i++){
   		cout<< "Clause "<< i<< ": " ;
   		printVector(clauses[i]);
   	}
}
/*inline function for print a vector*/
template <class T> void printVector(vector<T>& vec){
	for (typename vector<T>::const_iterator i = vec.begin(); i != vec.end(); ++i){
		cout << *i << ' ';
	}
	cout << endl;
}

Process::Process(){
	vector<double>&  pd = paraD[0];
	vector<int>&  pi = paraI[1];
	cb = pd[0];
    ct = pd[1];
    eps = pd[2];
    fct = pi[0];
    ict = pi[1];
    flips = pi[2];
    steps = pi[3];
    tabu_flag =pi[4];
#pragma omp critical
	{
   // printParameters();
	}
	// init lookUptable
	lookUpTable = (double*)malloc(sizeof(double) * maxOcc);
	switch (fct){
	case 0:initLookUpTable_poly();break;
	default:initLookUpTable_exp();break;
	}
	// init assign
	assign = (bool*)malloc(sizeof(bool) * numVs);
	numP = vector<int> (numVs,0);
	unsatN = 0;
	unsatCs = (int*)malloc(sizeof(int) * numCs);
	switch(ict){
	case 0: randomBiasAssignment();break;
	case 1:	biasAssignment(); break;
	default: randomAssignment();break;
	}
	probs = (double*)malloc(sizeof(double) * numVs);
	if(tabu_flag){
		tabuS = (int*)malloc(sizeof(int) * numVs);
		for(int i = 0; i < numVs; i++){
			tabuS[i] = 0;
		}
	}
	else tabuS = tabuG;
	optimal();
}
void Process::initLookUpTable_exp(){
	for(int i = 0; i < maxOcc;i++){
		lookUpTable[i] = pow(cb,-i);
	}
}
void Process::optimal(){
	while(true){
		for(int i = 0;i < flips; i++){
			for(int j = 0;j < steps; j++){
				if (unsatN == 0 ){
				#pragma omp critical
					{
					printAssignment();
					test();
					}
					satG = true;
					return;
				}
				search_prob();
			}
			if(satG) return;
		}
		switch(rand()%3){
		case 0: randomBiasAssignment();break;
		case 1:	biasAssignment(); break;
		default: randomAssignment();break;
		}
	}
	//test();
	debugAssign();
}

void Process::initLookUpTable_poly(){
	for(int i = 0; i < maxOcc;i++){
		lookUpTable[i] = pow((eps+i),-cb);
	}
}
// based on the occurences of literals.
void Process::randomBiasAssignment(){
	for(int i = 0; i < numVs; i++){
		    int sum = posOc[i] +negOc[i];
		    if (sum == 0){
		    	assign[i] = (rand()%2 ==1);
		    	continue;
		    }
			assign[i] = (rand()%(posOc[i] +negOc[i]))< posOc[i];
	}
	setAssignment();
}
// based on the occurences of literals.
void Process::biasAssignment(){
	for(int i = 0; i < numVs; i++){
		    int sum = posOc[i] +negOc[i];
		    if (sum == 0){
		    	assign[i] = (rand()%2 ==1);
		    	continue;
		    }
		    assign[i] = posOc[i] >negOc[i];
	}
	setAssignment();
}
void Process::randomAssignment(){
	for(int j = 0; j < numVs; j++){
		assign[j] = (rand()%2 ==1);
	}
    setAssignment();
}

void Process::setAssignment(){
	fill(numP.begin(), numP.end(), 0);
	unsatN = 0;
   	for(int j = 0; j < numVs; j++){
		if(assign[j] == false){
	   		for (std::vector<int>::const_iterator i = negC[j].begin(); i != negC[j].end(); ++i){
	   			numP[*i]++;
	   		}
		}
		else{
			for (std::vector<int>::const_iterator i = posC[j].begin(); i != posC[j].end(); ++i){
	   			numP[*i]++;
			}
   		}
   	}
   	for(int i = 0; i < numCs; i++){
   		if(numP[i] == 0){
   			unsatCs[unsatN]=i;
   			unsatN++;
   		}
   	}
}

void Process::debugAssign(){
	   	printAssignment();
	   	printUnsatCs();
	   	printNumP();
}

void Process::printAssignment(){
	cout<< "v ";
	for(int i = 1; i < numVs; i++){
		if(assign[i]) cout <<i<<" ";
		else cout << -i<<" ";
	}
	cout <<endl ;
}
void Process::printUnsatCs(){
	cout<< "Unsatisfied clauses ";
	for(int i  = 0; i < unsatN; i++){
		cout<< unsatCs[i]<< ' ';
	}
	cout <<endl ;
}
void Process::printNumP(){
	cout<< "numP: ";
	for(int i = 0; i < numCs; i++){
		cout << numP[i]<< " ";
	}
	cout<<endl;
}

void Process::test(){
	ifstream fp;
	fp.open(fileName,std::ios::in);
	if(!fp.is_open()){
		perror("read file fails");
		exit(EXIT_FAILURE);
	}
	string buff;
	char head;
   	getline(fp,buff);
   	while(!fp.eof()){
   		if(buff.empty()) break;
		head =buff.at(0);
		if(head == 'p'){
			break;
		}
	  getline(fp,buff);
	}
   	while(!fp.eof()){
		getline(fp,buff);
		if(buff.empty()) break;
		testLine(buff);
   	}
   	cout<< "tested" << endl;
}
void Process::testLine(string line){
	char* str = strdup(line.c_str());
    const char s[2] = " ";
    int lit;
    int numT=0;
    char* token = strtok(str, s);
    while(token != NULL){
		if(*token== '-'){
			lit = atoi(token);
			if(assign[-lit] == false) numT++;
			token = strtok(NULL, s);
			continue;
		}
		if(*token == '0'){
			if(numT == 0){
				perror("TEST FAILURE");
				exit(EXIT_FAILURE);
			}
		    return;
		}
		lit = atoi(token);
		if(assign[lit] == true) numT++;
		token = strtok(NULL, s);
    }
	perror("a clause line does not terminates");
	exit(EXIT_FAILURE);
}

void Process::search_prob(){
	int randC = rand()%unsatN;
	int flipCindex = unsatCs[randC];
	if(numP[flipCindex] > 0){
		unsatN--;
		unsatCs[randC]=unsatCs[unsatN];
		return;
	}
	int flipVindex = getFlipCandidate(flipCindex);
	unsatN--;
	unsatCs[randC]=unsatCs[unsatN];
	flip(flipVindex);
}

int Process::getFlipCandidate(int cIndex){
	vector<int>&  vList = clauses[cIndex];
	int j=0,bre;
	double sum=0,randD;
	for (std::vector<int>::const_iterator i = vList.begin(); i != vList.end(); ++i){
		bre = computeBreakScore(*i);
		if(ct){
			if(bre == 0 && rand() > tabuS[*i]) return *i;
			bre += ct*tabuS[*i];
		}
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
void Process::flip(int j){
	std::vector<int>::const_iterator i;
	if(j > 0){
   		for (i = negC[j].begin(); i != negC[j].end(); ++i){
   			numP[*i]--;
   			if(numP[*i] == 0){
   			 unsatCs[unsatN]=*i;
   			 unsatN++;

   			}
   		}
		for (i = posC[j].begin(); i != posC[j].end(); ++i){
   			numP[*i]++;
		}

		assign[j] = true;
		if(ct){
			tabuS[j]++;
		}
	}
	else{
   		for (i = negC[-j].begin(); i != negC[-j].end(); ++i){
   			numP[*i]++;
   		}
		for (i = posC[-j].begin(); i != posC[-j].end(); ++i){
   			numP[*i]--;
   			if(numP[*i] == 0){
   				unsatCs[unsatN]=*i;
   				unsatN++;
   			}
		}
		assign[-j]= false;
		if(ct){
			//cout<< "tabu works"<< endl;
			tabuS[-j]++;
		}
	}
}

int Process::computeBreakScore(int index){
    int score = 0;
    int aIndex = abs(index);
    vector<int>& occList =(index < 0)? posC[aIndex] :negC[aIndex];
    for(std::vector<int>::const_iterator i = occList.begin(); i != occList.end(); ++i) {
        if (numP[*i]== 1) {
            score++;
        }
    }
    return score;
}

void Process::printParameters(){
	//cout<< omp_get_thread_num()<< endl;
	cout<<"c cb: "<<cb<<endl;
	cout<<"c maxFlips: "<<flips<<endl;
	cout<<"c maxStepss: "<<steps<<endl;
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
		cout<<"c exponential function"<<endl;
		cout<<"c cb: "<<cb<<endl;
		cout<< "pow(cb,-break)"<< endl;
		break;
		   }
	}
	cout<<"c ict: "<<ict<<endl;
}
