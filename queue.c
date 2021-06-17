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
struct Queue *createQueue(unsigned capacity)
{
    struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    queue->rear = capacity - 1;
    queue->array = (int *)malloc(queue->capacity * sizeof(double));
    return queue;
};

int isFullQ(struct Queue *queue)
{
    return (queue->size == queue->capacity);
}
int isEmptyQ(struct Queue *queue)
{
    return (queue->size == 0);
}

void pushQ(struct Queue *queue, double item)
{
    if (isFullQ(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}
int popQ(struct Queue *queue)
{
    if (isEmptyQ(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}