/*
 * miscellaneous.cpp
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */
#include "miscellaneous.h"
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
	printf("   --function, -f: 0 =  polynomial; 1 = exponential [default = 0]\n");
	printf("   --eps, -e <float value> [0.0,1.0] :  [default = 1.0]\n");
	printf("   --cb, -b <float value>  [2.0,10.0] : constant for break\n");
	printf("   --cm, -m <float value>  [-2.0,2.0] : constant for make\n");
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

void printClause(C& clause){
cout<< "numer of variables: " << clause.numLits << endl;
printVector(clause.lits);
cout<< "value: " << clause.numP<< endl;
cout<<endl;
}

void printVariable(V& variable){
cout<< variable.Assign << endl;
cout<< "positiv clauses: " << endl;
printVector(variable.posC);
cout<< "negative clauses: " << endl;
printVector(variable.negC);
cout<<endl;
}

bool randomBoolean(){
	return rand()%2 ==1;
}

int randomIndex(int range){
	return rand()% range;
}
