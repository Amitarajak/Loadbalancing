#include <omnetpp.h>
#include "ActiveConnectionUpdateMsg.h"

using namespace omnetpp;

class Server : public cSimpleModule {
private:
    int maxConnections;
    int numConnections = 0;
    simtime_t processingTime;
    cMessage *syncEvent; // Event for synchronization

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void sendActiveConnectionUpdate();

public:
    Server();
    virtual ~Server();
};

Define_Module(Server);

Server::Server() {
    maxConnections = 6; // Setting the maximum connections for this server
    syncEvent = nullptr;// sets the syncEvent pointer to a null value, indicating that it currently does not point to any valid object or memory location.
}

Server::~Server() {
    cancelAndDelete(syncEvent);//cancel and delete the scheduled event pointed to by syncEvent, preventing it from being executed and freeing the associated memory.
}

void Server::initialize() {
    processingTime = 5.0; // Set the processing time for this server

    // Schedule the initial self-message for sending active connection update
    scheduleAt(simTime() + exponential(processingTime), new cMessage("SendActiveConnectionUpdate"));

    // Initialize the synchronization event
    syncEvent = new cMessage("SyncEvent");//dynamically allocates memory for a new cMessage object and assigns it to the syncEvent pointer, initializing it with the name "SyncEvent".
    scheduleAt(simTime() + 5.0, syncEvent); // Synchronize after 5 seconds (adjust as needed)
}

void Server::handleMessage(cMessage *msg) {
    if (msg == syncEvent) {
        // Synchronization event: Sending the active connection update to the load balancer
        sendActiveConnectionUpdate();
    } else if (msg->isSelfMessage()) {
        // This branch is executed when it's time to send an active connection update
        sendActiveConnectionUpdate();

        // Schedule the next self-message for sending active connection update
        scheduleAt(simTime() + exponential(processingTime), new cMessage("SendActiveConnectionUpdate"));

        delete msg; // Deleting the self-message
    } else if (numConnections < maxConnections) {
        // Process the client request
        EV_INFO << "Received a request from client\n";

        // Simulate server processing time
        simtime_t processingDelay = exponential(processingTime);
        EV_INFO << "Processing time: " << processingDelay << endl;

        // Schedule a self-message to continue processing after the delay
        scheduleAt(simTime() + processingDelay, msg);

        // Increment the number of connections
        numConnections++;
    } else {
        EV_INFO << "Server is busy. Dropping the request\n";
        delete msg; // Delete the client request when the server is busy
    }
}

void Server::sendActiveConnectionUpdate() {
    ActiveConnectionUpdateMsg *updateMsg = new ActiveConnectionUpdateMsg("ActiveConnection");//dynamically allocates memory for a new ActiveConnectionUpdateMsg object with the name "ActiveConnection" and assigns it to the updateMsg pointer.
    updateMsg->setActiveConnections(numConnections);//sets the value of the ActiveConnectionUpdateMsg object's activeConnections property to the value stored in the numConnections variable.

    // Sending the active connection update to the LoadBalancer
    send(updateMsg, "out");
}
