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
#include "./include/irc-utils.h"
#include "./include/utils.h"

#define maxPacketSize 1000

/* This is an IRC bot that saves IRC conversations */
int
main(int argc, char *argv[]) {
	int err = 0,fd = 0;
	char *ip = malloc(100), *port = malloc(6), 
	     *nick = malloc(60), *flocation = malloc(200);
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
				if(atoi(optarg) > 65535){
					printf("Port number should be 0-65535\n");
					return EXIT_FAILURE;
				}
				port=strdup(optarg);
				break;
			case 'n':
				if(strlen(optarg)>60){
					printf("Nickname should not be longer than 60 characters\n");
					return EXIT_FAILURE;
				}				
				nick=strdup(optarg);
				break;
			case 'o':
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
	if ((err = InitConnection(&fd,ip,port)) != -1)
		printf("Connected\n"); else perror("ERROR: ");
	if ((err = login(fd,nick)) != -1) printf("Logged in\n");
	else perror("ERROR: ");
	free(ip);
	free(nick);
	
	sleep(1);
	for (int i = 0; i < argc; i++)
		join(fd,argv[i]);
	FILE *fp = fopen(flocation,"a+");
	if (!fp) {
		perror("Can't read file! Error: ");
		return EXIT_FAILURE;
	}

	int i=0,c=0;
	char *buff=malloc(maxPacketSize);
								
	while(read(fd,&c,1)){
		if(c != '\n' && c != '\r'){buff[i++]=c;continue;}
		else buff[i]='\0';
		
		IRCPacket packet;
		getPacket(buff,&packet);
		
		if(packet.type != NULL){
			fp=fopen(flocation,"a+");
			char *msg=parseMSG(packet);
			printf("%s",msg);
			fputs(msg,fp);
			fclose(fp);
		}
		i=0;
		memset(buff,0,strlen(buff));
	}
}
