#ifndef WIFI_SIMULATOR_H
#define WIFI_SIMULATOR_H

#include <memory>
#include <vector>
#include <string>
#include <fstream> 
#include "wifi4.h"
#include "wifi5.h"
#include "wifi6.h"
#include "access_point.h"


struct SimulationResult {
    int numUsers;
    std::string wifiType;
    double throughput;
    double avgLatency;
    double maxLatency;
};

class WiFiSimulator {
public:
    explicit WiFiSimulator(int numUsers); 
    void runSimulation();                 
private:
    int numUsers;                        
    std::shared_ptr<AccessPoint> ap;      
    void printResultsTable(const std::vector<SimulationResult>& results);
    void printBarChart(const std::vector<SimulationResult>& results);
    void logResultsToFile(const std::vector<SimulationResult>& results);
};

#endif
