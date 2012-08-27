#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#define MODE_SQUARE 0
#define MODE_CUBE 1
#define MODE_PROD 2

//////////////////////////////////////////
// Structure definitions
//////////////////////////////////////////

// Structure holding the information about the game
typedef struct {
	int mode;
	int nb;
	int score;

	int nb_a, nb_b;
}game_t;

typedef struct {
	int nb_a, 
		nb_b;

	long result; // expected result
	long answer; // user's answer
}game_step_t;

//////////////////////////////////////////
// Global variables
//////////////////////////////////////////

game_t g_game;
int score;

//////////////////////////////////////////
// Game code
//////////////////////////////////////////

void usage(char** argv){
	printf ("Usage : %s [nb mode a b]\n", argv[0]);
	printf ("   nb : number of figures that will be asked. Default 10.\n");
	printf ("   mode : game mode. Can be \"square\", \"cube\", \"product\". Default to square\n");
	printf ("   a : minimum value. Default 10.\n");
	printf ("   b : maximum value. Default 100.\n");
	
	printf ("\n");
	
	printf ("If the game mode is \"square\" or \"cube\", min and max indicate "\
			"the range of the values in which the number will be asked.\n");
	printf ("If the game mode is \"product\", the values indicate the number" \
		" of figures composing the 2 numbers. Ex: a=2 and b=3 mean that the product will be in the form (number with 2 figures)*(number with 3 figures)\n");
	
	printf ("\n");
	
	getchar();
}

void go_message(game_t* game){
	if (game->mode == MODE_SQUARE || game->mode == MODE_CUBE)
	{
		char *modes[]={"squares", "cubes"};
		printf("Let's find %d %s in [%d; %d]\n", game->nb, modes[game->mode], game->nb_a, game->nb_b);
	}
	else if (game->mode == MODE_PROD){
		printf ("Let's compute %d products of number with %d figures with numbers with %d figures",game->nb, game->nb_a, game->nb_b);
	}

	printf ("Press any key to start\n");
	getchar();
}

double average (double* times, int nb){
	double total = 0;
	for (int i = 0; i < nb; ++i){
		total += times[i];
	}
	return ((double)(total))/nb;
}

double variance (double* times, int nb, double avg){
	double total = 0;
	for (int i = 0; i < nb; ++i){
		double curr = ((double)times[i]) - avg;
		total += (curr*curr);
	}
	return ((double)(total))/nb;
}

double std_dev (double variance){
	return sqrt(variance);
}

void game_init(game_t* game, int argc, char**argv){
	// Default game : 10 squares between 10 and 100
	game->mode = MODE_SQUARE;
	game->nb = 10;
	
	if (argc >= 2)
		game->nb = atoi (argv[1]);
	if (argc >= 3){
		if (strcmp(argv[2], "square")==0)
			game->mode = MODE_SQUARE;
		else if (strcmp(argv[2], "cube") == 0)
			game->mode = MODE_CUBE;
		else if (strcmp(argv[2], "product") == 0)
			game->mode = MODE_PROD;
	}
	// Extracts the range
	if (argc >= 5)
	{
		game->nb_a = atoi (argv[3]);
		game->nb_b = atoi (argv[4]);
	}
	// Or provide with a default one
	else
	{
		if (game->mode == MODE_SQUARE || game->mode == MODE_CUBE)
		{
			game->nb_a = 10;
			game->nb_b = 100;
		}
		else
		{
			game->nb_a = 2;
			game->nb_b = 2;
		}
	}

	go_message (game);
}

int random_range (int min, int max){
	return (int)(min+rand ()%(max-min));
}

void game_generate_value (game_t* game, game_step_t* step){
	if (game->mode == MODE_SQUARE){
		step->nb_a = random_range(game->nb_a, game->nb_b);

		step->result = step->nb_a * step->nb_a;
		printf ("%d^2 ?", step->nb_a);
	}
	if (game->mode == MODE_CUBE){
		step->nb_a = random_range(game->nb_a, game->nb_b);

		step->result = step->nb_a * step->nb_a * step->nb_a;
		printf ("%d^3 ?", step->nb_a);
	}
	if (game->mode == MODE_PROD){
		int a_max = pow(10, game->nb_a),
			b_max = pow (10, game->nb_b);
		step->nb_a = random_range(a_max/10, a_max);
		step->nb_b = random_range(b_max/10, b_max);

		step->result = step->nb_a * step->nb_b;
		printf ("%d*%d ?", step->nb_a, step->nb_b);
	}
}

void game_ask_answer(game_step_t* step){
	printf ("\n\n > ");
	scanf ("%ld", &(step->answer));
}

void game_check_answer(game_t* game, game_step_t* step){
	if (step->answer == step->result)
		++score;
	else
		printf ("Faux ! La reponse est %ld\n", step->result);
}

void game_final_score (game_t* game, double* times, int score)
{
	printf ("%d figures in [%d; %d]\n", g_game.nb, g_game.nb_a, g_game.nb_b);
	printf ("Score : %d/%d good answers, \n", score, g_game.nb);
	double avg = average(times, g_game.nb);
	double var = variance(times, g_game.nb, avg);
	double sdev= std_dev (var); 
	printf ("Average answer time : %3.3fs\n", avg);
	printf ("Standard deviation : %3.3fs\n", sdev);
}

//////////////////////////////////////////
// Application entry point
//////////////////////////////////////////

int main (int argc, char**argv)
{
	srand(time(NULL));
	
	struct timeval t1, t2;

    long mtime, seconds, useconds;   

	game_step_t step;
	
	usage(argv);
	game_init(&g_game, argc, argv);

	double* times = (double*)malloc (g_game.nb*sizeof(double));

	for (int i = 0; i < g_game.nb; i++)
	{
		// time(&t1);    

    	gettimeofday(&t1, NULL);

		// Generate current number
		game_generate_value(&g_game, &step);

		// Get player's answer
		game_ask_answer(&step);

		// Show timer
		//time(&t2);
		gettimeofday(&t2, NULL);
		times[i] = 0;
		seconds  = t2.tv_sec  - t1.tv_sec;
    	useconds = t2.tv_usec - t1.tv_usec;

    	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

		times[i] = (double)(mtime * 0.001f);
		printf ("%3.3f secondes\n", (float)(times[i]));

		// Verify answer
		game_check_answer(&g_game, &step);
		
		printf ("Score : %d/%d, %d figures\n\n", score, i+1, g_game.nb);
	}

	printf ("\n\n");
	game_final_score(&g_game, times, score);

	return EXIT_SUCCESS;
}