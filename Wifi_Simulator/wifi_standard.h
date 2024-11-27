#ifndef WIFI_STANDARD_H
#define WIFI_STANDARD_H

#include <vector>
#include <memory>
#include "access_point.h"
#include "user.h"

class WiFiStandard {
public:
    virtual double calculateThroughput() = 0;
    virtual double calculateAverageLatency() = 0;
    virtual double calculateMaxLatency() = 0;
    virtual ~WiFiStandard() = default;
};

#endif
