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

	double answer_time; // required time to get answer
}game_step_t;

//////////////////////////////////////////
// Utilities
//////////////////////////////////////////

int random_range (int min, int max){
	return (int)(min+rand ()%(max-min));
}

double average (game_step_t* steps, int nb){
	double total = 0;
	for (int i = 0; i < nb; ++i){
		total += steps[i].answer_time;
	}
	return ((double)(total))/nb;
}

double variance (game_step_t* steps, int nb, double avg){
	double total = 0;
	for (int i = 0; i < nb; ++i){
		double curr = ((double)steps[i].answer_time) - avg;
		total += (curr*curr);
	}
	return ((double)(total))/nb;
}

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
}

// Small reminder of the rules before the game begins
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

void game_init(game_t* game, int argc, char**argv){
	// Default game : 10 squares between 10 and 100
	game->mode = MODE_SQUARE;
	game->nb = 10;
	
	srand(time(NULL));

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

void game_generate_value (game_t* game, game_step_t* step){
	if (game->mode == MODE_SQUARE){
		step->nb_a = random_range(game->nb_a, game->nb_b);

		step->result = step->nb_a * step->nb_a;
	}
	if (game->mode == MODE_CUBE){
		step->nb_a = random_range(game->nb_a, game->nb_b);

		step->result = step->nb_a * step->nb_a * step->nb_a;
	}
	if (game->mode == MODE_PROD){
		int a_max = pow(10, game->nb_a),
			b_max = pow (10, game->nb_b);
		step->nb_a = random_range(a_max/10, a_max);
		step->nb_b = random_range(b_max/10, b_max);

		step->result = step->nb_a * step->nb_b;
	}
}

char* question_message (game_t* game, game_step_t* step){
	char* mess = (char*)malloc(32*sizeof(char));

	if (game->mode == MODE_SQUARE){
		sprintf (mess, "%d^2 ?", step->nb_a);
	}
	if (game->mode == MODE_CUBE){
		sprintf (mess, "%d^3 ?", step->nb_a);
	}
	if (game->mode == MODE_PROD){
		sprintf (mess, "%d*%d ?", step->nb_a, step->nb_b);
	}

	return mess;
}

void game_display_question (game_t* game, game_step_t* step){
	char* m = question_message(game, step);
	printf("%s", m);
	free (m);
}

void game_ask_answer(game_step_t* step){
	printf ("\n\n > ");
	scanf ("%ld", &(step->answer));
}


void game_final_score (game_t* game, game_step_t* steps)
{
	int score = 0;
	int i;
	for (i = 0; i < game->nb; ++i)
		score += (steps[i].answer == steps[i].result);
	
	printf ("\n");
	printf ("%d figures in [%d; %d]\n", game->nb, game->nb_a, game->nb_b);
	for (i = 0; i < game->nb; i++){
		char* question = question_message (game, &steps[i]);
		printf("%s %c %ld\t%ld\t%3.3fs\n", 
			question, 
			steps[i].result == steps[i].answer ? ' ': 'x', 
			steps[i].result, 
			steps[i].answer, 
			steps[i].answer_time
			);
		free(question);
	}

	printf ("%d/%d good answers, \n", score, game->nb);
	double avg = average(steps, game->nb);
	double var = variance(steps, game->nb, avg);
	double sdev= sqrt (var); 
	printf ("Average answer time : %3.3fs\n", avg);
	printf ("Standard deviation : %3.3fs\n", sdev);
}


double compute_time_ms(struct timeval t1, struct timeval t2){
	long mtime, seconds, useconds;  
	seconds  = t2.tv_sec  - t1.tv_sec;
	useconds = t2.tv_usec - t1.tv_usec;

	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	return (double)(mtime * 0.001f);
}

//////////////////////////////////////////
// Application entry point
//////////////////////////////////////////

int main (int argc, char**argv)
{
	game_t g_game;
	struct timeval t1, t2;
	int i;
	usage(argv);
	game_init(&g_game, argc, argv);

	game_step_t* steps = (game_step_t*)malloc (g_game.nb*sizeof(game_step_t));
	
	for (i = 0; i < g_game.nb; i++)
		game_generate_value(&g_game, &steps[i]);

	for (i = 0; i < g_game.nb; i++)
	{
    	gettimeofday(&t1, NULL);

		// Generate current number
		game_display_question(&g_game, &(steps[i]));

		// Get player's answer
		game_ask_answer(&(steps[i]));

		// Show timer
		gettimeofday(&t2, NULL);
		steps[i].answer_time = compute_time_ms (t1, t2);
	}
	game_final_score(&g_game, steps);

	return EXIT_SUCCESS;
}