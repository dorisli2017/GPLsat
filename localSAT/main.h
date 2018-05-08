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
#include <stdlib.h>
#include <getopt.h>
using namespace std;

/* file */
static char* filename;

/*settings*/
int output_flag;


/*methods*/
void parseOptions(int argc, char *argv[]);
void readFile(char* fileName);
void printUsage();

#endif /* MAIN_H_ */
