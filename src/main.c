#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./include/connect.h"
#include "./include/irc-utils.h"

#define maxPacketSize 512

void usage(){
        printf("usage:\tircarchiver [-h host] [-p port] [-n nick]\n\t[-o file] channel\n");
}

/* This is an IRC bot that saves IRC conversations */
int
main(int argc, char *argv[]) {
	int err = 0, fd = 0;
	bool loggedin=false;
	char *ip, *port, *nick, *flocation;

	ip = strdup("localhost");
	port = strdup("6667");
	nick = strdup("bender");
	flocation = strdup("./archive.txt");
								
	int ch = 0;
	while ((ch = getopt(argc, argv, "h:p:n:o:")) != -1 ) {
		switch (ch) {
			case 'h':
				if(strlen(optarg)>100){
					usage();
					return EXIT_FAILURE;
				}
				else ip=strdup(optarg);
				break;
			case 'p':
				for(int i=0; i<strlen(optarg); i++){
					if(!isdigit(optarg[i])){
						printf("Invalid Port Number\n");
						return EXIT_FAILURE;
					}
				}
				if(atoi(optarg) > 65535){
					printf("Port number should be 0-65535\n");
					return EXIT_FAILURE;
				}
				port=strdup(optarg);
				break;
			case 'n':
				if(sizeof(optarg) >= 31){
					printf("Nickname should not be longer than 31 characters\n");
					return EXIT_FAILURE;
				}else if(isdigit(optarg[0])){
					printf("Nickname should not start with a digit\n");
					return EXIT_FAILURE;
				}
				nick=strdup(optarg);
				break;
			case 'o':
				if(sizeof(optarg)>300){
					printf("Output location is too long\n");
					return EXIT_FAILURE;
				}
				flocation=strdup(optarg);
				break;
			case '?':
			default:
				usage();
				return EXIT_FAILURE;
		}
	}							
	argc -= optind;
	argv += optind;

	if (argc < 1) {usage();return EXIT_FAILURE;}
								
	printf("Connecting to %s:%s\n",ip,port);
	if (!(err = InitConnection(&fd,ip,port)))
		printf("Connected\n"); 
  else{
    fprintf(stderr,"ERROR: %s\n", gai_strerror(err));
    return EXIT_FAILURE;
  }
	if ((err = login(fd,nick)) != -1) printf("Logged in\n");
	else{perror("ERROR");return EXIT_FAILURE;}
	free(ip);
	free(nick);
	
	FILE *fp = fopen(flocation,"a+");
	if (!fp) {
		perror("Can't read file! Error: ");
		return EXIT_FAILURE;
	}

	int i=0,c=0;
	char buff[maxPacketSize];
								
	while((err = read(fd,&c,1))){
		if(c != '\n' && c != '\r'){buff[i++]=c;continue;}
		else buff[i]='\0';
		
		IRCPacket packet;
		getPacket(buff,&packet);
									
		if(packet.type != NULL){
			if(strcmp(packet.type,"376") == 0){
				for(int i = 0; i < argc; i++)
					join(fd,argv[i]);
			}else if(strcmp(packet.type,"PING") == 0){
				pong(fd,packet.content);
			}else{
				fp=fopen(flocation,"a+");
				char *msg=parseMSG(packet);
				printf("%s",msg);
				fputs(msg,fp);
        fclose(fp);
			}
		}
		i = 0;
    buff[0] = '\0';
	}
    if(err == -1) perror("Disconnected: ");
}
