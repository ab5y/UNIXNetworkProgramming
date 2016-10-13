/*
** udp_server.c -- a datagram sockets server demo
*/

#if _WIN32 || _WIN64
# include <winsock2.h>
# include <WS2tcpip.h>
# include <windows.h>
# pragma comment(lib, "Ws2_32.lib")
#else
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
#endif

#define MYPORT "4950"		// The port users will be connecting to

#define MAXBUFLEN 100

// get sockaddr, IPv4 or IPv6
void* get_in_addr(struct sockaddr *sa) {
	if(sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void){
	
	#if _WIN32 || _WIN64
	WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  #endif

  int sockfd;
	struct addrinfo hints;
	struct addrinfo *servinfo;	// will point to results
	struct addrinfo *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	#if _WIN32 || _WIN64
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}
	#endif

	memset(&hints, 0, sizeof hints);	// make sure the struct is empty
	hints.ai_family = AF_UNSPEC; 			// IP agnostic
	hints.ai_socktype = SOCK_DGRAM;	// TCP stream sockets
	hints.ai_flags = AI_PASSIVE;			// fill in my IP for me

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p->ai_next) {
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	printf("listener: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';
	printf("listener: packet contains \"%s\"\n", buf);
	close(sockfd);
	return 0;
}