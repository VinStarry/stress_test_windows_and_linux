#include "client.h"

int main(int argc, const char *argv[]) {
    const string configfile = "network.config";
    cout << "cmake-build-debug/" << configfile << endl;
    Client client1(configfile);
    // set server ip and port
    client1.setServerParameters();
    // begin loop
    //client1.loop();
    client1.outputConnectionNums();
    getchar();

    return 0;
}