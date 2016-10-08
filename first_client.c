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

int main(int argc, char const *argv[])
{
	
	#if _WIN32 || _WIN64
	WORD wVersionRequested;
  WSADATA wsaData;
  int err;
  #endif

	int status, sockfd, buffer_len;
	void *buffer;
	struct addrinfo hints;
	struct addrinfo *servinfo;

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
	hints.ai_family = AF_UNSPEC; 			// don't care IPv4 or v6
	hints.ai_socktype = SOCK_STREAM;	// TCP stream sockets

	// get ready to connect
	status = getaddrinfo("localhost", "3490", &hints, &servinfo);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo error %s\n", gai_strerror(status));
		exit(1);
	}

	sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	if (sockfd == -1) {
		fprintf(stderr, "socket error %s\n", gai_strerror(errno));
		exit(1);
	}

	status = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);

	if (status == -1) {
		fprintf(stderr, "connect error %s\n", gai_strerror(errno));
		exit(1);
	}

	// Receive message from server
	status = recv(sockfd, buffer, 50, 0);

	if (status == -1) {
		fprintf(stderr, "recv error %s\n", gai_strerror(errno));
		exit(1);
	}

	printf("%s\n", buffer);

	return 0;
}	