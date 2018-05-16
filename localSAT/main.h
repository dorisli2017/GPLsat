/*
 * main.h
 *
 *  Created on: May 8, 2018
 *      Author: Guangping Li
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
#include <math.h>


/*problem and assignment*/
char* fileName;
int numCs;
int numVs;
struct C *clauses;
struct V *variables;
int* numP;
vector<int> unsatCs;


/*settings*/
bool output_flag;
bool tabu_flag;
bool seed_flag;
/*option values*/

unsigned long long int maxTries = ULLONG_MAX;
unsigned long long int maxFlips = ULLONG_MAX;
int seed;
double cb=3.6;
double cm=0.5;
double w = 0.3;// The probability used in the WALKSAT algorithm
float eps= 1.0;
int fct= 0;
int alg = 0;


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
void printNumP();
int getFlipCandidate(int cIndex);
double (*func)(int) = NULL;
double func_equal(int index);
double func_exp(int index);
double func_poly(int index);
void flip(int j);
void debugProblem();
void debugAssign();
void test();
int computeMakeScore(int index);
int computeBreakScore(int index);
void search_prob();
void (*search)(void)=NULL;
void search_lawa();
void search_wa();
int getFlipCandidate_wa(int cIndex);
int getFlipCandidate_max(int cIndex);
#endif /* MAIN_H_ */
