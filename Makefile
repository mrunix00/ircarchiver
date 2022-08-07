PROGRAM-NAME=	ircarchiver
SRC=	./src/*.c

all:	$(SRC) ./src/*.h
	$(CC) $(CFLAGS) -o $(PROGRAM-NAME) $(SRC)
clean:
	rm -f $(PROGRAM-NAME) $(PROGRAM-NAME).core vgcore* *.txt
