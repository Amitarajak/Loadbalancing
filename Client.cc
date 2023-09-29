/*
 * Client.cc
 *
 *  Created on: 15-Sep-2023
 *      Author: amita
 */
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;
// A new class is define which inherit the properties of cSimpleModule
class Client : public cSimpleModule {
private:                        //it is used because the variables which are declared here are used in this module
    int requestCount = 0;
    cMessage *requestTimer = nullptr;

protected:         //the variables over hear is used within the class and also with submodules
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sendRequest();

public:       //the variables over used within and outside class
    Client();    //it is used when object of the class is created
    virtual ~Client(); //it is used to clean when object of the class is destroyed
};

Define_Module(Client);//simple module class should be register with omnet++

Client::Client() {
    requestTimer = nullptr;//the pointer is initialize with nullptr means it is not pointing to any memory location
}

Client::~Client() {
    // It will clean up requestTimer if it exists and cancel the event
    if (requestTimer) {
        cancelEvent(requestTimer); // it cancel the scheduled event
        delete requestTimer; //it will delete the object
    }
}

void Client::initialize() { //it will define the implementation of initialize function of class client
    // Schedule the first request
    requestTimer = new cMessage("RequestTimer");
    sendRequest();
}


//it is invoked when module receives a message
void Client::handleMessage(cMessage *msg) {

    sendRequest();
}


//it is invoked when module wants to send a message
void Client::sendRequest() {
    // it will create and send a request
    char message[20];
    sprintf(message, "Request-%d", requestCount);//it is used to format and store a string value in message when it store integer value
    cMessage *requestMsg = new cMessage(message);//it will create new messages and give new address to that message
    send(requestMsg, "out");

    EV << "Sending request: " << message << endl;//it is sending messages and displaying it

    //it increments request count
    requestCount++;


    double randomDelay = uniform(1, 5); // Generate the next request with a random delay between 1 and 5 seconds
    scheduleAt(simTime() + randomDelay, requestTimer);//this schedules an event of future simulation time
}












