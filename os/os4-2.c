#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int originArray[] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int arr_size = sizeof(originArray) / sizeof(int);
int *ansArray;
void *sorter(void *);
void *merger(void *);
typedef struct
{
    int start;
    int end;
} parameters;

int main()
{
    // printf("arr_size: %d\n", arr_size);
    pthread_t leftThread; /* the thread identifier */
    pthread_t rightThread;
    pthread_t mergeThread;
    pthread_attr_t attr; /* set of attributes for the thread */
    /* get the default attributes */
    pthread_attr_init(&attr);

    parameters *dataleft = (parameters *)malloc(sizeof(parameters));
    dataleft->start = 0;
    dataleft->end = (arr_size / 2) - 1; // 0~4
    // printf("dataleft: %d,%d\n", dataleft->start, dataleft->end);

    parameters *dataright = (parameters *)malloc(sizeof(parameters));
    dataright->start = (arr_size / 2);
    dataright->end = arr_size - 1; // 5~9
    // printf("dataright: %d,%d\n", dataright->start, dataright->end);

    /* create the thread */
    pthread_create(&leftThread, &attr, sorter, dataleft);
    pthread_create(&rightThread, &attr, sorter, dataright);

    /* now wait for the thread to exit */
    pthread_join(leftThread, NULL);
    pthread_join(rightThread, NULL);

    // for (int i = 0; i < arr_size - 1; ++i)
    //{
    //     printf("%d, ", originArray[i]);
    // }
    // printf("%d\n", originArray[arr_size - 1]);

    ansArray = (int *)malloc(sizeof(int) * arr_size);
    pthread_create(&mergeThread, &attr, merger, dataleft); //不用參數?
    pthread_join(mergeThread, NULL);
    for (int i = 0; i < arr_size - 1; ++i)
    {
        printf("%d, ", ansArray[i]);
    }
    printf("%d\n", ansArray[arr_size - 1]);
    free(dataleft);
    free(dataright);
    free(ansArray);
    return 0;
}

void *sorter(void *param)
{
    parameters *data = param;
    int tmp;
    for (int i = data->end; i >= data->start; --i)
    {
        for (int j = data->start; j < i; ++j)
        {
            if (originArray[j] > originArray[j + 1])
            {
                tmp = originArray[j];
                originArray[j] = originArray[j + 1];
                originArray[j + 1] = tmp;
            }
        }
    }
    pthread_exit(0);
}

void *merger(void *param)
{
    int current = 0;
    parameters *data = param;
    int leftFirst = data->start;
    int leftEnd = data->end;
    int rightFirst = leftEnd + 1;
    int rightEnd = arr_size - 1;
    // printf("%d,%d,%d,%d\n", leftFirst, leftEnd, rightFirst, rightEnd);
    while (leftFirst <= leftEnd && rightFirst <= rightEnd)
    {
        if (originArray[leftFirst] < originArray[rightFirst])
        {
            ansArray[current] = originArray[leftFirst];
            leftFirst += 1;
        }
        else
        {
            ansArray[current] = originArray[rightFirst];
            rightFirst += 1;
        }
        current += 1;
        // printf("left:%d, ori: %d\n", leftFirst, originArray[leftFirst]);
        // printf("right:%d, ori: %d\n", rightFirst, originArray[rightFirst]);
    }
    while (leftFirst <= leftEnd)
    {
        // printf("left:%d, ori: %d\n", leftFirst, originArray[leftFirst]);
        ansArray[current] = originArray[leftFirst];
        leftFirst += 1;
        current += 1;
    }
    while (rightFirst <= rightEnd)
    {
        // printf("right:%d, ori: %d\n", rightFirst, originArray[rightFirst]);
        ansArray[current] = originArray[rightFirst];
        rightFirst += 1;
        current += 1;
    }

    pthread_exit(0);
}
