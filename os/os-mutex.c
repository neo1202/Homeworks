#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INTERVAL 1000
#define NTHREADS 5
int sum;
int circle_points = 0, square_points = 0; /* this data is shared by the thread(s) */
void *runner(void *param);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
    pthread_t thread_id[NTHREADS];
    double pi;

    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_create(&thread_id[i], NULL, runner, NULL);
    }
    for (int i = 0; i < NTHREADS; i++)
    {
        pthread_join(thread_id[i], NULL);
    }

    pi = (double)(4 * circle_points) / square_points;
    printf("circle:%d, square:%d\n", circle_points, square_points);
    printf("Final Estimation of Pi = %f\n", pi);
}

void *runner(void *param)
{
    int interval, i;
    double rand_x, rand_y, origin_dist, pi;
    srand(time(NULL));

    for (i = 0; i < (INTERVAL * INTERVAL); i++)
    {

        // Randomly generated x and y values
        rand_x = (double)(rand() % (INTERVAL + 1)) / INTERVAL;
        rand_y = (double)(rand() % (INTERVAL + 1)) / INTERVAL;
        origin_dist = rand_x * rand_x + rand_y * rand_y;
        pthread_mutex_lock(&mutex1); //上鎖
        if (origin_dist <= 1)
            circle_points++;
        square_points++;
        pthread_mutex_unlock(&mutex1);
    }
    pthread_exit(0);
}
