#ifndef WIFI6_H
#define WIFI6_H

#include "wifi_standard.h"
#include <memory>
#include <vector>

class WiFi6 : public WiFiStandard {
public:
    WiFi6(int numUsers, int numSubchannels, double bandwidthMHz, int packetSize);

    double simulateTransmission();
    double calculateThroughput();
    double calculateAverageLatency();
    double calculateMaxLatency();

private:
    int numUsers;
    int numSubchannels;
    double bandwidthMHz;
    int packetSize;
    std::vector<User> users;  // Declare users vector to store user objects
};

#endif
