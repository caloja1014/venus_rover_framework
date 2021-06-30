/**
 * Este test verifica que los sensores pueden enviar multiple información
 *
 * Se pueden cambiar los datos a conveniencia
 */
#include "../include/test.h"

int main(int argc, char const *argv[])
{
    char cmd_sensor[] = "/sensor 127.0.0.1 8080 [1,3,4] -t 1";
    execute_cmd(cmd_sensor);

    return 0;
}