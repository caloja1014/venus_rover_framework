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
};
struct OpProcesor *op_procesor_create(int id_op, double freq, int x_minutes);
double process_information(struct OpProcesor *opprocesor);
void add_information(struct OpProcesor *opprocesor, double data, double timestamp);