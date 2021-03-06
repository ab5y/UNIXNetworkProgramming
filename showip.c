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

	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

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
	
	if (argc != 2) {
		fprintf(stderr, "usage: showip hostname\n");
		return 1;
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;			// AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;	// TCP IP

	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	printf("IP addresses for %s\n\n", argv[1]);

	for(p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf(" %s: %s\n", ipver, ipstr);
	}
	freeaddrinfo(res);	// Free the linked-list
	return 0;
}