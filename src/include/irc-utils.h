typedef struct IRCPacket{
	char *sender,*realname,*channel,*hostname,*type,*content;

} IRCPacket;

void	 getPacket(char *,IRCPacket *);
char	*parseMSG(IRCPacket);
