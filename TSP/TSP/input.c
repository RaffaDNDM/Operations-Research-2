#include "input.h"

void parse_cmd(char** argv, int argc, tsp_instance* tsp_in)
{
	//at leat one param + program name
	assert(argc >1 );
	char** commands = malloc(sizeof(char*)*NUM_COMMANDS);

	tsp_in->num_nodes = -1;
	tsp_in->deadline = DEADLINE_MAX;
	tsp_in->alg = 1;
	tsp_in->integerDist = 0; 
	tsp_in->plot = 1;
	tsp_in->verbose = VERBOSE;

	int def_deadline = 0;

	strcpy(tsp_in->input, "NULL");

	int i = 1;
	for (; i < argc; i++)
	{
		if (strncmp(argv[i], "-alg", 4) == 0)
		{
			float algF = atof(argv[++i]);
			int alg = (int) algF;
			
			//the value inserted by the user must be an integer (alg!=0 && algF==alg) 
			//but also the value must be greater than zero
			assert(alg>0 && alg<=NUM_ALGS && algF==alg); 
			tsp_in->alg = alg;

			continue;
		}

		if (strncmp(argv[i], "-int", 4) == 0 || strncmp(argv[i], "-i", 2) == 0)
		{
			tsp_in->integerDist = 1;
			continue;
		}

		if (strncmp(argv[i], "-v", 2) == 0 || strncmp(argv[i], "-verbose", 8) == 0)
		{
			tsp_in->verbose = 100;
			continue;
		}

		if (strncmp(argv[i], "-noplot", 7) == 0 || strncmp(argv[i], "-np", 3) == 0)
		{
			tsp_in->verbose = 100; //more print because no plotting of solution
			tsp_in->plot = 0;
			continue;
		}

		if ((strncmp(argv[i], "-d", 2) == 0 || strncmp(argv[i], "-dead",5) == 0 || strncmp(argv[i], "-deadline",9) == 0) && !def_deadline)
		{
			double deadline =(double) atof(argv[++i]);

			//the value inserted by the user must be a floating point number (deadline!=0 ) 
			//but also the value must be greater than zero
			assert(deadline > 0 && deadline <= DEADLINE_MAX);

			def_deadline = 1;
			tsp_in->deadline = deadline;
			continue;
		}

		if ((strncmp(argv[i], "-f", 2) == 0 || strncmp(argv[i], "-file", 5) == 0 || strncmp(argv[i], "-input", 6) == 0))
		{
			if (strncmp(tsp_in->input, "NULL", 4) == 0)
			{
				strcpy(tsp_in->input, argv[++i]);
				continue;
			}
			
		}

		if ((strncmp(argv[i], "-help", 5) == 0 || strncmp(argv[i], "-h", 2) == 0) && (argc ==2))
		{
			help();
			continue;
		}
	}

	if (tsp_in->verbose > 80)
	{
		printf(LINE);
		printf("List of parameters specified on command line: \n");
		int i;
		for (i=1; i<argc; i=i+2)
		{
			int v_check = strncmp(argv[i], "-v", 2) == 0 || strncmp(argv[i], "-verbose", 8) == 0;
			int h_check = strncmp(argv[i], "-help", 5) == 0 || strncmp(argv[i], "-h", 2) == 0;
			
			if ( v_check|| h_check )
			{
				printf("%s\n", argv[i]);
				i = i - 1;
			}
			else
			{
				printf("%s : %s\n", argv[i], argv[i + 1]);
			}
			
		}
		printf(LINE);
	}
}

void help()
{
	printf(LINE);
	printf("                                       Help\n");
	printf(LINE);
	printf("Insert the file in input\n");
	printf("-f file_name.tsp\n");
	printf("-file file_name.tsp         where file_name = name of tsp file (input instance)\n");
	printf("-input file_name.tsp\n");
	printf(STAR_LINE);
	printf("Insert the max time of the execution\n");
	printf("-d dead_time\n");
	printf("-deadline dead_time         where dead_time = max execution time in seconds (float)\n");
	printf("-dead dead_time\n");
	printf(STAR_LINE);
	printf("Write -v or -verbose if you want information during the execution\n");
	printf(LINE);
	printf("Write -np or -noplot if you want only verbose info and no plot \n");
	printf(LINE);
	exit(0);

}


void parse_file(tsp_instance* tsp_in)
{
	FILE* f = fopen(tsp_in->input, "r");

	assert(f != NULL);
	
	tsp_in->num_nodes = -1;

	char line[LINE_SIZE];
	char* token;
	int point_def = 0;

	while (fgets(line, LINE_SIZE, f) != NULL)
	{
		token = strtok(line, " :");
	
		//Check key words in tsp file
		if (strncmp(token, "NAME", 4) == 0)
			continue;

		if (strncmp(token, "COMMENT", 7) == 0)
			continue;

		if (strncmp(token, "TYPE", 4) == 0)
			continue;

		if (strncmp(token, "DIMENSION", 9) == 0)
		{
			if (tsp_in->num_nodes < 0)
				tsp_in->num_nodes = atoi(strtok(NULL, " :"));

			continue;
		}

		if (strncmp(token, "EDGE_WEIGHT_TYPE", 16) == 0)
			continue;

		if (strncmp(token, "NODE_COORD_SECTION", 18) == 0)
		{
			//The number of nodes must be defined before this section
			assert(tsp_in->num_nodes > 0);  

			tsp_in->x_coords = (double*) calloc(tsp_in->num_nodes, sizeof(double));
			tsp_in->y_coords = (double*) calloc(tsp_in->num_nodes, sizeof(double));

			int count = 1;
			while (fgets(line, LINE_SIZE, f) != NULL && count<=(tsp_in->num_nodes))
			{
				int i = atoi(strtok(line, " "));
				
				assert(i>0 && i<=(tsp_in->num_nodes)); //Index in {1,num_nodes} 

				tsp_in->x_coords[i - 1] = atof(strtok(NULL, " "));
				tsp_in->y_coords[i - 1] = atof(strtok(NULL, " "));

				count++;
			}

			assert(count >= (tsp_in->num_nodes)); //Few nodes declarations
			continue;
		}

		if (strncmp(token, "EOF", 3) == 0)
		{
			break;
		}
	}

	fclose(f);

	if (tsp_in->verbose > 80)
	{
		printf(LINE);
		printf("Name of the input instance : %s\n", tsp_in->input);
		printf("Number of input nodes : %d\n",tsp_in->num_nodes);
		if (tsp_in->deadline < DEADLINE_MAX)
			printf("Deadline time : %d\n",tsp_in->deadline);

		printf("\nInput nodes coordinates:\n");

		for (int i = 0; i < tsp_in->num_nodes; i++)
		{
			printf("node %3d : x = %10.2f  y=%10.2f\n", i + 1, tsp_in->x_coords[i], tsp_in->y_coords[i]);
		}

		printf(LINE);
	}
}

void plot_solution(tsp_instance* tsp_in)
{
	//Print of solution on output file
	FILE* f = fopen(SOLUTION_FILENAME, "w");
	
	int i = 0;
	for (; i < tsp_in->num_nodes+1; i++)
	{
		fprintf(f, "%f ", tsp_in->x_coords[tsp_in->sol[i]]);
		fprintf(f, "%f ", tsp_in->y_coords[tsp_in->sol[i]]);
		int x = (i == tsp_in->num_nodes) ? 1 : i+1;
		fprintf(f, "%d \n", x);
	}
	
	fclose(f);

	FILE* pipe = _popen(GNUPLOT_EXE, "w");
	f = fopen(GNUPLOT_STYLE, "r");

	char line[LINE_SIZE];
	while(fgets(line, LINE_SIZE, f)!=NULL)
	{
		fprintf(pipe, "%s ", line);
	}

	fclose(f);
	_pclose(pipe);
}

void dealloc_inst(tsp_instance* tsp_in)
{
	free(tsp_in->x_coords);
	free(tsp_in->y_coords);
	free(tsp_in->sol);
	tsp_in->x_coords = NULL;
	tsp_in->y_coords = NULL;
	tsp_in->sol = NULL;
}