#pragma once

#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <string>
#include <thread>
#include <mutex>
#include <vector>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

class Client {
private: 
	string serverIp;
	short serverPort;
	SOCKET serverSocket;
	size_t threadNumber;
	size_t tasksPerThread;
	sockaddr_in serverAddr;
	size_t totalConnections;

	std::vector<std::thread> *threadList;

protected:
	

public:
	Client(string configFileName);
	~Client();

	virtual void connectAndSend();
	bool initWinsock();
	bool setServerParameters();
	void loop();
	void outputConnectionNums();

};