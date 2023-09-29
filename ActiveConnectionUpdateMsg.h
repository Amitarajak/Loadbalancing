// ActiveConnectionUpdateMsg.h
#ifndef __ACTIVECONNECTIONUPDATEMSG_H
#define __ACTIVECONNECTIONUPDATEMSG_H

#include <omnetpp.h>

using namespace omnetpp;

class ActiveConnectionUpdateMsg : public cMessage {
private:
    int serverIndex; // Custom field to store the server index
    int activeConnections;

public:
    ActiveConnectionUpdateMsg(const char *name = nullptr, int kind = 0) : cMessage(name, kind) {}//is a constructor that initializes an ActiveConnectionUpdateMsg object, allowing the specification of its name and kind, and it inherits from the cMessage class.

    // Getter and setter for server index
    int getServerIndex() const { return serverIndex; }//a member function that retrieves and returns the serverIndex property of an object, and the const keyword indicates that this function does not modify the object's internal state.
    void setServerIndex(int index) { serverIndex = index; }//a member function that sets the value of the serverIndex property of an object to the specified index.

    // Getter and setter for active connections
    int getActiveConnections() const { return activeConnections; }//a member function that retrieves and returns the value of the activeConnections property of an object, and the const keyword indicates that this function does not modify the object's internal state.
    void setActiveConnections(int connections) { activeConnections = connections; }//a member function that sets the value of the activeConnections property of an object to the specified connections value, allowing for updates to the number of active connections.

    virtual ActiveConnectionUpdateMsg *dup() const override {// a member function that creates a duplicate (copy) of an ActiveConnectionUpdateMsg object, allowing for safe message passing within the simulation environment without affecting the original message.
        return new ActiveConnectionUpdateMsg(*this);
    }
};

#endif
