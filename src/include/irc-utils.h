#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct IRCPacket {
    char *sender, *realname, *channel, *hostname, *type, *content;

} IRCPacket;

void getPacket(char *, IRCPacket *);

char *parseMSG(IRCPacket);
