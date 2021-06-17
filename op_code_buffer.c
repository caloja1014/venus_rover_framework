#include <queue.c>

#define MAX_SIZE_BUFFER 20
struct op_code_buffer
{
    struct Queue *data;
    struct Queue *times;
    int id_op;
};

struct op_code_buffer *op_code_buffer_create(int id_op)
{
    struct op_code_buffer *op_cb = malloc(sizeof(struct op_code_buffer));
    op_cb->data=createQueue(MAX_SIZE_BUFFER);
    op_cb->times=createQueue(MAX_SIZE_BUFFER);
    op_cb->id_op = id_op;
    return op_cb;
}
void pop_information(struct op_code_buffer* op_cb){
    popQ(op_cb->data);
    popQ(op_cb->times);
}
void push_information(struct op_code_buffer* op_cb,double data,double time){
    if (isFullQ(op_cb->times)){
        pop_information(op_cb);
    }
    pushQ(op_cb->data,data);
    pushQ(op_cb->times,time);
}

void isEmpty(struct op_code_buffer* op_cb){
    return isEmptyQ(op_cb->data);
}
