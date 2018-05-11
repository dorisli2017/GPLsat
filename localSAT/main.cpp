/*
 * main.cpp
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */

#include "main.h"

int main(int argc, char *argv[])
{
 /*todo:
  * parse the argument (including options and filename)
  * parse the file (clauses and variables)
  *
  */
	/* Finished**********************************/

	parseOptions(argc, argv);
	//printOptions();
	const char* fileName = argv[1];
	//cout<< "start readFile"<<endl;
	readFile(fileName);
	//cout<< "finish readFile"<<endl;
   	printVariables();
   	printClauses();
   	printAssignment();
	/*Finished**********************************/



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
	if(seed_flag)srand (seed);
	else srand (time(NULL));
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
	initializeSearch();
   	// Get the clauses
   	int index = -1;
   	while(!fp.eof()){
   		index++;
		getline(fp,buff);
		if(buff.empty()) break;
		parseLine(buff, index);
   	}
	//cout<< "out readFile"<<endl;
};
void memAllocate(string buff){
	parseLine(buff,-1);
	clauses = new C[numCs];
	variables = new V[numVs];


}
void parseLine(string line,int indexC){
	//cout<< "in parseLine"<<endl;
	char* str = strdup(line.c_str());
    const char s[2] = " ";
    if( indexC == -1){
    	strtok(str, s);
		strtok(NULL, s);
		numVs = atoi(strtok(NULL, s))+1;
		numCs = atoi(strtok(NULL, s));
		//cout<< "out parseLine"<<endl;
		return;
    }// for the p line
    int numLits = -1;
    int lit;
    char* token = strtok(str, s);
    while(token != NULL){
    	++numLits;
		if(*token== '-'){
			lit = atoi(token);
		    clauses[indexC].lits.push_back(lit);
		    variables[-lit].negC.push_back(indexC);
   			if(variables[-lit].Assign == false)clauses[indexC].numP++;
			token = strtok(NULL, s);
			continue;
		}
		if(*token == '0'){
		    clauses[indexC].numLits = numLits;
		    //printClause(clauses[indexC]);
			//cout<< "out parseLine"<<endl;
		    return;
		}
		lit = atoi(token);
	    clauses[indexC].lits.push_back(lit);
	    variables[lit].posC.push_back(indexC);
		if(variables[lit].Assign == true)clauses[indexC].numP++;
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
	cout<< "Assignment ";
	for(int i = 0; i < numVs; i++){
		cout<< i<< ": "<<variables[i].Assign<<"|";
	}
	cout <<endl ;
}

void initializeSearch(){
   	for(int i = 0; i < numVs; i++){
   		variables[i].Assign = randomBoolean();
   	}
}
