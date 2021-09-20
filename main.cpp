#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#define INFO_BUFFER_SIZE 32
#undef UNICODE

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

std::string Retrieve_IPv4_str() {
	std::string IPv4_Address_str;
	WSADATA wsaData;
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;
	struct sockaddr_in* sockaddr_ipv4;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return "WSAStartup Error";
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf, &bufCharCount);

	DWORD dwRetval = getaddrinfo(infoBuf, "0", &hints, &result);
	if (dwRetval != 0) {
		printf("getaddrinfo failed with error: %d\n", dwRetval);
		WSACleanup();
		return "getaddrinfo Error";
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		switch (ptr->ai_family) {
		case AF_INET:
			sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
			IPv4_Address_str = inet_ntoa(sockaddr_ipv4->sin_addr);
			break;
		}
	}

	freeaddrinfo(result);
	WSACleanup();
	return IPv4_Address_str;
}

int main(void) {
	std::cout << "IPv4: " << Retrieve_IPv4_str() << "\n";
	system("pause");
	return 0;
}
