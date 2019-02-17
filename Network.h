#pragma once
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#pragma comment(lib, "ws2_32.lib")

class Network
{
	public:
		Network();
		void Connect();
		void Disconnect();
		int Send(const char *buffer, int len);
		int Recv(char *buffer, int len);

		bool isConnected();

	private:
		WSADATA WSAData;
		SOCKET sock;
		SOCKADDR_IN sin;

		bool _connected = false;
};