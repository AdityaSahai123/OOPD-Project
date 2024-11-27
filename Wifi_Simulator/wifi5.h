#ifndef WIFI5_H
#define WIFI5_H

#include "wifi_standard.h"
#include "constants.h"
#include <memory>
#include <vector>
#include <queue>

class WiFi5 : public WiFiStandard {
public:
    WiFi5(int numUsers, std::shared_ptr<AccessPoint> ap);

    double simulateTransmission();
    double calculateThroughput();
    double calculateAverageLatency();
    double calculateMaxLatency();

private:
    int numUsers;                                // Number of users in the network
    std::shared_ptr<AccessPoint> ap;            // Access point
    std::vector<User> users;                    // List of users
    std::queue<int> transmissionQueue;          // Round-robin scheduling

    // Simulation phases
    double simulateBroadcastPhase();
    double simulateCSIPhase();
    double simulateParallelTransmission();

    // Helper functions
    void initializeRoundRobin();
    int getNextUserInQueue();
};

#endif
