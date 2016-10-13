# include <stdio.h>
# include <stdlib.h>
#if _WIN32 || _WIN64
# include <winsock2.h>
# include <WS2tcpip.h>
# include <windows.h>
# pragma comment(lib, "Ws2_32.lib")
#else
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
#endif

int main(){
	
	#if _WIN32 || _WIN64
	WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  #endif

  int s, sockfd, new_fd;
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;	// will point to results
	struct sockaddr_storage their_addr;
	socklen_t addr_size;

	char *msg = "Beej was here!";
	int len, bytes_sent;

	char *hostname;

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
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets
	hints.ai_flags = AI_PASSIVE;			// fill in my IP for me

	if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error %s\n", gai_strerror(status));
		exit(1);
	}

	printf("Shit works\n");

	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	// if socket() call failed
	if (sockfd == -1) {
		fprintf(stderr, "socket error %s\n", gai_strerror(errno));
			exit(1);
	}
	
	// bind it to the port we passed in to getaddrinfo():

	s = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);

	// if bind() call failed
	if (s == -1) {
		fprintf(stderr, "bind error %s\n", gai_strerror(errno));
			exit(1);
	}

	// listen for incoming connect
	s = listen(sockfd, 20);

	if (s == -1) {
		fprintf(stderr, "listen error %s\n", gai_strerror(errno));
		exit(1);
	}

	// Accept incoming connection
	addr_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

	if (new_fd == -1) {
		fprintf(stderr, "accept error %s\n", gai_strerror(errno));
		exit(1);	
	}

	len = strlen(msg);
	bytes_sent = send(new_fd, msg, len, 0);

	if (bytes_sent == -1) {
		fprintf(stderr, "send error %s\n", gai_strerror(errno));
		exit(1);	
	}

	s = gethostname(&hostname, sizeof &hostname);

	if (s == -1) {
		fprintf(stderr, "gethostname error %s\n", gai_strerror(errno));
		exit(1);		
	}
	
	printf("hostname is %s\n", hostname);

	freeaddrinfo(servinfo); // Free the linked-list
	#if _WIN32 || _WIN64
	closesocket(sockfd);
	#else
	close(sockfd);
	#endif
	return 0;
}