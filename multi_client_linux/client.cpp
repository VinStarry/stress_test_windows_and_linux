//
// Created by vinstarry on 11/15/18.
//

#include "client.h"

std::mutex mtx;

Client::Client(string filename) {
    fstream fin;
    fin.open(filename, ios::in | ios::binary);
    if (!fin.is_open()) {
        cout << "can't open config file" << endl;
        cout << "Use default config:\nserver ip = 127.0.0.1\nserver port = 5050" << endl;
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
            this->serverPort = std::stoi(value);
        }
        else {
            err = true;
        }
        fin >> key >> eql >> tCnt;
        if (key == "thread_number" && eql == "=") {
            this->threadNumber = tCnt;
            this->threadNumber = 1;
        }
        else {
            err = true;
        }
        fin >> key >> eql >> tsks;
        if (key == "tasks_per_thread" && eql == "=") {
            this->tasksPerThread = tsks;
        }
        else {
            this->tasksPerThread = 1;
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
        threadList = nullptr;
    }
}

void Client::setServerParameters() {
    bzero(&this->servaddr, sizeof(this->servaddr));
    this->servaddr.sin_family = AF_INET;
    this->servaddr.sin_port = htons(this->serverPort);
    inet_pton(AF_INET, this->serverIp.c_str(), &this->servaddr.sin_addr);
}

void Client::connectAndSend() {
    size_t tasks = this->tasksPerThread;
    size_t successNum = 0;

    while (tasks--) {
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == SOCKET_ERROR) {
            continue;
        }

        int err = connect(clientSocket, (struct sockaddr*)&this->servaddr, sizeof(sockaddr));

        if (err == SOCKET_ERROR) {
            mtx.lock();
            cout << "Connect error" << endl;
            mtx.unlock();
        }

        const char* sendBuf = "GET /hello.txt HTTP/1.1\r\n\0";

        auto err1 = send(clientSocket, sendBuf, strlen(sendBuf), 0);

        if (err1 == SOCKET_ERROR) {
            continue;
        }

        mtx.lock();
        this->totalConnections++;
        successNum++;
        mtx.unlock();
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