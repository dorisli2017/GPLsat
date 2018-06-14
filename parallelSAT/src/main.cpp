/*
Author      : Ping
Version     :
Copyright   : Your copyright notice
Description : Hello World in C++, Ansi-style
*/
#include "main.h"
using namespace std;


int main(int argc, char *argv[]) {

	//read global parameters (paramter files maybe)
	parseOptions(argc, argv);
	printOptions();
	//done
	//read problem file
	readFile(fileName);





	//initial assignment
	//build tabu stack
	//Best settings


	//parallel parts
	   // parameters different

	#pragma omp parallel num_threads(2)
	{
		cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	}
	return 0;
}


/*parse the argument (including options and filename)
 *using getopt_long to allow GNU-style long options as well as single-character options
 */
void parseOptions(int argc, char *argv[]){
	//opterr = 0;
	//The argument longopts must be an array of long option structures.
	struct option longopts[] ={{"output",no_argument,0,'o' },
							   {"help",no_argument,0,'h' },
							   {"Rounds", required_argument,0, 'r'},
							   {"Flips", required_argument,0, 'f'},
	                           //the opt array must terminate with a all zero element.
							   {0,0,0,0}
	                          };
	int result;
	int option_index = 0;
	while ((result = getopt_long(argc, argv, "ohr:f:",longopts,&option_index)) != -1){
		switch (result) {
		case 'o': output_flag = true; break;
		case 'h':
			printUsage();
			exit(0);
			break;
		case 'r': maxTries = strtoull(optarg,0,0); break;
		case 'f': maxFlips = strtoull(optarg,0,0); break;
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
}
/*print the manual of the program*/
void printUsage(){
	printf("\n--------parallelSAT, a parallel stochastic local search solving SAT problem-----------------\n");
	printf("\nautor: Guangping Li\n");
	printf("references: Engineering a lightweight and efficient local search sat solver \n");

	printf("\n-------------------Usage--------------------------------------------------------\n");
	printf("./locolSAT  <DIMACS CNF instance> [options]\n");

	printf("\n-------------------Options------------------------------------------------------\n");
	printf("**for the process:\n");
	printf("   --rounds, -r <long long int_value>,   : maximum number of tries \n");
	printf("   --flips, -f  <long long int_value>,   : maximum number of flips per try \n");
	printf("\n");
	printf("**further options:\n");
	printf("   --output, -o : output the final assignment\n");
	printf("   --help, -h : output this help\n");
	printf("---------------------------------------------------------------------------------\n");
}
void printOptions(){
	printf("localSAT options: \n");
	cout << fileName<< endl;
	cout<<"c output_flag: "<<output_flag<<endl;
	cout<<"c maxTries: "<<maxTries<<endl;
	cout<<"c maxFlips: "<<maxFlips<<endl;
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
};
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
	tabuS = (int*)malloc(sizeof(int) * numVs);
	for(int i = 0; i < numVs; i++){
		tabuS[i] = 0;
	}
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
    int lit;
    int size;
    char* token = strtok(str, s);
    while(token != NULL){
		if(*token== '-'){
			lit = atoi(token);
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
		clauseT.push_back(lit);
	    posOc[lit]++;
	    size++;
		token = strtok(NULL, s);
    }
	perror("a clause line does not terminates");
	exit(EXIT_FAILURE);
}

