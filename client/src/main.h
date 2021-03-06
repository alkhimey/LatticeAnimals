#ifndef _MAIN_H_
#define _MAIN_H_

#include <vector>
#include <utility>


/******************************************************************
 Helper functions declarations

******************************************************************/

/* Parse the parameters provided through command line. Exit and print help in case of error. */
void parseCmdParams(int argc, char* argv[], 
		    string *host, int *portno, 
		    unsigned int *n, unsigned int *n0, count_t *lowId, count_t *hightId, 
		    int *algo_id, 
		    string* dump_file_name);

/* Get job details from the server. Returns false if no more jobs are available. Exit program in case of error */
bool getJobFromServer(string host, int portno, string *secret, int *algo_id, unsigned int *n, unsigned int *n0, count_t *lowId, count_t *hightId);

/**
 * Reports the results of running the job back to the server.
 */
void reportResultsToServer(string host, int portno, string secret, count_t lowId, count_t hightId, std::map< std::string, count_t > results, double cpuTime);


/**
 * Prints the results into the logger in a human readable form.
 */
void printResults(std::map<std::string, count_t> mycount);

/* Read a word untill space from the buffer and store the word into the result string. Returns the beginning of the next word. */ 
char* readNextWord(char* start, string* result);

/* Will return 0 if the content is ill formatted. */
int splitContent(char* content,string* secret, int* algo_id, unsigned int* n, unsigned int* n0, count_t* lowId, count_t* hightId);

/* Parse the string s into the variable provided. Returns 0 is ill formatted, 1 otherwise. */
int parseU(const char* s, unsigned int* u);
int parseCount(const char* s, count_t* ull);

/* Print usage info and exit */
void usage(string);


#endif
