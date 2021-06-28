#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    double *array;
};
struct Queue *createQueue(unsigned capacity);

int isFullQ(struct Queue *queue);
void pushQ(struct Queue *queue, double item);
double popQ(struct Queue *queue);