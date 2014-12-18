#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <utility>
#include "getopt_pp.h"
#include "redelemeier.h"
#include "simplehttp.h"
#include "counting_algorithm.h"


using namespace std;
using namespace GetOpt;


#define CLIENT_VERSION "1.3"
#define DEFAULT_HOST "alkhimey.appspot.com"
#define ALLOCATE_PAGE "allocate"
#define REPORT_PAGE "report"
#define DEFAULT_PORN_NO 80
#define DEFAULT_ALGO_ID 0

#define NUMBER_OF_ALGORITHMS = 1;
vector < const pair < string, CountingAlgorithm > > ALGORITHMS {
  {"redelemeier-recursive-3d", redelemeier_recursive_3d}
};

/******************************************************************
 Helper functions declarations

******************************************************************/

/* Parse the parameters provided through command line. Exit and print help in case of error. */
void parseCmdParams(int argc, char* argv[], string *host, int *portno, unsigned int *n, unsigned int *n0, count_t *lowId, count_t *hightId, int *algo_id);

/* Get job details from the server. Returns false if no more jobs are available. Exit program in case of error */
bool getJobFromServer(string host, int portno, string *secret, int *algo_id, unsigned int *n, unsigned int *n0, count_t *lowId, count_t *hightId);
void reportResultsToServer(string host, int portno, string secret, count_t lowId, count_t hightId, vector<count_t> mycount, double cpuTime);

/* Print the results in a nicely formatted way. */
void printResults(vector<count_t> mycount);

/* Read a word untill space from the buffer and store the word into the result string. Returns the beginning of the next word. */ 
char* readNextWord(char* start, string* result);

/* Will return 0 if the content is ill formatted. */
int splitContent(char* content,string* secret, int* algo_id, unsigned int* n, unsigned int* n0, count_t* lowId, count_t* hightId);

/* Parse the string s into the variable provided. Returns 0 is ill formatted, 1 otherwise. */
int parseU(const char* s, unsigned int* u);
int parseUll(const char* s, unsigned long long int* ull);

/* Print usage info and exit */
void usage(string);


/*
 * Main entry point
 *
 * This function will:
 * 1. Parse command line parameters.
 * 2. Get job from the server (optional).
 * 3. Run the algorithm on.
 * 4. Report results or print them on the screen.
 *
 */
int main(int argc, char* argv[]) {
  string host = "";
  int portno;
  string secret;
  
  unsigned int n, n0 = 0;
  count_t lowId = 0, hightId = 1;
  int algo_id;
  
  // Parse command line parameters
  parseCmdParams(argc, argv, &host,&portno, &n, &n0, &lowId, &hightId, &algo_id);
  
  // Main loop (only one iteration if host is not present)
  while(true) {
 
   // If we need to get more parameters from server
    if(host != "") 
      if(getJobFromServer(host, portno, &secret, &algo_id, &n, &n0, &lowId, &hightId) == false)
	break;
    
    // Do the algortihm
    //  cout << "Running Redelemeier algorithm with predicate " << predName << "..." << endl;

    vector<count_t> results = vector<count_t>(n + 1, 0);
    clock_t c0, c1;
    c0 = clock();
    //runRedelemeier(n, n0, lowId, hightId, predicateMap(predName), &mycount);
    ALGORITHMS[algo_id].second(n, n0, lowId, hightId, &results);    

    c1 = clock();
    double t = (c1-c0)/ (double) CLOCKS_PER_SEC;		
    
    printResults(results);
    cout << "This took " << t << " seconds" << endl;
    
    // If we need to report result back to server
    if(host != "") {
      reportResultsToServer(host, portno,  secret, lowId, hightId, results, t);
    } else {
      break;
    }
  }
  
  
#if defined (_WIN32)
  system("pause");
#endif
  return 0;
}

/******************************************************************
 Helper functions definitions

******************************************************************/

char* readNextWord(char* start, string* result) {
  stringstream s;
  
  char* i = start;
  while(*i != ' ') { 
    s << *i;
    i++;
  }
  (*result) = s.str();
	
  return i+1; // skip the next space char
}

int splitContent(char* content, string* secret, int* algo_id, unsigned int* n, unsigned int* n0, count_t* lowId, count_t* hightId) {
  content = readNextWord(content, secret);
  
  if(sscanf(content, "%d %d %d %llu %llu", algo_id, n, n0, lowId, hightId) != 4)
    return 0;
  
  return 1;
}


int parseU(const char* s, unsigned int* u) {
  return sscanf(s, "%u", u) == 1;
}


int parseUll(const char* s, unsigned long long int* ull) {
  return sscanf(s, "%llu", ull) == 1;
}


void usage(string app_name) {
  cout << "Version " << CLIENT_VERSION << endl;
  cout << "Usage examples:" << endl;
  cout << app_name << " -s localhost" << endl;
  cout << app_name << " -s localhost -p 8080" << endl;
  cout << app_name << " 8" << endl;
  cout << app_name << " 8 --algo_id 1" << endl;
  cout << app_name << " 10 8 154 1043" << endl;
  cout << app_name << "" << endl;
  cout << endl;
  
#if defined (_WIN32)
  system("pause");
#endif
  exit(-1);
}

void parseCmdParams(int argc, char* argv[], string *host, int *portno, unsigned int *n, unsigned int *n0, 
		    count_t *lowId, count_t *hightId, int *algo_id) {
  GetOpt_pp ops(argc, argv);
  ops.exceptions_all(); 
  bool success = true;
  try {
    // TODO: It is possible to rearrange some stuff here.
    if(!ops.options_remain()) {
      *host = DEFAULT_HOST;
      *portno = DEFAULT_PORN_NO;
    } else if (ops >> OptionPresent('h', "help")) {
      usage(ops.app_name());
    } else if (ops >> OptionPresent('s', "server")) {
      ops >> Option('s', "server", *host);
      if(ops >> OptionPresent('p', "port")) 
	ops >> Option('p', "port", *portno);
      else
	*portno = DEFAULT_PORN_NO;
    } else if(ops >> OptionPresent('p', "port")) { 
      ops >> Option('p', "port", *portno);
      *host = DEFAULT_HOST;
    } else {	
      if(ops >> OptionPresent("algo_id")) {
	ops >> Option("algo_id", *algo_id);
	// TODO: Make sure id exists!
	//	if(predicateMap(*predName) == NULL)
	//  throw GetOpt::GetOptEx();
      } else {
	*algo_id = DEFAULT_ALGO_ID;
      }
      
      vector<string> args;	 
      ops >> GlobalOption(args);
      if(args.size() == 1) 
	success &= parseU(args[0].c_str(), n);
      else if (args.size() == 4) {
	success &= parseU(args[0].c_str(), n);
	success &= parseU(args[1].c_str(), n0);
	success &= parseUll(args[2].c_str(), lowId);
	success &= parseUll(args[3].c_str(), hightId);
      } else
	throw GetOpt::GetOptEx();
    }
    
    ops.end_of_options();
  } catch(GetOpt::GetOptEx ex) {
    success &= false;
  }
  
  if(!success) {
    cout << "ERROR parsing parameters" << endl << endl;
    usage(ops.app_name());
  }
}


bool getJobFromServer(string host, int portno,  string *secret, int* algo_id, unsigned int *n, unsigned int *n0, count_t *lowId, count_t *hightId) {
  cout << "Connecting to server...." << endl;
  
  char* response = httpGet(host.c_str(), ALLOCATE_PAGE, portno);
  char* content = extractContent(response);
  
  if(content == NULL) {
    cout << "ERROR extracting content from response" << endl;
    exit(0);
  }
  
  string expectedVersion;
  
  content = readNextWord(content, &expectedVersion);
  
  if (expectedVersion.compare(CLIENT_VERSION) != 0) {
    cout << "Version mismatch, please download client version " << expectedVersion  << endl;
    return false;
  }
  
  if(splitContent(content, secret, algo_id, n, n0, lowId, hightId) == 0) {
    cout << "ERROR parsing server response" << endl;
    return false;
  }
  
  free(response);	
  if (secret->compare("0") == 0) {
    cout << "No more jobs available. Prorbably because server allocated all jobs." << endl;
    return false;
  }
  
  cout << "Recieved job from server n=" << *n << " n0=" << *n0 << " algo_id=" << *algo_id << " lowId=" << *lowId << " hightId=" << *hightId << endl;
  return true;
}

void reportResultsToServer(string host, int portno, string secret, count_t lowId, count_t hightId, vector<count_t> mycount, double cpuTime) {
  stringstream req;
  req << REPORT_PAGE << "?secret=" << secret << "&low=" << lowId << "&hight=" << hightId << "&cpu=" << cpuTime << "&res=";
  unsigned int i = 0;
  while(mycount[i] == 0)
    i++;
  for(; i < mycount.size(); i++)
    req << i << ":" << mycount[i] << "+";
  
  free( httpGet(host.c_str(), req.str().c_str(), portno));
}

void printResults(vector<count_t> mycount) {
  unsigned int i = 0;
  while(mycount[i] == 0)
    i++;
  for(; i < mycount.size(); i++)
    cout << i << "\t" << mycount[i] << endl;
}

