#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "./include/irc-utils.h"

void
getPacket(char *buff,IRCPacket *packet){
	if(buff[0] == ':'){
		packet->sender=strtok(buff," ")+1;
		packet->type=strtok(NULL," ");
		packet->channel=strtok(NULL," ");
		packet->content=strtok(NULL,"\0");
								
		if(packet->type != NULL)
			packet->content++;
		if(strcmp(packet->type,"PRIVMSG") == 0 
		|| strcmp(packet->type,"JOIN") == 0){
			char *tmp=malloc(128);
			strcpy(tmp,packet->sender);

			packet->sender=strtok(tmp,"!");
			packet->realname=strtok(NULL,"@");
			packet->hostname=strtok(NULL,"\0");

			free(tmp);
		}
	}
}

char*
parseMSG(IRCPacket packet){
								
	time_t now = time(NULL);
	char *tmstr=malloc(50);
	tmstr=strdup(ctime(&now));
	tmstr[strlen(tmstr)-1]='\0';
	if(strcmp(packet.type,"PRIVMSG") == 0){
		char *s=malloc(1000);
		sprintf(s,"[%s] %s -> %s | %s\n",
				tmstr,packet.sender,
				packet.channel,packet.content);
		free(tmstr);
		return s;
	}else if(strcmp(packet.type,"JOIN") == 0){
		char *s=malloc(1000);
		sprintf(s,"[%s] %s joined %s\n",tmstr,
				packet.sender,++packet.channel);
		free(tmstr);
		return s;
	}else return "";
}
