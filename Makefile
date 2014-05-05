CC=gcc
CFLAGS = -g 
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: timecast

timecast: multicast.c
	$(CC) -o timecast multicast.c $(LIBS)

clean:
	rm -f timecast
