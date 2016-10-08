#if _WIN32 || _WIN64
# include <winsock2.h>
# include <WS2tcpip.h>
# include <windows.h>
# pragma comment(lib, "Ws2_32.lib")
#else
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
#endif

int main() {
	struct sockaddr_in sa;		//IPv4
	struct sockaddr_in6	sa6;	//IPv6

	int i = inet_pton(AF_INET, "172.31.10.41", &(sa.sin_addr));										//IPv4
	printf("IPv4 result is %d\n", i);
	i = inet_pton(AF_INET6, "fe80::ecc4:7495:937c:4e86", &(sa6.sin6_addr));	//IPv6
	printf("IPv6 result is %d\n", i);

	return 0;
}