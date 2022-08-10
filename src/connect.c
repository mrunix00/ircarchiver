#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./include/connect.h"
							
/*
 * Connect to the IRC Server
*/
int
InitConnection(int *fd, char *ip, char *port){
	struct addrinfo hints,*res;
        
        memset(&hints,0,sizeof(hints));
        hints.ai_family=AF_INET;
        hints.ai_socktype=SOCK_STREAM;
        hints.ai_flags=AI_PASSIVE;

        getaddrinfo(ip,"6667",&hints,&res);
        *fd = socket(res->ai_family,res->ai_socktype,
			res->ai_protocol);
        int err = connect(*fd,res->ai_addr,
			res->ai_addrlen);

        return err;
}

/*
 * Login in into the server
*/								
int
login(int fd, char *nick){
	char s[100];
	sprintf(s,"NICK %s\nUSER %s 0 * :realname\n",nick,nick);
	int err=write(fd,s,sizeof(s));

	return err;
}

/*
 * Join an IRC channel
*/
int
join(int fd, char *channel){
	char s[40];
	sprintf(s,"\rJOIN :#%s\r\n",channel);
	int err=write(fd,s,strlen(s));

	return err;
}

int
pong(int fd, char *token){
	char s[40];
	sprintf(s,"\rPONG :%s\r\n",token);
	int err=write(fd,s,strlen(s));

	return err;
}
