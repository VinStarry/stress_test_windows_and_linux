#include "client.h"

string SERVER_IP = "127.0.0.1";
short SERVER_PORT = 5050;

std::mutex mtx;

Client::Client(string fileName) {
	fstream fin;
	fin.open(fileName, ios::in);
	if (!fin.is_open()) {
		cout << "Use default config:\n server ip = 127.0.0.1\nserver port = 5050" << endl;
		this->serverIp = "127.0.0.1";
		this->serverPort = 5050;
	}
	else {
		string key, eql, value;
		bool err = false;
		size_t tCnt, tsks;
		fin >> key >> eql >> value;
		if (key == "server_ip" && eql == "=") {
			this->serverIp = value;
		}
		else {
			err = true;
		}
		fin >> key >> eql >> value;
		if (key == "server_port" && eql == "=") {
			this->serverPort = atoi(value.c_str());
		}
		else {
			err = true;
		}
		fin >> key >> eql >> tCnt;
		if (key == "thread_number" && eql == "=") {
			this->threadNumber = tCnt;
		}
		else {
			err = true;
		}
		fin >> key >> eql >> tsks;
		if (key == "tasks_per_thread" && eql == "=") {
			this->tasksPerThread = tsks;
		}
		else {
			err = true;
		}
		if (err) {
			cout << "Use default config:\n server ip = 127.0.0.1\nserver port = 5050" << endl;
			this->serverIp = "127.0.0.1";
			this->serverPort = 5050;
		}
	}
	totalConnections = 0;
	cout << "Config succeessfully\n" << "Server IP: " << this->serverIp << "\nServer port: " << this->serverPort << endl;
	cout << "Thread number: " << this->threadNumber << "\nTasks per thread: " << this->tasksPerThread << endl;
	this->threadList = new vector<thread>;
}

Client::~Client() {
	if (this->threadList != nullptr) {
		this->threadList->clear();
		delete this->threadList;
	}
}

bool Client::initWinsock() {
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA initData;
	int err = WSAStartup(sockVersion, &initData);
	if (err != 0) {
		cout << "init Failed!" << endl;
		return false;
	}
	return true;
}

bool Client::setServerParameters() {
	this->serverAddr.sin_family = AF_INET;
	this->serverAddr.sin_port = htons(this->serverPort);
	in_addr addr;
	inet_pton(AF_INET, this->serverIp.c_str(), (void*)&addr);
	this->serverAddr.sin_addr = addr;

	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket == INVALID_SOCKET)
	{
		cout << "Create server socket failed" << endl;
		return false;
	}

	return true;
}

void Client::connectAndSend() {
	size_t tasks = this->tasksPerThread;
	size_t successNum = 0;

	while (tasks--){
		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		int err = connect(clientSocket, (sockaddr*)&this->serverAddr, sizeof(sockaddr));
		if (err == INVALID_SOCKET)
		{
			mtx.lock();
			cout << "connect failed!" << WSAGetLastError() << endl;
			mtx.unlock();
		}
		if (clientSocket == INVALID_SOCKET)
		{
			//cout << "Create socket failed!" << endl;
			continue;
		}

		const char *buf = "GET /hello.txt HTTP/1.1\r\n\0";

		int err1 = send(clientSocket, buf, strlen(buf), 0);
		if (err1 == SOCKET_ERROR)
		{
			//cout << "send failed!" << WSAGetLastError() << endl;
			continue;
		}

		mtx.lock();
		totalConnections++;
		successNum++;
		mtx.unlock();

		//shutdown(clientSocket, SD_SEND);
	}
}

void Client::loop() {
	
	for (size_t i = 0; i < this->threadNumber; i++) {
		threadList->push_back(std::thread(&Client::connectAndSend, this));
	}
	
	for (auto& th : *threadList) {
		th.join();
	}
}

void Client::outputConnectionNums() {
	cout << "Total connections number: " << this->totalConnections << endl;
}