#include <netdb.h>
#include <sys/socket.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int InitConnection(int *, char *, char *);
ssize_t login(int, char *);
ssize_t join(int, char *);
ssize_t pong(int, char *);
