#include <getopt.h>
#include <limits.h>
#include <sys/wait.h>
#include <pthread.h>
#include <syslog.h>
#include <fcntl.h>
#include <semaphore.h>
#include <syslog.h>

#include "../include/common.h"
#include "../include/opprocesor.h"
#include "../include/map.h"

#define MAXLINE 1024
#define ATTENDING 1
#define WAITING 0
void *atender_cliente(void *connfd);
void *verify_frequency(void *opprocesor);
void *verify_waiting_time(void *args);
struct arg_thread
{
    volatile *end_procesor;
    struct OpProcesor *opprocesor;
};

char **parse_comando(char *linea, char *delim)
{
    char *token;
    char *linea_copy;
    int i, num_tokens = 0;
    char **argv = NULL;
    char *saveptr = NULL;

    linea_copy = (char *)malloc(strlen(linea) + 1);
    strcpy(linea_copy, linea);

    /* Obtiene un conteo del número de argumentos */
    token = strtok_r(linea_copy, delim, &saveptr);
    /* recorre todos los tokens */
    while (token != NULL)
    {
        token = strtok_r(NULL, delim, &saveptr);
        num_tokens++;
    }
    free(linea_copy);

    /* Crea argv en el heap, extrae y copia los argumentos */

    if (num_tokens > 0)
    {

        /* Crea el arreglo argv */
        argv = (char **)malloc((num_tokens + 1) * sizeof(char **));
        /* obtiene el primer token */
        token = strtok_r(linea, delim, &saveptr);
        /* recorre todos los tokens */
        for (i = 0; i < num_tokens; i++)
        {
            argv[i] = (char *)malloc(strlen(token) + 1);

            strcpy(argv[i], token);
            token = strtok_r(NULL, delim, &saveptr);
        }
        argv[i] = NULL;
    }

    return argv;
}
void print_help(char *command)
{

    printf("Op Procesor\n");
    printf("Use:\n %s <puerto>", command);
    printf(" %s -h\n", command);
    printf("Opciones:\n");
    printf(" -h\t\t\tHelp, show this message\n");
    printf(" -d\t\t\tRun in modo-debug in other case it will run in modo-realtime");
    printf(" -f\t\t\tSet the frecuency of the opprocesors");
    printf(" -x\t\t\tSet the maximun (in minutes)tim to wait");
}
void recoger_hijos(int signal)
{
    while (waitpid(-1, 0, WNOHANG) > 0)
        ;

    return;
}

int dflag = 0;
double freq;
double x_minutes;
sem_t mutex;
struct MapCustom *operations_map;
int main(int argc, const char **argv)
{
    operations_map = create_map();
    openlog("Framework Rover", LOG_PID, LOG_USER);
    atexit(closelog);
    sem_init(&mutex, 0, 1);
    int opt, index;
    char *eptr;
    int listenfd, *connfd;
    unsigned int clientlen;

    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp, *port;

    while ((opt = getopt(argc, argv, "hdf:x:")) != -1)
    {
        switch (opt)
        {
        case 'h':

            print_help(argv[0]);
            return 0;
        case 'd':
            dflag = 1;
            break;
        case 'f':
            freq = strtod(optarg, &eptr);
            break;
        case 'x':
            x_minutes = strtod(optarg, &eptr);
            break;
        default:
            fprintf(stderr, "uso: %s <puerto>\n", argv[0]);
            fprintf(stderr, "     %s -h\n", argv[0]);
            fprintf(stderr, "     %s -f\n", argv[0]);
            fprintf(stderr, "     %s -d\n", argv[0]);
            return -1;
        }
    }

    for (index = optind; index < argc; index++)
        port = argv[index];
    if (argv == NULL)
    {
        fprintf(stderr, "uso: %s <puerto>\n", argv[0]);
        fprintf(stderr, "     %s -h\n", argv[0]);
        fprintf(stderr, "     %s -f\n", argv[0]);
        fprintf(stderr, "     %s -d\n", argv[0]);
        return 1;
    }
    printf("%d  %d\n", freq, x_minutes);

    int port_n = atoi(port);
    if (port_n <= 0 || port_n > USHRT_MAX)
    {
        fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
        return 1;
    }

    signal(SIGCHLD, recoger_hijos);

    listenfd = open_listenfd(port);

    if (listenfd < 0)
        connection_error(listenfd);

    pthread_t t_id;

    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = malloc(sizeof(int));
        *connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        pthread_create(&t_id, NULL, atender_cliente, connfd);
    }

    return 0;
}

void *atender_cliente(void *connfd)
{

    pthread_detach(pthread_self());
    int fd_conn = *((int *)connfd);

    int n, status;
    char buf[MAXLINE] = {0};
    struct OpProcesor *opprocesor;
    bool flag = true;

    while (1)
    {

        n = read(fd_conn, buf, MAXLINE);

        if (n <= 0)
            return;

        char **sep_data = parse_comando(buf, "]");
        char *opers = sep_data[0] + 1;
        char **opers_sep = parse_comando(opers, ",");
        char **values_split = parse_comando(sep_data[1], ",");

        char *eptr;
        double data_sensor = strtod(values_split[0], &eptr);
        double time_sensor = strtod(values_split[1], &eptr);

        char *actual_operation;
        int indice = 0;
        while ((actual_operation = opers_sep[indice++]) != NULL)
        {
            pthread_t sub_t_id, sub_t_id2;
            int id_op = atoi(actual_operation);
            if (!has_key(operations_map, id_op))
            {
                sem_wait(&mutex);

                opprocesor = op_procesor_create(id_op, freq, (int)x_minutes, dflag);

                put_value(operations_map, id_op, opprocesor);

                if(dflag)
                {
                    char log_message[MAXLINE];
                    sprintf(log_message, "Se ha creado correctamente el OpProcesor para la operación %d\n", id_op);
                    syslog(LOG_INFO, log_message);
                }
                else
                {
                    printf("Se ha creado correctamente el OpProcesor para la operación %d\n", id_op);
                }

                sem_post(&mutex);

                add_information(opprocesor, data_sensor, time_sensor);
                struct arg_thread *args = malloc(sizeof(struct arg_thread));
                args->opprocesor = opprocesor;
                args->end_procesor = false;
                pthread_create(&sub_t_id2, NULL, verify_waiting_time, args);
                pthread_create(&sub_t_id, NULL, verify_frequency, args);
            }
            else
            {
                opprocesor = get_value_of_map(operations_map, id_op);
                printf("Agregando...\n");
                add_information(opprocesor, data_sensor, time_sensor);
            }
            write(fd_conn, "1", 2);
            // printf("tid=%d   %f  %f\n", pthread_self(), data_sensor, time_sensor);
            memset(buf, 0, MAXLINE);
        }
    }
    close(fd_conn);
    return NULL;
}

void *verify_frequency(void *args)
{
    pthread_detach(pthread_self());
    struct arg_thread *arg = (struct arg_thread *)args;
    struct OpProcesor *op = arg->opprocesor;
    volatile end_loop;

    while (!(end_loop = arg->end_procesor))
    {
        double now_time = (double)time(NULL);
        double result = now_time - (op->init_time);
        double div = result / 60;
        do
        {
            sleep((int)(freq * 60));
            now_time = (double)time(NULL);
            result = now_time - (op->init_time);
            div = result / 60;

            //hacer con nanosleep
        } while ((div < op->freq));

        // sem_wait(&mutex);

        double val = process_information(op);
        sem_post(&mutex);
        if (dflag)
        {
            //syslogs
            if (val == -1)
            {
                // syslog(LOG_INFO, "No existen datos para realizar el cálculo\n");
            }
            else
            {
                char log_message[MAXLINE];
                sprintf(log_message, "El promedio de los datos de la operación %d es: %f\n", op->id_op, val);
                syslog(LOG_INFO, log_message);
            }
        }
        else
        {
            if (val == -1)
            {
                // printf("No existen datos para realizar el cálculo\n");
            }
            else
            {
                printf("El promedio de los datos de la operación %d es: %f\n", op->id_op, val);
            }
        }

        // printf("valor es: %f de la thread %d\n", val, pthread_self());
        op->init_time = (double)time(NULL);
    }
}

void *verify_waiting_time(void *args)
{
    pthread_detach(pthread_self());
    struct arg_thread *arg = (struct arg_thread *)args;
    struct OpProcesor *op = arg->opprocesor;
    double now_time = (double)time(NULL);
    double result = (now_time - (op->last_time_pushed)) / 60;
    int wait_time = op->x_minutes;

    while (result < wait_time)
    {
        now_time = (double)time(NULL);
        result = (now_time - (op->last_time_pushed)) / 60;
    }

    double val = process_information_and_verify(op);

    if (dflag)
    {
        //syslog
        char log_message[MAXLINE];
        sprintf(log_message, "La operación %d se ha detenido dado que ha dejado de recibir elementos por %d minutos apróximadamente\n", op->id_op, wait_time);
        syslog(LOG_INFO, log_message);
    }
    else
    {
        printf("La operación %d se ha detenido dado que ha dejado de recibir elementos por %d minutos apróximadamente\n", op->id_op, wait_time);
    }
    arg->end_procesor = true;

    // sem_wait(&mutex);
    delete_key(operations_map, arg->opprocesor->id_op);
    sem_post(&mutex);
}