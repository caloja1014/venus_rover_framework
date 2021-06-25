#include <getopt.h>
#include "common.h"
#include <limits.h>
#include <time.h>
void print_help(char *command)
{

    printf("Sensor\n");
    printf("Use:\n %s <hostname> <puerto>", command);
    printf(" %s -h\n", command);
    printf("Opciones:\n");
    printf(" -h\t\t\tHelp, show this message\n");
    printf(" -t\t\t\tSet the time to send information");
}

int main(int argc, char **argv)
{

    int opt, time;
    int clientfd;
    char *hostname, *port;

    int signal_recv;
    while ((opt = getopt(argc, argv, "ht:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help(argv[0]);
            break;
        case 't':
            time = atoi(optarg);
            break;
        case '?':
        default:
            fprintf(stderr, "Use:\n %s <hostname> <puerto>", argv[0]);
            fprintf(stderr, "\t\t\t\t%s -h\n", argv[0]);
            break;
        }
    }

    int contador = 0;
    int index = 0;
    for (index = optind; index < argc; index++)
        contador++;

    if (contador != 2)
    {
        fprintf(stderr, "uso: %s <hostname> <puerto>\n", argv[0]);
        fprintf(stderr, "     %s -h\n", argv[0]);
        return 1;
    }
    else
    {
        hostname = argv[optind];
        port = argv[optind + 1];
    }

    int port_n = atoi(port);
    if (port_n <= 0 || port_n > USHRT_MAX)
    {
        fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
        return -1;
    }

    clientfd = open_clientfd(hostname, port);
    if (clientfd < 0)
    {
        connection_error(clientfd);
    }
    printf("Connected on %s in port %s succesfully.\n", hostname, port);

    clock_t start_time, end_time;
    start_time = clock();
    double time_spend;

    char dato_prueba[] = "[2,4,9],99,1095379198.75";
    size_t n, l = sizeof(dato_prueba);
    time_spend = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    //printf("%d\n",time_spend);
    while (time_spend < time)
    {
        n = write(clientfd, dato_prueba, l);
        printf("%d\n", n);
        sleep(5);
            if (n <= 0)
        {
            printf("Error"); //cambiar cuando ya esté en producción
            break;
        }
        time_spend = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    }
}
