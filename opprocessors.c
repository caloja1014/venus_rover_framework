#include <getopt.h>
#include <limits.h>
#include <sys/wait.h>

#include <syslog.h>
#include <fcntl.h>
#include "threadpool.h"
#include "common.h"
#define MAXLINE 1024
void *atender_cliente(void *connfd);

void print_help(char *command)
{

    printf("Op Procesor\n");
    printf("Use:\n %s <puerto>", command);
    printf(" %s -h\n", command);
    printf("Opciones:\n");
    printf(" -h\t\t\tHelp, show this message\n");
    printf(" -d\t\t\tRun in modo-debug in other case it will run in modo-realtime");
}
int dflag;
int main(int argc, char  **argv)
{
    int opt, index;

    //Sockets
    int listenfd, *connfd;
    unsigned int clientlen;
    //Direcciones y puertos
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp, *port;

    while ((opt = getopt(argc, argv, "hd")) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help(argv[0]);
            return 0;
        case 'd':
            dflag = 1;
            break;
        default:
            fprintf(stderr, "uso: %s <puerto>\n", argv[0]);
            fprintf(stderr, "     %s -h\n", argv[0]);
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
        fprintf(stderr, "     %s -d\n", argv[0]);
        return 1;
    }

    int port_n = atoi(port);
    if (port_n <= 0 || port_n > USHRT_MAX)
    {
        fprintf(stderr, "Puerto: %s invalido. Ingrese un número entre 1 y %d.\n", port, USHRT_MAX);
        return 1;
    }
    listenfd = open_listenfd(port);
    if (listenfd < 0)
        connection_error(listenfd);

    tpool_t *tm;
    size_t num_threads = pcthread_get_num_procs();
    tm = tpool_create(3);

    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = malloc(sizeof(int));
        *connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        tpool_add_work(tm, atender_cliente, connfd);
    }

    tpool_wait(tm);

    tpool_destroy(tm);
    //printf("%d\n",pcthread_get_num_procs());
    return 0;
}

void *atender_cliente(void *connfd)
{

    int fd_conn = *((int *)connfd);
    int n, status;
    char buf[MAXLINE] = {0};
    
    while (1)
    {
        
        n = read(fd_conn, buf, MAXLINE);
        if (n <= 0)
            return;

        printf("tid=%d\n",pthread_self());
        memset(buf, 0, MAXLINE);
    }
}