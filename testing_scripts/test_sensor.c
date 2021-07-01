/**
 * Este test verifica que los sensores pueden enviar multiple información
 *
 * Se pueden cambiar los datos a conveniencia
 */
#include "../include/test.h"

int main(int argc, char const *argv[])
{
    char cmd_sensor[] = "/sensor 127.0.0.1 8080 [1,2,3,4,5,12,14,132,23,342,2344,234,23467,543,643] -t 5000";
    execute_cmd(cmd_sensor);

    return 0;
}