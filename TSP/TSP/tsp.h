#ifndef TSP

#define TSP

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//Maximum time for the computation of the solution (15 min)
#define DEADLINE_MAX 7200000

//Number of algoritms implemented
#define NUM_ALGS 7

//#define NUM_COMMANDS 8

//default value of verbose
#define VERBOSE 40

/*
Call to python program that generates the file instances.txt
with all the name of the instances in the directory specified
as argument in the command line
*/
#define DIR_LIST_PY "python3 list_dir.py"

//Names of the algorithms
#define ALG1 "Loop"
#define ALG2 "Branch&Cut"
#define ALG3 "Branch&CutGeneral"
#define ALG4 "MTZ"
#define ALG5 "GG"
#define ALG6 "VNS"
#define ALG7 "TabuSearch"

//Creation or not of the performance profile
#define PERF_PROF_ON 1
//Call to python program that creates the performance profile
#define PERF_PROF_PY "python perfprof.py -D , -T 33034 -S 2 -M 20 perf_data.csv pp.pdf -P \"all instances, shift 2 sec.s\""

//Length of the dir string, specified in command line
#define DIR_SIZE 500 

typedef struct
{
	//Input (Graph structure) 
	int num_nodes;
	double* x_coords;
	double* y_coords;
	int num_cols;

	//Parameters
	char input[500];
	double deadline;
	int size;
	int alg;
	int which_alg[NUM_ALGS];
	int integerDist;
	int plot;
	int verbose;
	int heuristic;
	int node_lim;
	int sol_lim;
	double eps_gap;
	int seed;
	char dir[DIR_SIZE];

	//Output
	double* sol;
	int bestCostI;
	double bestCostD;
	double execution_time;

} tsp_instance;

/**
	Compute the solution of the TSP problem
	looking to the algorithm chosen by user on command line
	1 no permutation (default algorithm)
	\param tsp_in reference to tsp instance structure
*/
void solution(tsp_instance* tsp_in);

/**
	Change parameters of CPLEX solver and calls resolution of model
	\param tsp_in reference to tsp instance structure
*/
void set_params_and_solve(tsp_instance* tsp_in);

/**
	Manage file in input and directory and create performance profile
	\param tsp_in reference to tsp instance structure
*/
void manage_input(tsp_instance* tsp_in);
#endif

