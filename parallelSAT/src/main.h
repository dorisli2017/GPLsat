/*
 * main.h
 *
 *  Created on: Jun 12, 2018
 *      Author: ping
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <vector>
#include <climits>
#include <getopt.h>
#include <climits>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <omp.h>
#include <fstream>
using namespace std;

/*problem */
int numCs;
int numVs;
vector<int>* posC;
vector<int>* negC;
int* posOc;
int* negOc;
int maxOcc;
double* lookUpTable;
vector<int>* clauses;
vector<int> clauseT;
//tabu
int* tabuS;


/*settings*/
bool output_flag;
char* fileName;
unsigned long long int maxTries = ULLONG_MAX;
unsigned long long int maxFlips = ULLONG_MAX;
/*methods*/

// get parameters
void parseOptions(int argc, char *argv[]);
void printUsage();
void printOptions();

//read the problem file
void readFile(const char* fileName);
void memAllocate(string buff);
void parseLine(string line,int indexC);







#endif /* MAIN_H_ */
