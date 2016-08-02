targets = server client

objects = server.o client.o

CFLAGS = -Wall -pedantic -ansi -g

all : $(targets) reloc

server : server.o
client : client.o
$(targets) :
	$(CC) $< -o $@

reloc :
	mkdir -p ../build
	mv $(targets) ../build

$(objects) : common.h
$(objects) : %.o : %.c
	$(CC) -c $(CFLAGS) $<

clean :
	-rm -rf *.o ../build

.PHONY : clean reloc
