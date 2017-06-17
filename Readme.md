
This is a source code of a tool which allows running distributed algorithms for counting [lattice animals](https://en.wikipedia.org/wiki/Polyomino). 

Provided are several implementations ,which are based on Redelemeier's algorithm, that enumerate various classes of lattice animals as and aggregating the results in various ways.

![Free pentominoes](https://upload.wikimedia.org/wikipedia/commons/thumb/a/aa/All_18_Pentominoes.svg/440px-All_18_Pentominoes.svg.png)

It is possible to extend the system to count additional classes of lattice animals.

Additionaly, a slightly modified version of Redelemeier is introduced. This version enumerates some classes of lattice animals by pruning some search trees instead of enumerating all the possible lattice animals and filtering those which are not inside the class. 

This is a two part system:

1. Client - Each client can run in either a stand-alone mode, in which it executes a specific algorithm with a set of parameters. Or it can run as part of a distributed counting effort where it receives the jobs from a server.

2. Sever - The server manages the counting effort. It allocates jobs and returns then to clients on requests, it also receives the results and stores them in a database.

## Table of contents
1. [The client](#the-client)
    1. [Building](#building)
    2. [Windows](#windows)
    3. [Running](#running)
    4. [How to add a new algorithm](#how-to-add-a-new-algorithm)
    5. [Counting different classes of lattice animals](#counting-different-classes-of-lattice-animals)
    6. [Counting using a different counting method](#counting-using-a-different-counting-method)
    7. [Why D is a generic parameter?](#why-d-is-a-generic-parameter)
    8. [What is the purpose of `can_add` method?](#what-is-the-purpose-of-can-add-method)
2. [The server](#the-server)
    1. [Requirements for running locally](#requirements-for-running-locally)
    2. [Running locally](#running-locally)
    3. [Adding a configuration](#adding-a-configuration)
    4. [Info page](#info-page)
    5. [Deploying](#deploying)
3. [Protocol](#protocol)
    1. [Version](#version)
    2. [Allocating and obtaining new job](#allocating-and-obtaining-new-job)
    3. [Reporting the results of job execution](#reporting-the-results-of-job-execution)
4. [Extra](#extra)

## The client


### Building

The client code is written in C++.  It does not use external libraries and is compiled with `g++`. 

There are two targets you can build it for: Linux (native) and windows (cross compiled using MinGW). The target is determined using the `PLAT` parameter to the make file.

For example, to build a *release* version for *Windows*, run the following command:

```make CFG=release PLAT=Linux```

This will produce the following files:

```
client/bin/run_Linux_release
client/bin/unit_tests_Linux_release
```

### Windows

As was mentioned, to build for windows, MinGW cross compiler needs to be installed. 

```
sudo apt-get install mingw-w64
```

You can use a different cross compiler version. In this case, change the following line in the `Makefile`:

```
CXX=i686-w64-mingw32-g++
```

### Running

```
$client/bin/run_Linux_release -h
2017-06-09 23:16:07,541 INFO  [default] Starting program. Client version: 2.4
2017-06-09 23:16:07,541 INFO  [default] Parsing command line parameters...
Version 2.4
Usages:
./../client/bin/run_Linux_release -s <hostname>
./../client/bin/run_Linux_release -s <hostname> -p <port_number>
./../client/bin/run_Linux_release <n>
./../client/bin/run_Linux_release <n> -f <output_file_name>
./../client/bin/run_Linux_release <n> --algo_id <algo_id>
./../client/bin/run_Linux_release <n> <split_n> <low_id> <high_id>
./../client/bin/run_Linux_release

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

The second parameter is a pointer to a function that runs the implementation.

The signature of the function is define in `counting_algorithm.h`:

```C++
/* Prototype for a counting algorithm entry point 
 * 
 * @param n		The maximum size of the lattice animal to count
 * @param n0		
 * @param low_id	
 * @param high_id	
 * @results             
 * @param dump_file     Pointer to the dump stream. The stream should be already open for writing. If stream is
 *                      open, the algorithm should dump all the polycubes that it has counted.
 */ 
typedef void (*CountingAlgorithm) (coord_t n,
				   coord_t n0,
				   count_t low_id,
				   count_t  high_id,
				   std::map< std::string, count_t > *results,
				   std::ofstream* dump_file);
```

Now, you have two options. You can either provide your own implementation ( a function with the above signature ), or instantiate the generic function `redelemeier_main` found in the same file.

For using your own function, you can look at `redelemeier_3d_line_convex` from the file ``.

The reason that there are several implementations is because this work was done iteratively. In each iteration, a newer implementation was introduced, but the older implementations were retained for cross referencing the results.

If you want to use the already existing, modular infrastructure look at the generic function `redelemeier_main`:

```C++
template <class A, class C, dim_t D>
void redelemeier_main (coord_t n,
		       coord_t n0,
		       count_t low_id,
		       count_t  high_id,
		       std::map< std::string, count_t > *results,
		       std::ofstream* dump_file)
```

In brief, the generic parameters are:

* __A__ - A class which represents the model of the lattice animal.
* __C__ - A class which represents the entity which counts and aggregates the results.
* __D__ - The dimension at which the counter needs to operate.

### Counting different classes of lattice animals

Suppose you want to count convex lattice animals.

A starting point would to look at the `LatticeAnimal` class. In most scenarios, it is possible to derive from this class and override only some of it's methods.

In this particular example, a trivial implementation would override `is_in_class` with a method that tests the lattice animal for it's convexity.

Checking convexity is a heavy operation, and it will run for each lattice animal found by the Redelemeier algorithm. A more sophisticated approach would be to override `add` and `remove` methods as well. It is much faster to check for convexity if the convexity of the original lattice animal is know along with the operation (addition or removal of a cell).

This example can be found in the `wc_lattice_animal.h` file 

### Counting using a different counting method

If all that you want is to count the total perimeter of all lattice animal of size n, then it is enough to provide a custom implementation of class A (change to the `get_count()` method). But is you want to count the number of lattice animals for _each_ perimeter, you will also have to use non default class C implementation.

The default implementation is `SimpleCounter` from `simple_counter.h`, you can use it as an example. It is advisable to derive from it and override the functions. More advanced example is the `HistogramCounter` which can be used exactly for the propose that was described above.

Notice that the `increment` method is called for every counted object. Therefore it must be as efficient as possible.

### Why D is a generic parameter?

Some legacy implementations do not have an option to scale the dimension dynamically, therefore the prototype `CountingAlgorithm` can not have a dimension parameter.

Since `redelemeier_main` must comply with that prototype, the dimension is passed through a generic parameter.

This is not an intrinsic limitation and the code can be rewritten in a way that would allow receiving the dimension as part of a job configuration, independently of the algorithm id.

### What is the purpose of `can_add` method?

TBD

## The server

The source code of the server is located in `/counting_site`. Do not be confused with `/server` which is an obsolete version which is powered by Google's app engine. 

The server is powered by Django framework and it uses MySQL as a database.

### Requirements for running locally

TBD

### Running locally

TBD


### Adding a configuration

TBD

### Info page

The `/info` page will display all `Config`s in the system:

![info page](/docs/pics/info_page.png)


It is possible to view information about progress and other statistics by going to the info page of an individual config:

![info 16 page](/docs/pics/info_16_page.png)

### Deploying

TBD


## Protocol

The communication between the server and the client is based on HTTP. 
The servers runs an HTTP server and the client is the on who initiates the communication.

### Version

Client's version can be changed by editing the line in `main.cpp`.

```
#define CLIENT_VERSION "2.4"
```

Client's version must match the version specified in the server's active Config.


### Allocating and obtaining new job

To obtain a new job, the client sends a `GET` request to `/allocate`.

In the body of the response, will contain a line of the following structure:

```
<client_version> <secret_hash> <algo_id> <n> <n0> <low_id> <high_id>
```

* **client_version** - The version of the client that the server requires. If the client version is different from this value, it should not process the job.
* **secret_hash** - Client should use it to identify the job when reporting it back to the server.
* **algo_id** - The algorithm implementation to run. The mapping between the id and the actual implementation is decided by the client and is version dependent.
* **n** - The maximal size of the lattice animal we want to count.
* **n0** - The size at which to split the counting.
* **low_id, high_id** - The range of the lattice animals not to prune at the n0 level.

If the server can not allocate any job at the time, it will send the following response:
```
0 0 0 0 0 0 0
```
 

### Reporting the results of job execution

After completing the computation, the client is required to send a GET request that looks like this:

```
/report?secret=<secret_hash>&res=1:<count_1>+2:<count_2>+...
```

## Extra

### Performance tips

Life is short and waiting for results is a useless waste of it, so here are some tips that can speed the counting a bit. Some of these are already in the code.

- [V] Use good compiler flags. 

- [V] Precalculate anything you can (for example neighbor of each cell are calculated in `LatticeAnimal` constructor)

- [ ] Determine the best `n0` depending on the number of available clients.

- [ ] Redelemeier algorithm can be implemented without recursion!

- [V] Use iterative approach to determine whether a lattice animal is inside a class (not implemented for strongly convex yet)

- [V] Make sure values are passed by reference and there is no implicit copy constructor invocation in the recursive call of the Redelemeier implementation. 

### Lattice animal classes

Lattice animal classes can have the following characteristics:

1. Fixed or free - whether or not two symmetrical lattice animals are considered distinct.
2. Dimension - polycubes and higher dimensions.
3. Common property - for example all the convex, column convex, directed, strongly convex in higher dimensions etc.
4. Lattice - for example polyiamond (triangle lattice) or polyhex (hexagonal lattice).
5. Neighbor connections - for example polyknights

Here is an article with some ideas: [Some problems in counting of lattice animals, polyominoes, polygons and walks / Andrew Daniel Rechnitzer](http://www.math.yorku.ca/~andrew/documents/thesis.pdf)

## Unit tests

