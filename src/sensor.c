#include <getopt.h>
#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <syslog.h>
#include <signal.h>
#include "../include/common.h"
#define MAXLINE 1024
void chance_state();
void print_help(char *command)
{

    printf("Sensor\n");
    printf("Use:\n %s <hostname> <puerto>", command);
    printf(" %s -h\n", command);
    printf("Opciones:\n");
    printf(" -h\t\t\tHelp, show this message\n");
    printf(" -t\t\t\tSet the time to send information");
}

volatile terminar;
volatile pausar;
pthread_mutex_t mutex2;

sem_t mutex;
int main(int argc, char **argv)
{

    // pthread_mutex_init(&mutex, 0);
    openlog("Sensor Rover", LOG_PID, LOG_USER);
    sem_init(&mutex, 0, 1);
    terminar = false;
    pausar = false;
    pid_t t_id;
    pthread_create(&t_id, NULL, chance_state, NULL);
    int opt, time_wait;
    int clientfd;
    char *hostname, *port, *opcodes;

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


    clock_t start_time;
    start_time = clock();
    double time_spend;

    size_t n, n2;
    time_spend = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    char read_buffer[MAXLINE + 1] = {0};

    while (!terminar)
    {
        if (!pausar)
        {
            char s_timestamp[1020];
            char s_num[1020];
            unsigned int timestamp = (unsigned int)time(NULL);
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

            sleep((int)(time_wait/1000));
            if (n <= 0)
            {
                printf("Error"); //cambiar cuando ya esté en producción
                break;
            }
            time_spend = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        }
    }

    return 0;
}

void chance_state()
{
    pthread_detach(pthread_self());
    do
    {
        // fflush(stdout);
        // fputs("\033c", stdout);
        // pthread_mutex_lock(&mutex2);
        
        printf("\n");
        printf("Si desea cambiar el estado del sensor digite:\n");
        printf("p: pausa\n");
        printf("r: reactivarn\n");
        printf("t: terminar\n");
        printf("Ingrese la opción que requiera: ");
        printf("\n");
        char op;

        scanf(" %c", &op);

        switch (op)
        {
        case 'p':
            syslog(LOG_INFO, "El envio de la información se ha colocado en pausa\n");
            // sem_wait(&mutex);
            pausar = true;
            // sem_post(&mutex);
            break;
        case 'r':
            // sem_wait(&mutex);
            syslog(LOG_INFO, "Se ha reactivado el envio de la información\n");
            pausar = false;
            // sem_post(&mutex);
            break;
        case 't':
            syslog(LOG_INFO, "Finaliza el sensor\n");
            terminar = true;
            exit(0);
            break;
        default:
            break;
        }
        // pthread_mutex_unlock(&mutex2);

    } while (!terminar);
}
