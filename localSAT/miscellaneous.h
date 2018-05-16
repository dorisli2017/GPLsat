/*
 * miscellaneous.h
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */
#include <vector>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

/*defined structure*/
struct C{
	int numLits;
	vector<int> lits;
};
 struct V{
	vector<int> posC;
	vector<int> negC;
	bool Assign;
 };

void printVector(vector<int>& vec);
void printUsage();
void printClause(C& clause);
void printVariable(V& variable);
double randomDouble(double fMax);

