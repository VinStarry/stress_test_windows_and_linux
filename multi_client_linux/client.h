//
// Created by vinstarry on 11/15/18.
//

#ifndef MULTI_CLIENT_LINUX_CLIENT_H
#define MULTI_CLIENT_LINUX_CLIENT_H

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET_ERROR -1

using std::string;
using std::fstream;
using std::vector;
using std::thread;
using std::ios;
using std::cout;
using std::endl;

class Client {
private:
    string serverIp;
    u_int16_t serverPort;
    size_t threadNumber;
    size_t tasksPerThread;
    size_t totalConnections;
    vector<thread> *threadList;
    struct sockaddr_in servaddr;

public:
    explicit Client(string filename);
    ~Client();
    void setServerParameters();
    void connectAndSend();
    void loop();
    void outputConnectionNums();
};


#endif //MULTI_CLIENT_LINUX_CLIENT_H
