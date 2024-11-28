#ifndef WIFI_SIMULATOR_H
#define WIFI_SIMULATOR_H

#include <memory>
#include <vector>
#include <string>
#include <fstream> // For file operations
#include "wifi4.h"
#include "wifi5.h"
#include "wifi6.h"
#include "access_point.h"

// Struct to hold simulation results for tabular output
struct SimulationResult {
    int numUsers;
    std::string wifiType;
    double throughput;
    double avgLatency;
    double maxLatency;
};

class WiFiSimulator {
public:
    explicit WiFiSimulator(int numUsers); // Constructor to initialize the number of users
    void runSimulation();                 // Run the WiFi simulations and print results

private:
    int numUsers;                         // Number of users in the simulation
    std::shared_ptr<AccessPoint> ap;      // Access Point shared between WiFi types

    // Private method to print results in tabular format
    void printResultsTable(const std::vector<SimulationResult>& results);

    // Private method to print bar chart visualization
    void printBarChart(const std::vector<SimulationResult>& results);

    // Private method to log results to a file
    void logResultsToFile(const std::vector<SimulationResult>& results);
};

#endif
