#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void usage(int nb){
	printf ("\nC'est parti pour %d carres\n\n", nb);
	printf ("Appuyez sur n'importe quelle touche pour demarrer\n");
	getchar();
}

double average (time_t* times, int nb){
	time_t total = 0;
	for (int i = 0; i < nb; ++i){
		total += times[i];
	}
	return ((double)(total))/nb;

}

int main (int argc, char**argv){
	srand(time(NULL));
	int NB = 5;
	int curr;
	int score = 0;
	time_t t1, t2;
	int min = 10, max = 100;
	if (argc >= 2)
		NB = atoi (argv[1]);
	if (argc >= 4)
	{
		min = atoi (argv[2]);
		max = atoi (argv[3]);
	}
	usage (NB);
	time_t* times = (time_t*)malloc (NB*sizeof(time_t));

	for (int i = 0; i < NB; i++)
	{
		time(&t1);

		curr = (int)(min+rand ()%(max-min));
		int res;
		
		printf ("%d^2 ?\n\n > ", curr);
		scanf ("%d", &res);
		time(&t2);
		times[i] = t2-t1;
		printf ("%3.3f secondes\n", (float)(times[i]));

		if (res == (curr*curr))
			++score;
		else
			printf ("Faux ! La reponse est %d\n", curr*curr);
		printf ("Score : %d/%d, %d nombres\n\n", score, i+1, NB);
	}

	printf ("\n\n");
	printf ("%d nombres dans [%d; %d]\n", NB, min, max);
	printf ("Temps moyen : %f\n", average(times, NB));

	return EXIT_SUCCESS;
}