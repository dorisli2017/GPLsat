/*
 * main.h
 *
 *  Created on: Jun 12, 2018
 *      Author: ping
 */

#ifndef PL_H_
#define PL_H_
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
#include <math.h>
#include <assert.h>
#include <sstream>
using namespace std;

/*problem */
char* fileName;
bool satG= false;
bool* assignG;
int numCs;// number of clauses
int numVs;//number of variables
vector<int>* posC;// the clauses for each positive literal
vector<int>* negC;//the clauses for each negative literal
int* posOc;// number of positive occurrences of variables
int* negOc;//number of negative occurrences of variables
int maxOcc;// maximal occurrences of all literals(positive or negative), for estimate the maximal break number for establish the lookUptable in process
vector<int>* clauses;
vector<int> clauseT;// for make up the use of vector.
//the global tabu things
int* tabuG;

/*settings*/
bool output_flag;
bool seed_flag;
int seed;
char* paramteterFile;
int numberOfThreads = 2;

/*methods*/

// get parameters
void parseOptions(int argc, char *argv[]);
void printUsage();

//read the problem file
void readFile(const char* fileName);
void memAllocate(string buff);
void parseLine(string line,int indexC);


//all checks
void printOptions();
void printVariables();
void printClauses();
void printAssignment();
void debugProblem();
	//tools
	template <class T> void printVector(vector<T>& vec);


class Process{
			/*options*/
			double cb;
			double ct;
			float eps;
			int fct;
			int ict;
			int flips=10;
			int steps=10;


			/*lookUptable*/
			double* lookUpTable;

			/*assignment*/
			vector<int> numP;
			int* unsatCs;
			int unsatN;
			double* probs;
			bool* assign;

			/*tabu stack*/
			bool tabu_flag;
			int* tabuS;

	public:
			Process(string parameter);
			void optimal();
			// lookup table
			void initLookUpTable_exp();
			void initLookUpTable_poly();
			// assignment
			void randomBiasAssignment();
			void biasAssignment();
			void randomAssignment();
			void setAssignment();
				// check
				void printParameters();
				void debugAssign();
				void printAssignment();
				void printUnsatCs();
				void printNumP();

			void test();
			void testLine(string line);

			void search_prob();
			int getFlipCandidate(int cIndex);
			void flip(int j);
			int computeBreakScore(int index);


};



#endif /* PL_H_ */
