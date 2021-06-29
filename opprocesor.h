#ifndef OPPROCESOR_H
#define OPPROCESOR_H
#include "op_code_buffer.h"
#include <time.h>
struct OpProcesor
{
    struct OpCodeBuffer *buffer;
    int id_op;
    double freq;
    int x_minutes;
    double init_time;
    double last_time_pushed;
    bool is_debug;
};
struct OpProcesor *op_procesor_create(int id_op, double freq, int x_minutes,bool is_debug);
double process_information(struct OpProcesor *opprocesor);
void *add_information(struct OpProcesor *opprocesor, double data, double timestamp);

#endif /*OPPROCESOR_H */