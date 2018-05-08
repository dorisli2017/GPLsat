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
	parseOptions(argc, argv);

	//readFile(fileName);


    return 0;
}
/*parse the argument (including options and filename)
 *
 */
void parseOptions(int argc, char *argv[]){
	static struct option longopts[] ={{{"caching, "}}{"output",0,0,'o' }};
	int result;
	cout<<output_flag<<endl;
	while ((result = getopt_long(argc, argv, "oh:",longopts,NULL)) != -1){
		switch (result) {
		case 'o':
			output_flag = 1;
			break;
		default:
			printUsage();
			exit (0);
		}
	}
	cout<<output_flag<<endl;
}
/*
 *
 */
//void readFile(char* fileName){}
/*print the manual of the program*/
void printUsage(){}
