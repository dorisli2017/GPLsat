/*
 * miscellaneous.h
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */
#include <vector>
#include<iostream>
#include <stdio.h>
using namespace std;

/*defined structure*/
struct C{
	int numLits;
	vector<int> lits;
	int numP;
};
 struct V{
	vector<int> posC;
	vector<int> negC;
 };

void printVector(vector<int>& vec);
void printUsage();
void printClause(C& clause);
void printVariable(V& variable);
