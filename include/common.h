#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/inet.h>


#define WAIT_FOR_MORESPACE 1
#define MESSAGE_RECEIVED 0

int open_listenfd(char *port);


int open_clientfd(char *hostname, char *port);


void connection_error(int connfd);

#endif