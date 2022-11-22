PROGRAM-NAME=	ircarchiver
SRC=	./src/*.c
CFLAGS+= -fsanitize=address
DEBUG+= -g -Werror -Wall -Wpedantic -Wshadow -Wpadded -Winline -Wwrite-strings

all:	$(SRC) ./src/include/*.h
	$(CC) $(CFLAGS) -o $(PROGRAM-NAME) $(SRC)
debug:
	$(CC) $(CFLAGS) $(DEBUG) -o $(PROGRAM-NAME) $(SRC)
clean:
	rm -f $(PROGRAM-NAME) $(PROGRAM-NAME).core vgcore* *.txt
