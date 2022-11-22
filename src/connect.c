#include "./include/connect.h"

/*
 * Connect to the IRC Server
 */
int InitConnection(int *fd, char *ip, char *port) {
  struct addrinfo hints, *res;
  int err;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  err = getaddrinfo(ip, port, &hints, &res);
  if (err != 0)
    return err;
  *fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  err = connect(*fd, res->ai_addr, res->ai_addrlen);
  if (*fd < 0)
    return -1;

  return err;
}

/*
 * Login in into the server
 */
ssize_t login(int fd, char *nick) {
  char s[100];
  sprintf(s, "\rNICK %s\r\nUSER %s 0 * :realname\r\n", nick, nick);
  ssize_t err = write(fd, s, sizeof(s));

  return err;
}

/*
 * Join an IRC channel
 */
ssize_t join(int fd, char *channel) {
  char s[40];
  sprintf(s, "\rJOIN :#%s\r\n", channel);
  ssize_t err = write(fd, s, strlen(s));

  return err;
}

ssize_t pong(int fd, char *token) {
  char s[40];
  sprintf(s, "\rPONG :%s\r\n", token);
  ssize_t err = write(fd, s, strlen(s));

  return err;
}
