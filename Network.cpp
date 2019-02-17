#include "Network.h"

Network::Network()
{
}

void Network::Connect()
{
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(42);

	int connected = connect(sock, (SOCKADDR *)&sin, sizeof(sin));
	if (connected == 0) _connected = true;
}

void Network::Disconnect()
{
	closesocket(sock);
	WSACleanup();
}

int Network::Send(const char * buffer, int len)
{
	return send(sock, buffer, len, 0);
}

int Network::Recv(char * buffer, int len)
{
	return recv(sock, buffer, len, 0);;
}

bool Network::isConnected()
{
	return _connected;
}
