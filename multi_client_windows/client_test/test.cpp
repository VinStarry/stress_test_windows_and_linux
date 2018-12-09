#include "client.h"

#pragma comment(lib,"ws2_32.lib")

const string configfile = "server.config";

int main()
{
	Client client1(configfile);
	// initialize winsock
	if (client1.initWinsock() == false) { return 1; }
	// set server ip and port, create socket
	if (client1.setServerParameters() == false) { return 1; }
	client1.loop();
	system("pause");
	client1.outputConnectionNums();

	WSACleanup();
	

	return 0;
}