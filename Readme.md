This software package is a tool which allows running distributed algorithms for enumerating lattice animals. 

Provided  are several implementations that enumerate various classes of lattice animals as well as aggregating the results in various ways is possible.

![Free pentominoes](https://upload.wikimedia.org/wikipedia/commons/thumb/a/aa/All_18_Pentominoes.svg/440px-All_18_Pentominoes.svg.png)

This is a two part system:

1. Client - Each client can run in either a tand-alone mode, where it excutes a specific algorithm with a set of parameters or as part of a distributed counting effort where it recieves the jobs from a server.

2. Sever - The server manages the enumeration effort. It allocates jobs and returns then to clients on requests, it also recieves the results and stores them in a database.



## The client


### Building
The client code is written in C++ and it does not use external libraries. 

There are two targets you can compile it for: linux (native) and windows (cross compiled using MingW). The taget is determined using the `PLAT` parameter to the make file.

For example, to build a release version for Windows, run the follwoing command:

```make CFG=release PLAT=linux```

This will produce the following files:

```
client/bin/run_linux_release
client/bin/unit_tests_linux_release
```

### Running

```
$client/bin/run_linux_release -h
2017-06-09 23:16:07,541 INFO  [default] Starting program. Client version: 2.4
2017-06-09 23:16:07,541 INFO  [default] Parsing command line parameters...
Version 2.4
Usages:
./../client/bin/run_linux_release -s <hostname>
./../client/bin/run_linux_release -s <hostname> -p <port_number>
./../client/bin/run_linux_release <n>
./../client/bin/run_linux_release <n> -f <output_file_name>
./../client/bin/run_linux_release <n> --algo_id <algo_id>
./../client/bin/run_linux_release <n> <split_n> <low_id> <high_id>
./../client/bin/run_linux_release

Available algorithms are:
0	fixed 2d (default)
1	fixed 3d
2	fixed 4d
3	fixed 5d
4	fixed 6d
5	weakly convex fixed 2d
6	weakly convex fixed 3d
7	weakly convex fixed 4d
8	weakly convex fixed 5d
9	count total adjacent cells 2d
10	count total adjacent cells 3d
11	count histo adjacent cells 2d
12	count hosto adjacent cells 3d
13	strongly convex fixed 2d
14	strongly convex fixed 3d
15	strongly convex fixed 4d
16	strongly convex fixed 5d
17	weakly convex fixed 2d simple algorithm using filter
18	weakly convex fixed 3d simple algorithm using filter
19	weakly convex fixed 4d simple algorithm using filter
20	trivial redelemeier weak convex 3d  (obsolete)
```

### How to add a new algorithm

First you need to add a new entry in the `ALGORITHMS` array in `main.cpp`:

```C++
vector< pair< string, CountingAlgorithm > > ALGORITHMS = {
  make_pair("fixed 2d", redelemeier_main<LatticeAnimal, SimpleCounter, 2>),
  make_pair("fixed 3d", redelemeier_main<LatticeAnimal, SimpleCounter, 3>),
  make_pair("fixed 4d", redelemeier_main<LatticeAnimal, SimpleCounter, 4>),
  ...
  make_pair("my new shiny implementation", pointer_to_afunction),
  
};
```

The first paramter in the pair is a human readable description of the implementation.

The second parameter is a pointer to a function that runs the implemnentation.

The signature of the function is define in `counting_algorithm.h`:

```C++
/* Prototype for a counting algorthim entry point 
 * 
 * @param n		The maximum size of the lattice animal to count
 * @param n0		
 * @param low_id	
 * @param high_id	
 * @results             
 * @param dump_file     Pointer to the dump stream. The stream should be already open for writing. If stream is
 *                      open, the algortihm should dump all the polycubes that it has counted.
 */ 
typedef void (*CountingAlgorithm) (coord_t n,
				   coord_t n0,
				   count_t low_id,
				   count_t  high_id,
				   std::map< std::string, count_t > *results,
				   std::ofstream* dump_file);
```

## The server

The source code of the server is located in `/counting_site`. Do not be confused with `/server` which is an obsolte version based on Google's app engine. 

The server is powered by Django framework and it uses MySQL as a database.

### Prerequirments for running localy


### Running localy

### Adding a configuration



### Info page

The `/info` page will display all `Config`s in the system:

![info page](/docs/pics/info_page.png)


It is possible to view information about progress and other statistics by going to the info page of an individual config:

![info 16 page](/docs/pics/info_16_page.png)

### Deploying



## Protocol

The communication between the server and the client is based on HTTP. 
The servers runs an HTTP server and the client is the on who initiates the communication.

### Versioning

Client's version can be changed by editing the line

```
#define CLIENT_VERSION "2.4"
```
in `main.cpp`.

Client's version must match the version specofoed in the server's active Config.


### Allocating and obtaining new job

To obtain a new job, the client sends a `GET` request to `/allocate`.

In the body of the response, will contain a lije of the following structure:

```
<client_version> <secret_hash> <algo_id> <n> <n0> <low_id> <high_id>
```

* **client_version** - The version of the client that the server requires. If the client version if deifferent from this value, it shuld not proccess the job.
* **secret_hash** - Client should use it to identify the job when reporting it back to the server.
* **algo_id - The algorithm implementation to run. The mapping between the id and the actual imlementation is decided by the client and is version dependent.
* **n** - The maximal size of the lattice animnal we want to count.
* **n0** - The size at which to split the counting
* **low_id, high_id** - the

If the server can not allocate any job at the time, it will send the following response:
```
0 0 0 0 0 0 0
```
 

### Reporting the results of job execution

After completing the computation, the client is required to send a GET request that looks like this:

```
/report?secret=<secret_hash>&res=1:<count_1>+2:<count_2>+...
```

## Some theory

### Lattice animal classes

Lattice animal classes can have the following characteristics:

1. Fixed or free - whether or not two symmetrical lattice animals are considered distinct.
2. Dimension - polycubes and higher dimensions.
3. Common property - for example all the convex polyominoes.
4. Lattice - for example polyiamond (triangle lattice) or polyhex (hexagonal lattice).
5. Neighbor confections - for example polyknights


## Unit tests

