#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INTERVAL 1000

int sum;
int circle_points = 0, square_points = 0; /* this data is shared by the thread(s) */
void *runner(void *param);                /* the thread */

int main(int argc, char *argv[])
{
    pthread_t tid;       /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    double pi;
    /* get the default attributes */
    pthread_attr_init(&attr);
    /* create the thread */
    pthread_create(&tid, &attr, runner, argv[1]);
    /* now wait for the thread to exit */
    pthread_join(tid, NULL);
    // estimated pi after this iteration
    pi = (double)(4 * circle_points) / square_points;
    // Final Estimated Value
    // cout << "\nFinal Estimation of Pi = " << pi;
    printf("circle:%d, square:%d\n", circle_points, square_points);
    printf("Final Estimation of Pi = %f\n", pi);
}

void *runner(void *param)
{
    int interval, i;
    double rand_x, rand_y, origin_dist, pi;
    srand(time(NULL));
    // Total Random numbers generated = possible x
    // values * possible y values
    for (i = 0; i < (INTERVAL * INTERVAL); i++)
    {

        // Randomly generated x and y values
        rand_x = (double)(rand() % (INTERVAL + 1)) / INTERVAL;
        rand_y = (double)(rand() % (INTERVAL + 1)) / INTERVAL;
        origin_dist = rand_x * rand_x + rand_y * rand_y;
        if (origin_dist <= 1)
            circle_points++;
        square_points++;
    }
    pthread_exit(0);
}
