#include <string.h>
#include <omnetpp.h>
#include "ActiveConnectionUpdateMsg.h"

using namespace omnetpp;

class Loadbalancer : public cSimpleModule {
private:
    int numServers;
    int* activeConnections;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    int findServerWithLeastConnections();

public:
    Loadbalancer();
    virtual ~Loadbalancer();
};

Define_Module(Loadbalancer);

Loadbalancer::Loadbalancer() {
    activeConnections = nullptr;//assigns a null pointer value to the variable
}

Loadbalancer::~Loadbalancer() {
    delete[] activeConnections;// deallocates the memory pointed to by the activeConnections array,
                               //releasing the previously allocated memory.
}

void Loadbalancer::initialize() {
    numServers = 3;
    activeConnections = new int[numServers];//dynamically allocates memory to create an integer array named
                                            //activeConnections with a size specified by numServers.

    for (int i = 0; i < numServers; i++) {
        activeConnections[i] = 0;
    }
}

void Loadbalancer::handleMessage(cMessage *msg) {
    if (dynamic_cast<ActiveConnectionUpdateMsg *>(msg)) {//a conditional statement that checks if the message msg can be successfully cast to the
                                                         //ActiveConnectionUpdateMsg type, which is determined at runtime.
        ActiveConnectionUpdateMsg *updateMsg = dynamic_cast<ActiveConnectionUpdateMsg *>(msg);//attempts to cast the message msg to the ActiveConnectionUpdateMsg type and assigns the result to the pointer variable updateMsg,
                                                                                              //allowing access to the message's properties if the cast is successful.
        int serverIndex = updateMsg->getServerIndex();//retrieves the value of the serverIndex property from the
                                                     //updateMsg object and assigns it to the serverIndex variable.

        if (serverIndex >= 0 && serverIndex < numServers) {
            int serverActiveConnections = updateMsg->getActiveConnections();//The line int serverActiveConnections = updateMsg->getActiveConnections(); retrieves the value of the serverActiveConnections property from the updateMsg object and
                                                                            //assigns it to the serverActiveConnections variable.
            EV_INFO << "Received active connections update for server " << serverIndex << ": " << serverActiveConnections << " connections" << endl;//This line logs a message indicating the received active connection update for a server, including the server's index and the number of connections.

            activeConnections[serverIndex] = serverActiveConnections;//This line assigns the value of `serverActiveConnections` to the `activeConnections` array at the index specified by `serverIndex`.
        } else {
            EV_ERROR << "Received an active connection update from an unknown server." << endl;
        }

        delete msg;
    } else {
        // Handle messages from clients and forward them to the server with the least load
        int serverIndex = findServerWithLeastConnections();

        if (serverIndex != -1) {
            EV_INFO << "Forwarding message to Server " << serverIndex << endl;
            std::string gateName = "out" + std::to_string(serverIndex + 1);
            send(msg, gateName.c_str());
        } else {
            EV_ERROR << "No servers available to handle the message." << endl;
            delete msg;
        }
    }
}

int Loadbalancer::findServerWithLeastConnections() {
    int minConnections = INT_MAX;//The line int minConnections = INT_MAX; initializes the minConnections variable to the maximum possible value representable by an integer in C++, providing a safe starting point for finding the minimum value.
    int serverIndex = -1;//it will indicate uninitialized or invalid state

    for (int i = 0; i < numServers; i++) {
        if (activeConnections[i] < minConnections) {//This line checks if the value at the i-th index of the activeConnections array is less than the minConnections value, performing a comparison for the minimum.
            minConnections = activeConnections[i];//This line assigns the value of activeConnections[i] to the minConnections variable, updating it with a smaller value.
            serverIndex = i;
        }
    }

    return serverIndex;
}
