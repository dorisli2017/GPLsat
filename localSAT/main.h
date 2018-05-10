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
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string>
#include <fstream>
#include <string.h>
#include <vector>
#include "miscellaneous.h"

/*problem*/
int numCs;
int numVs;
struct C *clauses;
struct V *variables;

/*settings*/
int output_flag;
int caching_flag;
int tabu_flag;
/*option values*/
unsigned long long int maxTries;
unsigned long long int maxFlips;
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
int parseLine(string line, int index);

#endif /* MAIN_H_ */
