/* Do not make any changes in the skeleton. Your submission will be invalidated if the skeleton is changed */
 
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<stdbool.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<semaphore.h>
 
 
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t mutex;

int N;
int startx, starty; //randomizer bound
int num_sq;
pthread_t tid[2];
 
 
 
void *solve();
 
int mov_x[] = {1, -1, 2, -2, 2, -2, 1, -1};
int mov_y[] = {2, 2, 1, 1, -1, -1, -2, -2};
 
// Helper struct, can utilize any other custom structs if needed 
typedef struct
{
	int x,y;
} pair;
 
 
// Not necessary to use this function as long as same printing pattern is followed 
void print_path(pair path[], int n)
{
	for(int i = 0; i < n; i++)
		printf("%d,%d|", path[i].x, path[i].y);
}
 
void createthread()
{
	sem_init(&mutex, 0, 1);
	for(int curr_t = 0; curr_t < 2; curr_t++)
	{
		pthread_create(&tid[curr_t], NULL, solve, NULL);
	}
}
 
int getScore(int n, int x, int y, bool board[50][50])
{
	int score = 0;
	if(x < 0 || y < 0 || x>=n || y>=n || board[y][x] == true)
	{
		return INT_MAX;
	}
 
	for(int i = 0; i < 8; i++)
	{
		if(x + mov_x[i] < 0 || y + mov_y[i] < 0 || x + mov_x[i] >= n || y + mov_y[i] >= n || board[y+mov_y[i]][x+mov_x[i]])
		{
			continue;
		}
		else
		{
			score++;
		}
	}
	return score;
}
 
void printsol(int arr[][2])
{
	for (int i = 0; i < num_sq; i++)
	{
		printf("%d,%d|", arr[i][0], arr[i][1]);
	}
}
 
void *solve()
{
	
	bool sol = true;
	num_sq = N*N;
	while(1)
	{
		sol = true;
		int start_x = startx;
		int start_y = starty;
		int path[num_sq][2];
		
 
		bool board[50][50];
		for (int i = 0; i < 50; ++i)
			for (int j = 0; j < 50; ++j)
				board[i][j] = false;
		
		int new_x, new_y;
		//path[0][0] = start_x, path[0][1] = start_y;
		
		for(int i = 1; i < num_sq; i++)
		{
			board[start_y][start_x] = true;
			int min_score = INT_MAX;
 
			int totalmin = 0;
			int cells[8][2];
			int x = 0;
			for(int j = 0; j < 8; j++)
			{
				if(getScore(N, start_x + mov_x[j], start_y + mov_y[j], board) < min_score)
				{
					new_x = start_x + mov_x[j];
					new_y = start_y + mov_y[j];
					
					min_score = getScore(N, start_x + mov_x[j], start_y + mov_y[j], board);
				}
			}
			if(min_score == INT_MAX)
			{
				sol = false;
				break;
			}
 
			for(int j = 0; j < 8; j++)
			{
				if(getScore(N, start_x + mov_x[j], start_y + mov_y[j], board) == min_score)
				{
					totalmin++;
					new_x = start_x + mov_x[j];
					new_y = start_y + mov_y[j];
					cells[x][0] = new_x;
					cells[x][1] = new_y;
					x++;
				}
			}
 
			//randomizer
			srand(time(0));
			int randd;
			if(totalmin != 0)
			{
				randd = rand() % totalmin;
			}
			else
			{
				sol = false;
				break;
			}
 
			static int a = 0;
 
			// printf("on run %d: x = %d, y = %d\n", a++, start_x, start_y);
			path[i - 1][0] = start_x, path[i - 1][1] = start_y;
			start_x = cells[randd][0];
			start_y = cells[randd][1];
		}
 
		path[num_sq - 1][0] = start_x;
		path[num_sq - 1][1] = start_y;
		
		if(!sol)
		{
			continue;
		}
 
		//pthread_mutex_lock(&lock);
		sem_wait(&mutex);
		// for (int i = 0; i < 2; i++)
		// {
		// 	if(tid[i] != pthread_self())
		// 	{
		// 		pthread_cancel(tid[i]);
		// 	}
		// }
		printsol(path);
		printf("\n");
		// sem_destroy(&mutex);
		exit(0);
	}
}
 
int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("Usage: ./Knight.out grid_size StartX StartY");
		exit(-1);
	}
		
	N = atoi(argv[1]);
	startx = atoi(argv[2]);
	starty = atoi(argv[3]);
		
	if(N == 2 || N == 3 || N == 4)
	{
		printf("No Possible Tour");
		return 0;
	}
	if(N % 2 == 1)
	{
		if((startx + starty) % 2 == 1)
		{
			printf("No Possible Tour");
			return 0;
		}
	}
 
	createthread();
	
	for (int i = 0; i < 2; ++i)
	{
		pthread_join(tid[i], NULL);
	}
 
	return 0;
}
