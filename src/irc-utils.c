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
			packet->sender=strtok(packet->sender,"!");
			packet->realname=strtok(NULL,"@");
			packet->hostname=strtok(NULL,"\0");
		}
	}else if(buff[0] != 0){
		packet->type=strtok(buff," ");
		if(strcmp(packet->type,"PING") == 0){
			packet->content=strtok(NULL,"\0");
			packet->content++;
		}
	}
}

char*
parseMSG(IRCPacket packet){
	time_t now = time(NULL);
	char *tmstr = strdup(ctime(&now));
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
