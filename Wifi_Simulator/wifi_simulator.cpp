#include "wifi_simulator.h"
#include <iostream>
#include <iomanip> // For formatting output
#include <vector>
#include <string>

// Constructor
WiFiSimulator::WiFiSimulator(int numUsers) : numUsers(numUsers) {
    ap = std::make_shared<AccessPoint>(20.0); // 20 MHz bandwidth
}

// Function to print results in a tabular format
void WiFiSimulator::printResultsTable(const std::vector<SimulationResult>& results) {
    std::cout << "| Users | WiFi Type        | Throughput (Mbps) | Avg Latency (ms) | Max Latency (ms) |\n";
    std::cout << "|-------|------------------|-------------------|------------------|------------------|\n";
    
    for (const auto& result : results) {
        // Adjust the widths and precision to ensure better formatting
        std::cout << "| " 
                  << std::setw(6) << result.numUsers << " | " 
                  << std::setw(16) << result.wifiType << " | "
                  << std::setw(17) << std::fixed << std::setprecision(4) << result.throughput << " | "
                  << std::setw(16) << std::fixed << std::setprecision(4) << result.avgLatency << " | "
                  << std::setw(16) << std::fixed << std::setprecision(4) << result.maxLatency << " |\n";
    }
}
// Run the simulation for all WiFi types
void WiFiSimulator::runSimulation() {
    std::vector<SimulationResult> results;

    // WiFi 4 simulation
    WiFi4 wifi4(numUsers, ap);
    results.push_back({
        numUsers, "WiFi 4",
        wifi4.calculateThroughput(),
        wifi4.calculateAverageLatency(),
        wifi4.calculateMaxLatency()
    });

    // WiFi 5 simulation
    WiFi5 wifi5(numUsers, ap);
    results.push_back({
        numUsers, "WiFi 5",
        wifi5.calculateThroughput(),
        wifi5.calculateAverageLatency(),
        wifi5.calculateMaxLatency()
    });

    // WiFi 6 simulation for different subchannel configurations
    for (int subchannels : {10, 5, 2}) {
        WiFi6 wifi6(numUsers, subchannels, 20.0, 1024);
        results.push_back({
            numUsers, "WiFi 6 (" + std::to_string(subchannels) + " SC)",
            wifi6.calculateThroughput(),
            wifi6.calculateAverageLatency(),
            wifi6.calculateMaxLatency()
        });
    }

    // Print results in tabular format
    printResultsTable(results);
}
