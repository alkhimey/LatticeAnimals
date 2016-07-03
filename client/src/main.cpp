#define MAIN_CPP_


#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <utility>

#include "getopt_pp.h"
#include "alphanum.hpp"
#include "timing.hpp"
#include "easylogging++.h"
#include "simplehttp.h"

#include "redelemeier.h"
#include "counting_algorithm.h"
#include "convex_counter.h"
#include "wc_lattice_animal.h"
#include "wc_filtered_lattice_animal.h"
#include "sc_lattice_animal.h"
#include "adj_count_lattice_animal.h"
#include "simple_counter.h"
#include "histogram_counter.h"

#include "main.h"


using namespace std;
using namespace GetOpt;

INITIALIZE_EASYLOGGINGPP

#define CLIENT_VERSION "2.4"
#define DEFAULT_HOST "clown.pythonanywhere.com"
#define ALLOCATE_PAGE "allocate"
#define REPORT_PAGE "report"
#define DEFAULT_PORT_NO 80
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

  //  make_pair("trivial redelemeier 3d (obsolete)", redelemeier_recursive_3d),
  //  make_pair("trivial redelemeier weak convex 3d  (obsolete)", redelemeier_3d_line_convex),
  // make_pair("trivial redelemeier strong convex 3d (obsolete)", redelemeier_3d_full_convex),

  make_pair("count total adjacent cells 2d", redelemeier_main<AdjCountLatticeAnimal, SimpleCounter, 2>),
  make_pair("count total adjacent cells 3d", redelemeier_main<AdjCountLatticeAnimal, SimpleCounter, 3>),

  make_pair("count histo adjacent cells 2d", redelemeier_main<AdjCountLatticeAnimal, HistogramCounter, 2>),
  make_pair("count hosto adjacent cells 3d", redelemeier_main<AdjCountLatticeAnimal, HistogramCounter, 3>),

  //make_pair("strongly convex fixed 2d", redelemeier_main<StronglyConvexLatticeAnimal, SimpleCounter, 2>),
  //make_pair("strongly convex fixed 3d", redelemeier_main<StronglyConvexLatticeAnimal, SimpleCounter, 3>),
  
  //make_pair("redelemeier with pruning weak convex 3d (obsolete)", redelemeier_with_pruning::line_convex_counter_3d),
  //make_pair("redelemeier with pruning strong convex 3d (obsolete)", redelemeier_with_pruning::full_convex_counter_3d)

  make_pair("strongly convex fixed 2d", redelemeier_main<StronglyConvexLatticeAnimal, SimpleCounter, 2>),
  make_pair("strongly convex fixed 3d", redelemeier_main<StronglyConvexLatticeAnimal, SimpleCounter, 3>),
  make_pair("strongly convex fixed 4d", redelemeier_main<StronglyConvexLatticeAnimal, SimpleCounter, 4>),
  make_pair("strongly convex fixed 5d", redelemeier_main<StronglyConvexLatticeAnimal, SimpleCounter, 5>),

  make_pair("weakly convex fixed 2d simple algorithm using filter", redelemeier_main<WeaklyConvexFilteredLatticeAnimal, SimpleCounter, 2>),
  make_pair("weakly convex fixed 3d simple algorithm using filter", redelemeier_main<WeaklyConvexFilteredLatticeAnimal, SimpleCounter, 3>),
  make_pair("weakly convex fixed 4d simple algorithm using filter", redelemeier_main<WeaklyConvexFilteredLatticeAnimal, SimpleCounter, 4>)
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
  
  LOG(INFO) << "Starting program. Client version: " << CLIENT_VERSION;

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
      LOG(ERROR) << "Client does not support alogrithm id " << algo_id;
      exit(0);
    }

    if(n < n0) {
      LOG(ERROR) << "n=" << n << " can't be lower than " << "n0=" << n0; 
      exit(0);
    }

    if (dump_file_name != "") {
      LOG(INFO) << "Attempting to open dump file " << dump_file_name;
      dump_file.open(dump_file_name.c_str());
      if (dump_file.fail()) {
	LOG(WARNING) << "Could not open dump file " << dump_file_name;
      } else {
      LOG(INFO) << "Opened dump file "<< dump_file_name <<" succesfully";
      }
    }

    LOG(INFO) << "Running algorithm \"" << ALGORITHMS[algo_id].first << "\"...";
    
    std::map<std::string, count_t> results;
    
    double t0 = get_cpu_time();
    
    ALGORITHMS[algo_id].second(n, n0, lowId, hightId, &results, &dump_file);    
    
    double t1 = get_cpu_time();

    dump_file.close();

    LOG(INFO) << "Finished running algorithm. This took " << t1 - t0 << " cpu time";

    printResults(results);
    
    /* If we need to report result back to server */
    if(host != "") {
      reportResultsToServer(host, portno,  secret, lowId, hightId, results, t1-t0);
    } else {
      break;
    }
  }

  LOG(INFO) << "Program finished";

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
  
  if(sscanf(content, "%d %d %d %lu %lu", algo_id, n, n0, lowId, hightId) != 5)
    return 0;
  
  return 1;
}


int parseU(const char* s, unsigned int* u) {
  return sscanf(s, "%u", u) == 1;
}


int parseCount(const char* s, count_t* ull) {
  return sscanf(s, "%lu", ull) == 1;
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

  LOG(INFO) << "Parsing command line parameters...";
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
	success &= parseCount(args[2].c_str(), lowId);
	success &= parseCount(args[3].c_str(), hightId);
      } else
	throw GetOpt::GetOptEx();
    }
    
    ops.end_of_options();
  } catch(GetOpt::GetOptEx ex) {
    success &= false;
  }
  
  if(!success) {
    LOG(ERROR) << "Can not parse command line parameters";
    cout << endl;
    usage(ops.app_name());
  }
}


bool getJobFromServer(string host, int portno,  string *secret, int* algo_id, unsigned int *n, unsigned int *n0, count_t *lowId, count_t *hightId) {
  
  LOG(INFO) << "Will attempt to recieve job from server...";
  
  LOG(INFO) << "Connecting to " << host << ":" << portno << "...";
  
  char* response = httpGet(host.c_str(), ALLOCATE_PAGE, portno);

  if(isResponseOk(response) == false) {
    LOG(ERROR) << "Bad response from server";
    return false;
  }


  char* content = extractContent(response);

  if(content == NULL) {
    LOG(ERROR) << "Could not extract the content from the response";
    return false;
  }
  
  string expectedVersion;
  
  content = readNextWord(content, &expectedVersion);

  if (expectedVersion.compare("0") == 0) {
    LOG(INFO) << "No more jobs available. Prorbably because server allocated all jobs"; 
    return false;
  } else if (expectedVersion.compare(CLIENT_VERSION) != 0) {
    LOG(ERROR) << "Version mismatch, server is expecting client version " << expectedVersion; 
    return false;
  }
  
  if(splitContent(content, secret, algo_id, n, n0, lowId, hightId) == 0) {
    LOG(ERROR) << "Could not parse server response";
    return false;
  }
  
  LOG(INFO) << "Recieved job from server n=" << *n << " n0=" << *n0 << " algo_id=" << *algo_id << " lowId=" << *lowId << " hightId=" << *hightId;
  return true;
}

void reportResultsToServer(string host, int portno, string secret, count_t lowId, count_t hightId, std::map< std::string, count_t > results, double cpuTime) {

  LOG(INFO) << "Reporting results to the server...";

  stringstream req;
  req << REPORT_PAGE << "?secret=" << secret << "&low=" << lowId << "&hight=" << hightId << "&cpu=" << cpuTime << "&res=";

  for (std::map<std::string, count_t>::iterator i = results.begin(); i != results.end(); i++) {
    if (i != results.begin()) {
      req << "+";
    }

    req << i->first << ":" << i->second;
  }

  httpGet(host.c_str(), req.str().c_str(), portno);

  LOG(INFO) << "Finished reporting results";

}


void printResults(std::map<std::string, count_t> results) {

  std::priority_queue < std::string, std::vector< std::string >, doj::alphanum_great< std::string > > q;

  for (std::map<std::string, count_t>::iterator i = results.begin(); i != results.end(); i++) {
    q.push(i->first);
  }

  while(q.empty() == false) {
    LOG(INFO) << "Result for " << q.top() << "\t is " << results[q.top()];
    q.pop();
  }
}

