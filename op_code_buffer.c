#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_SIZE_BUFFER 20
struct op_code_buffer
{
    double data[MAX_SIZE_BUFFER];
    double times[MAX_SIZE_BUFFER];
    int actual_size, id_op;
    
};

struct op_code_buffer *op_code_buffer_create(int id_op){
    struct op_code_buffer *op_cb=malloc(sizeof(struct op_code_buffer));
    op_cb->actual_size=0;
    op_cb->id_op=id_op;
    return op_cb;
}

