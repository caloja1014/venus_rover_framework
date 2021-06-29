#ifndef opprocesor.h
#include "../include/opprocesor.h"
#endif
struct OpProcesor *op_procesor_create(int id_op, double freq, int x_minutes,bool is_debug)
{
    struct OpProcesor *opprocesor = malloc(sizeof(struct OpProcesor));
    opprocesor->id_op = id_op;
    opprocesor->freq = freq;
    opprocesor->x_minutes = x_minutes;
    opprocesor->buffer = op_code_buffer_create(1);
    opprocesor->init_time = (double)time(NULL);
    opprocesor->is_debug=is_debug;

    return opprocesor;
}
bool flag = true;
double process_information(struct OpProcesor *opprocesor)
{
    flag = false;
    int capacity = opprocesor->buffer->data->size;
    double total = 0.0;
    double final_time = (double)time(NULL);
    while (!isEmpty(opprocesor->buffer))
    {

        double actual_data = 0;
        double actual_time = 0;
        pop_and_set(opprocesor->buffer, &actual_data, &actual_time);
        double result = (final_time - actual_time)/60;
        double comp = ((double)(opprocesor->x_minutes)) / 20;
        // printf("Result: %f comp: %f  data: %f  time: %f  final time: %f    capacidad: %d  %f\n",result,comp,actual_data,actual_time,final_time,capacity,total);
        if (result < comp)
        {
            
            total += actual_data;
        }
    }
    clean_queue(opprocesor->buffer);
    flag = true;
    
    return capacity!=0? total / capacity:-1;
}

void *add_information(struct OpProcesor *opprocesor, double data, double timestamp)
{
    if (flag)
    {
        push_information(opprocesor->buffer, data, timestamp);
    }
    opprocesor->last_time_pushed = (double)time(NULL);
}
void clean_buffer()
{
}