// JG 2014 05 05 
// Simple multiCast Time Server/Client with Microseconds
// gcc -o timecast multicast.c
// usage :
// server: ./timecast -
// client: ./timecast 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>

#define PORT 6000
#define MULTICAST_GROUP "239.0.0.1"
#define SLEEP_TIME 10000 // usleep

double getCurrentTimeWithMicroSeconds();

int main(int argc, char **argv){

	struct sockaddr_in addr;
	int addrlen, sock, cnt;
	struct ip_mreq mreq;
	char message[500];

// Socket
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	addrlen = sizeof(addr);

	if (argc > 1) {
	//Sender
		addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);
		while (1) {
			//time_t t = time(0);
			sprintf(message, "%6.6f", getCurrentTimeWithMicroSeconds());
			printf("sending: %s\n", message);
			cnt = sendto(sock, message, sizeof(message), 0, (struct sockaddr *) &addr, addrlen);
			if (cnt < 0) {
				perror("sendto");
				exit(1);
			}
			usleep(SLEEP_TIME);
		}
	} else {
	//Reciver
		if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {        
			perror("bind");
			exit(1);
		}    
		mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_GROUP);         
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);         
		if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
			perror("setsockopt mreq");
			exit(1);
		}         
		while (1) {
			cnt = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *) &addr, (socklen_t *)&addrlen);
			if (cnt < 0) {
				perror("recvfrom");
				exit(1);
			} else if (cnt == 0) {
				break;
			}
			printf("%s: message = \"%s\" ", inet_ntoa(addr.sin_addr), message);
			printf("  %6.6f \n",getCurrentTimeWithMicroSeconds()-atof(message));
			//printf("time for message to send ")
		}
	}
	return(0);
}


double getCurrentTimeWithMicroSeconds(){

  struct timeval now;
	int errorstatus;
	double timenow;
	
	errorstatus=gettimeofday(&now, NULL);

	if(errorstatus==0) {
		timenow= now.tv_sec+ (double)now.tv_usec/1000000 ;
	}
	else {
		printf("gettimeofday() failed, errno = %d\n",
				errno);
		return -1;
	}

	return timenow;
}