/**
 * Este test verifica que el framework funciona con multiples sensores
 * El framework se ejecutará en el puerto 8080, realizará las operaciones cada 1 minuto y esperará máximo 2 minutos
 * Se pueden cambiar los datos a conveniencia
 */
#include "../include/test.h"

int main(int argc, char const *argv[])
{
    char cmd_framework[] = "/framework 8080 -f 0.1 -x 1";
    // char cmd_sensor[] = "/sensor 127.0.0.1 8080 [1,3,4] -t 1";
    execute_cmd(cmd_framework);

    return 0;
}
