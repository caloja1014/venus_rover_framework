#ifndef OPCODEBUFFER
#define OPCODEBUFFER

#include "queue.h"
#define MAX_SIZE_BUFFER 20
struct OpCodeBuffer
{
    struct Queue *data;
    struct Queue *times;
    int id_op;
};

struct OpCodeBuffer *op_code_buffer_create(int id_op);
void pop_information(struct OpCodeBuffer *op_cb);
void pop_and_set(struct OpCodeBuffer *op_cb, double *data, double *time_s);
void push_information(struct OpCodeBuffer *op_cb, double data, double time);
int isEmpty(struct OpCodeBuffer *op_cb);
void clean_queue(struct OpCodeBuffer *op_cb);

#endif /* OPCODEBUFFER */