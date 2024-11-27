#ifndef WIFI4_H
#define WIFI4_H

#include "wifi_standard.h"
#include "access_point.h"
#include <vector>
#include <memory>

class WiFi4 : public WiFiStandard {
public:
    WiFi4(int numUsers, std::shared_ptr<AccessPoint> ap);

    double calculateThroughput() override;
    double calculateAverageLatency() override;
    double calculateMaxLatency() override;

private:
    double simulateTransmission();
    int numUsers;
    std::shared_ptr<AccessPoint> ap;
    std::vector<User> users;

    double averageLatency; // Store average latency
    double maxLatency;     // Store max latency
};

#endif // WIFI4_H
