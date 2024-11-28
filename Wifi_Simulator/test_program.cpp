#include "wifi_simulator.h"
#include <iostream>

int main() {

    for(int i=1;i<=100;i++){
    WiFiSimulator simulator(i);
    simulator.runSimulation();
    }
    return 0;
}

