#include "wifi_simulator.h"
#include <iostream>

int main() {
    // Number of users for the simulation

    for(int i=10;i<=1000;i+=50){
    //int numUsers = 10;  // You can change this as needed

    // Create a WiFiSimulator object with the specified number of users
    WiFiSimulator simulator(i);

    // Run the simulation and display the results
    simulator.runSimulation();
    }
    return 0;
}

