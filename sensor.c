#include <getopt.h>
#include "common.h"
#include <limits.h>
#include <time.h>
#define MAXLINE 1024
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

    int opt, time_wait;
    int clientfd;
    char *hostname, *port, *opcodes;

    int signal_recv;
    while ((opt = getopt(argc, argv, "ht:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help(argv[0]);
            break;
        case 't':
            time_wait = atoi(optarg);
            break;
        case '?':
        default:
            fprintf(stderr, "Use:\n %s <hostname> <puerto> <opcode1,opcode2,...>", argv[0]);
            fprintf(stderr, "\t\t\t\t%s -h\n", argv[0]);
            break;
        }
    }

    int contador = 0;
    int index = 0;
    for (index = optind; index < argc; index++)
        contador++;

    if (contador != 3)
    {
        fprintf(stderr, "uso: %s <hostname> <puerto> <[opcode1,opcode2,...]>\n", argv[0]);
        fprintf(stderr, "     %s -h\n", argv[0]);
        return 1;
    }
    else
    {
        hostname = argv[optind];
        port = argv[optind + 1];
        opcodes = argv[optind + 2];
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

    // char dato_prueba[] = "[2,4,9],99,1095379198.75";
    size_t n, n2;
    time_spend = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    char read_buffer[MAXLINE + 1] = {0};
    //printf("%d\n",time_spend);
    while (time_spend < time_wait)
    {
        char *s_timestamp;
        char *s_num;
        unsigned long timestamp = (unsigned long)time(NULL);
        int num_random = rand() % 256;
        sprintf(s_timestamp, "%d", timestamp);
        sprintf(s_num, "%d", num_random);

        char dato_send[MAXLINE];
        memset(dato_send, 0, MAXLINE);
        strcat(dato_send, opcodes);
        strcat(dato_send, ",");
        strcat(dato_send, s_num);
        strcat(dato_send, ",");
        strcat(dato_send, s_timestamp);
        size_t l = sizeof(dato_send);
        n2 = recv(clientfd, read_buffer, sizeof(int), MSG_DONTWAIT);
        n = write(clientfd, dato_send, l);

        printf("%d  %s  %s\n", n, dato_send, read_buffer);

        sleep(5);
        if (n <= 0)
        {
            printf("Error"); //cambiar cuando ya esté en producción
            break;
        }
        time_spend = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    }
}
