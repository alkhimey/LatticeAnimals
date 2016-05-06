This document is a brief introduction to the lattice animal distributed counter.

It will explain how to compile and use the software as well as how to extend it with new algorithms.

Itroduction
===========

The idea behind distributed lattice animal counter is that Redelemeier algorithm can be easily split into seperate independent jobs.

Our implementation relies on a modified Redelemeier implementation to count different classes of lattice animals.

It allows counting different classes of lattice animals, in different dimesions as well as different ways of aggregating the results (for example histogram of some parameter of the lattice animal).

There are two major software parts in our scheme:

* The server part performs on demand allocation of jobs to clients, recieves the results and keeps track of the aggregated results. The server part is implemented in the Django framework, with sqlite as the database.
* The client part is written in C++. It can be run as stand-alone, as well as part of the distributed counting effort.

The client 
===========

Build
-----
To build the release linux version of the client, go to the *client* directory and run *make CFG=release PLAT=linux*. The oprion **CFG** can be either

This will build the binary file s well as the unit tests executioner in the *bin* subdirectory.

Run
---
    Here is the ussage message as of version 2.0:

    Version 2.0
    Usages:
    ./bin/run_linux_release -s <hostname>
    ./bin/run_linux_release -s <hostname> -p <port_number>
    ./bin/run_linux_release <n>
    ./bin/run_linux_release <n> -f <output_file_name>
    ./bin/run_linux_release <n> --algo_id <algo_id>
    ./bin/run_linux_release <n> <split_n> <low_id> <high_id>
    ./bin/run_linux_release
    
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
    9	trivial redelemeier 3d (obsolete)
    10	trivial redelemeier weak convex 3d  (obsolete)
    11	trivial redelemeier strong convex 3d (obsolete)
    12	count total adjacent cells 2d
    13	count total adjacent cells 3d
    14	count histo adjacent cells 2d
    15	count histo adjacent cells 3d
    


Design
-------

TODO


Server
======

The server code is located in the *counting_site* directory.

To run the server localy, run:

    python manage.py runserver

You can access the admin interface via the browser:

    http://127.0.0.1:8000/admin

To configure a new counting effort, go into the *Config* page, click new config and fill in the desired paramters.

Once a configuration is activated, it will be used as the allocation source for new job allocation requests. The server will always use the newest activated configuration.

The *Config* edit page will also display the current result and various statistics regarding the progress of the counting effort.
