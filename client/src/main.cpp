#define MAIN_CPP_


#include <iostream>
#include <fstream>
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
#include "convex_counter.h"
#include "wc_lattice_animal.h"
#include "adj_count_lattice_animal.h"

#include "simple_counter.h"
#include "histogram_counter.h"

#include "logging.h"
#include "main.h"


using namespace std;
using namespace GetOpt;

#define CLIENT_VERSION "2.0"
#define DEFAULT_HOST "melon"
#define ALLOCATE_PAGE "allocate"
#define REPORT_PAGE "report"
#define DEFAULT_PORT_NO 8000
#define DEFAULT_ALGO_ID 0


vector< pair< string, CountingAlgorithm > > ALGORITHMS = {
  make_pair("fixed 2d", redelemeier_main<LatticeAnimal, SimpleCounter, 2>),
  make_pair("fixed 3d", redelemeier_main<LatticeAnimal, SimpleCounter, 3>),
  make_pair("fixed 4d", redelemeier_main<LatticeAnimal, SimpleCounter, 4>),
  make_pair("fixed 5d", redelemeier_main<LatticeAnimal, SimpleCounter, 5>),
  make_pair("fixed 6d", redelemeier_main<LatticeAnimal, SimpleCounter, 6>),


  make_pair("weakly convex fixed 2d", redelemeier_main<WeaklyConvexLatticeAnimal, SimpleCounter, 2>),
  make_pair("weakly convex fixed 3d", redelemeier_main<WeaklyConvexLatticeAnimal, SimpleCounter, 3>),
  make_pair("weakly convex fixed 4d", redelemeier_main<WeaklyConvexLatticeAnimal, SimpleCounter, 4>),
  make_pair("weakly convex fixed 5d", redelemeier_main<WeaklyConvexLatticeAnimal, SimpleCounter, 5>),

  make_pair("trivial redelemeier 3d (obsolete)", redelemeier_recursive_3d),
  make_pair("trivial redelemeier weak convex 3d  (obsolete)", redelemeier_3d_line_convex),
  make_pair("trivial redelemeier strong convex 3d (obsolete)", redelemeier_3d_full_convex),

  make_pair("count total adjacent cells 2d", redelemeier_main<AdjCountLatticeAnimal, SimpleCounter, 2>),
  make_pair("count total adjacent cells 3d", redelemeier_main<AdjCountLatticeAnimal, SimpleCounter, 3>),

  make_pair("count histo adjacent cells 2d", redelemeier_main<AdjCountLatticeAnimal, HistogramCounter, 2>),
  make_pair("count hosto adjacent cells 3d", redelemeier_main<AdjCountLatticeAnimal, HistogramCounter, 3>),
  
  
  //make_pair("redelemeier with pruning weak convex 3d (obsolete)", redelemeier_with_pruning::line_convex_counter_3d),
  //make_pair("redelemeier with pruning strong convex 3d (obsolete)", redelemeier_with_pruning::full_convex_counter_3d)
};





/*
 * Main entry point
 *
 * This function will:
 * 1. Parse command line parameters.
 * 2. Get job from the server (optional).
 * 3. Run the chosen algorithm.
 * 4. Report results or print them on the screen.
 *
 */
int main(int argc, char* argv[]){ 
  string host = "";
  string dump_file_name = "";
  int portno;
  string secret;
  
  unsigned int n, n0 = 0;
  count_t lowId = 0, hightId = 1;
  int algo_id;

  ofstream dump_file;
  
  /* Configure logger (log4cxx) */
  PropertyConfigurator::configure(LOGGING_CONF_PATH);
 

  LOG4CXX_INFO(logger, "Starting program. Client version: " << CLIENT_VERSION);

  parseCmdParams(argc, argv, &host,&portno, &n, &n0, &lowId, &hightId, &algo_id, &dump_file_name);
  
  /* Main loop (only one iteration if host is not present) */
  while(true) {
 
    /* If we need to get more parameters from server */
    if(host != "") {
      if(getJobFromServer(host, portno, &secret, &algo_id, &n, &n0, &lowId, &hightId) == false) {
	break;
      }
    }
    
    if(algo_id >= (int)ALGORITHMS.size()) {
      LOG4CXX_ERROR(logger, "Client does not support alogrithm id " << algo_id);
      exit(0);
    }

    if(n < n0) {
      LOG4CXX_ERROR(logger, "n=" << n << " can't be lower than " << "n0=" << n0); 
      exit(0);
    }

    if (dump_file_name != "") {
      LOG4CXX_INFO(logger, "Attempting to open dump file " << dump_file_name);
      dump_file.open(dump_file_name.c_str());
      if (dump_file.fail()) {
	LOG4CXX_WARN(logger, "Could not open dump file " << dump_file_name);
      } else {
	LOG4CXX_INFO(logger, "Opened dump file "<< dump_file_name <<" succesfully");
      }
    }

    LOG4CXX_INFO(logger, "Running algorithm \"" << ALGORITHMS[algo_id].first << "\"...");
    
    vector<count_t> results = vector<count_t>(n + 1, 0);
    clock_t c0, c1;
    c0 = clock();

    ALGORITHMS[algo_id].second(n, n0, lowId, hightId, &results, &dump_file);    

    c1 = clock();
    double t = (c1-c0)/ (double) CLOCKS_PER_SEC;		
    
    dump_file.close();

    printResults(results);
    LOG4CXX_INFO(logger, "Finished running algorithm");
    
    /* If we need to report result back to server */
    if(host != "") {
      reportResultsToServer(host, portno,  secret, lowId, hightId, results, t);
    } else {
      break;
    }
  }
  
  
  LOG4CXX_INFO(logger, "Program finished");

#if defined (_WIN32)
  system("pause");
#endif
  return 0;
}


/* Read a word starting at the start pointer, put it into result and return a pointer to a
 * location aftr the word.
 */
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
  
  if(sscanf(content, "%d %d %d %llu %llu", algo_id, n, n0, lowId, hightId) != 5)
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
  cout << "Usages:" << endl;
  cout << app_name << " -s <hostname>" << endl;
  cout << app_name << " -s <hostname> -p <port_number>" << endl;
  cout << app_name << " <n>" << endl;
  cout << app_name << " <n> -f <output_file_name>" << endl;
  cout << app_name << " <n> --algo_id <algo_id>" << endl;
  cout << app_name << " <n> <split_n> <low_id> <high_id>" << endl;
  cout << app_name << "" << endl;
  cout << endl;
  cout << "Available algorithms are:" << endl;
  for(unsigned int i = 0; i < ALGORITHMS.size(); i++){
    cout << i << "\t" << ALGORITHMS[i].first;
    if( i == DEFAULT_ALGO_ID)
      cout << " (default)"; 
    cout << endl;
  }

#if defined (_WIN32)
  system("pause");
#endif
  exit(-1);
}

void parseCmdParams(int argc, char* argv[], string *host, int *portno, unsigned int *n, unsigned int *n0, 
		    count_t *lowId, count_t *hightId, int *algo_id, string* dump_file_name) {

  //LOG4CXX_INFO(logger, "Command line: " << (*argv)); // TODO: Print command line to log
  LOG4CXX_INFO(logger, "Parsing command line parameters...");
  GetOpt_pp ops(argc, argv);
  ops.exceptions_all(); 
  bool success = true;
  try {
    // TODO: It is possible to rearrange some stuff here.
    if(!ops.options_remain()) {
      *host = DEFAULT_HOST;
      *portno = DEFAULT_PORT_NO;
    } else if (ops >> OptionPresent('h', "help")) {
      usage(ops.app_name());
    } else if (ops >> OptionPresent('s', "server")) {
      ops >> Option('s', "server", *host);
      if(ops >> OptionPresent('p', "port")) 
	ops >> Option('p', "port", *portno);
      else
	*portno = DEFAULT_PORT_NO;
    } else if(ops >> OptionPresent('p', "port")) { 
      ops >> Option('p', "port", *portno);
      *host = DEFAULT_HOST;
    } else {	
      if(ops >> OptionPresent('f')) {
	ops >> Option('f', *dump_file_name);
      } else {
	*dump_file_name = "";
      }

      if(ops >> OptionPresent("algo_id")) {
	ops >> Option("algo_id", *algo_id);
	if(*algo_id < 0 || *algo_id >= (int)ALGORITHMS.size())
	  throw GetOpt::GetOptEx();

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
    LOG4CXX_ERROR(logger, "Can not parse command line parameters");
    cout << endl;
    usage(ops.app_name());
  }
}


bool getJobFromServer(string host, int portno,  string *secret, int* algo_id, unsigned int *n, unsigned int *n0, count_t *lowId, count_t *hightId) {
  
  LOG4CXX_INFO(logger, "Will attempt to recieve job from server...");
  
  LOG4CXX_INFO(logger, "Connecting to " << host << ":" << portno << "...");
  
  char* response = httpGet(host.c_str(), ALLOCATE_PAGE, portno);

  if(isResponseOk(response) == false) {
    LOG4CXX_ERROR(logger, "Server error (response not ok)");
    return false;
  }


  char* content = extractContent(response);

  if(content == NULL) {
    LOG4CXX_ERROR(logger, "Could not extract the content from the response");
    return false;
  }
  
  string expectedVersion;
  
  content = readNextWord(content, &expectedVersion);

  if (expectedVersion.compare("0") == 0) {
    LOG4CXX_INFO(logger, "No more jobs available. Prorbably because server allocated all jobs"); 
    return false;
  } else if (expectedVersion.compare(CLIENT_VERSION) != 0) {
    LOG4CXX_ERROR(logger, "Version mismatch, server is expecting client version " << expectedVersion); 
    return false;
  }
  
  if(splitContent(content, secret, algo_id, n, n0, lowId, hightId) == 0) {
    LOG4CXX_ERROR(logger, "Could not parse server response ");
    return false;
  }
  
  LOG4CXX_INFO(logger, "Recieved job from server n=" << *n << " n0=" << *n0 << " algo_id=" << *algo_id << " lowId=" << *lowId << " hightId=" << *hightId);
  return true;
}

void reportResultsToServer(string host, int portno, string secret, count_t lowId, count_t hightId, vector<count_t> mycount, double cpuTime) {


  LOG4CXX_INFO(logger, "Reporting results to the server...");

  stringstream req;
  req << REPORT_PAGE << "?secret=" << secret << "&low=" << lowId << "&hight=" << hightId << "&cpu=" << cpuTime << "&res=";
  unsigned int i = 0;
  while(mycount[i] == 0)
    i++;
  for(; i < mycount.size(); i++) {
    req << i << ":" << mycount[i];
    if(i < mycount.size() - 1) {
      req << "+";
    }
  }

  httpGet(host.c_str(), req.str().c_str(), portno);

  LOG4CXX_INFO(logger, "Finished reporting results");

}

void printResults(vector<count_t> mycount) {
  unsigned int i = 0;

  while(mycount[i] == 0)
    i++;
  for(; i < mycount.size(); i++) {
    if(logger == NULL) {
      cout << i << "\t" << mycount[i] << endl;
    } else {
      LOG4CXX_INFO(logger, "Result for " << i << "\t is " << mycount[i]);
    }
  }
}

