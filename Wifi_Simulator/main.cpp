#include <iostream>
#include <memory>
#include "wifi_simulator.h"

int main() {
    int users[] = {1, 10, 100};
    for (int numUsers : users) {
        std::cout << "\nSimulating with " << numUsers << " user(s):\n";

        WiFiSimulator simulator(numUsers);
        simulator.runSimulation();
    }
    return 0;
}
