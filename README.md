# CS 561: Data Systems Architecture - Zonemaps


## About

This repo is implemented on TemplateZonemaps (see original repo) with some changes to make testing easier.

## Generating Workloads
You can generate workloads of a specific size, with varying noise levels. A workload generator
is provided as part of the API that performs the required functionality. Here, noise (%) is the percentage (int) 
of out of order elements, and windowThreshold(%) is the window (as percentage of total elements) within which an
out of order element can be placed from its original location. So, a 5% noise and 5% window threshold means 
5% of the total domain size of elements will be out-of-order and each of these out-of-order elements will be 
placed within a 5% window from its original (sorted) location. Compile the workload generator using 
the "make" command, and execute using
```c
./workloadgenerator <domain_size> <noise_percentage> <windowThreshold>
```
For example, to generate a workload of 1 Million integers with 0% noise and 5% windowThreshold, use: 
```c
./workloadgenerator 1000000 0 5
```

## Zone Map Header
The zonemaps.h header file is a template that defines the basic components of a zone map data structure. It is 
required to implement the data structure using this template in a separate file named "zonemaps.cpp". You are 
free to modify certain components of the template to improve performance of the data structure, however the core 
functionality should remain the same. In your implementation, it is important to comment your code to highlight various 
design decisions and logic. 

## Testing 
The main.cpp file provides a foundational testing framework expected for this project. You are required to generate the 
workloads, read the data and build the zone map, perform point queries and range queries. For point queries, use the simple 
query generator function inside the main.cpp file. Refer to the assignment instruction document for range queries. Both point 
queries and range queries are to be timed for evaluating performance. The execution times are to be written either into a log file
or displayed onto the terminal along with 
other relevant details about the workload, such as: domain size, noise percentage, window threshold, etc. 

Compile all files using the "make" command. 
There are currently 4 test cases pre-configured in main.cpp, as per the assignment request. Once the implementation of zone maps is 
complete, you will be required to fill in the appropriate test call in the main.cpp file for the point/range queries. To run a specific test case, 
use the command: 
```c
./main <input_file> <test_case>
```
If no case is given, it will run *all* preconfigured cases.

For example, to run point queries on an input file:
```c
./main ./data_1000000-elems_0-kperct_5-lperct_1seed1611707662.dat test_pq
```

For general tests, just run like:
```c
./main ./data_1000000-elems_0-kperct_5-lperct_1seed1611707662.dat
```

## Some result

On 1 million data with 0/5/25 noise, average point query use 17.79/158/158 ms on my machine. We can see a huge performance decrease when zone map becomes noisy. (Need to scan more zones) But there's a threshold that when it's noisy enough, we need to scan all zones and degenerate to linear scan whole zonemap.

For range query, the performance is almost the same. About 8000 ms for 10-20, 6500 ms for 30-40, 5000 ms for 50-60 and 4000 ms for 70-80. There is some increase of time when noise level grows, but not that much. Not sure why it's faster in higher domain than lower, maybe related to distribution of data?

## Contact

If you have any questions please feel free to see Aneesh in office hours, or
email your queries at aneeshr@bu.edu.
