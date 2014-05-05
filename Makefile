CC=gcc
CFLAGS = -Wall -Werror
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: timecast

timecast: multicast.c
	$(CC) $(CFLAGS) -o timecast multicast.c $(LIBS)

clean:
	rm -f timecast
