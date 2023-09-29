//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package finalexam;
//
// TODO documentation
//
simple Client
{
    @display("i=abstract/person");
    gates:
        output out;
}

//
// TODO documentation
//
simple Server
{
    @display("i=block/app2");
    gates:
        input in;
        output out;
}

//
// TODO documentation
//
simple Loadbalancer
{
    @display("i=abstract/dispatcher");
    gates:
        input in1;// this is input gates
        input in2;
        input in3;
        input in4;
        input in5;
        output out1;//this is output gates
        output out2;
        output out3;
}

//
// TODO documentation
//
network Network
{
    @display("i=abstract/db;bgb=1066,490");
    submodules:
        client: Client {
            @display("p=80,65");
        }
        client1: Client {
            @display("p=81,244");
        }
        loadbalancer: Loadbalancer {
            @display("p=314,161");
        }
        server: Server {
            @display("p=759,56");
        }
        server1: Server {
            @display("p=761,180");
        }
        server2: Server {
            @display("p=748,314");
        }
    connections:
        client.out --> loadbalancer.in1;//This is the connections between client output gate and load balancer input gate
        client1.out --> loadbalancer.in2;
        loadbalancer.out1 --> server.in;
        loadbalancer.out2 --> server1.in;
        loadbalancer.out3 --> server2.in;
        server.out --> loadbalancer.in3;
        server1.out --> loadbalancer.in4;
        server2.out --> loadbalancer.in5;
}

