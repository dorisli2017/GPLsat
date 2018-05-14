/*
 * main.h
 *
 *  Created on: May 8, 2018
 *      Author: ping
 */

#ifndef MAIN_H_
#define MAIN_H_

/* includes*/

#include <iostream>
#include <climits>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <string.h>
#include <vector>
#include <assert.h>
#include "miscellaneous.h"


/*problem and assignment*/
int numCs;
int numVs;
struct C *clauses;
struct V *variables;
int* numP;
vector<int> unsatCs;

/*settings*/
bool output_flag;
bool caching_flag;
bool tabu_flag;
bool seed_flag;
/*option values*/
unsigned long long int maxTries = ULLONG_MAX;
unsigned long long int maxFlips = ULLONG_MAX;
int seed;
float cb;
float cm;
float eps;
int fkt;

/*methods*/
void parseOptions(int argc, char *argv[]);
void printOptions();
void readFile(const char* fileName);
void memAllocate(string buff);
void parseLine(string line, int index);
void initializeAssignment();
void printVariables();
void printClauses();
void printAssignment();
void printUnsatCs();
int getFlipCandidate(C& clause);
double func(int index);
void flip(int j);
void debug();
void test();
int computeMakeScore(int index);
int computeBreakScore(int index);


#endif /* MAIN_H_ */
