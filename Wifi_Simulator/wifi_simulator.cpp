#include "wifi_simulator.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream> // Include for file operations

// Constructor
WiFiSimulator::WiFiSimulator(int numUsers) : numUsers(numUsers) {
    ap = std::make_shared<AccessPoint>(20.0); // 20 MHz bandwidth
}

// Function to print results in a tabular format
void WiFiSimulator::printResultsTable(const std::vector<SimulationResult>& results) {
    std::cout << "| Users | WiFi Type        | Throughput (Mbps) | Avg Latency (ms) | Max Latency (ms) |\n";
    std::cout << "|-------|------------------|-------------------|------------------|------------------|\n";
    
    for (const auto& result : results) {
        std::cout << "| " 
                  << std::setw(6) << result.numUsers << " | " 
                  << std::setw(16) << result.wifiType << " | "
                  << std::setw(17) << std::fixed << std::setprecision(4) << result.throughput << " | "
                  << std::setw(16) << std::fixed << std::setprecision(4) << result.avgLatency << " | "
                  << std::setw(16) << std::fixed << std::setprecision(4) << result.maxLatency << " |\n";
    }
}

// Function to print bar chart
void WiFiSimulator::printBarChart(const std::vector<SimulationResult>& results) {
    const int barWidth = 50; // Width of the bar chart
    double maxThroughput = 0, maxAvgLatency = 0, maxMaxLatency = 0;

    // Find maximum values for scaling
    for (const auto& result : results) {
        maxThroughput = std::max(maxThroughput, result.throughput);
        maxAvgLatency = std::max(maxAvgLatency, result.avgLatency);
        maxMaxLatency = std::max(maxMaxLatency, result.maxLatency);
    }

    std::cout << "\nThroughput (Mbps) Bar Chart:\n";
    for (const auto& result : results) {
        int barLength = static_cast<int>((result.throughput / maxThroughput) * barWidth);
        std::cout << result.wifiType << ": " << std::string(barLength, '=') << " " << result.throughput << " Mbps\n";
    }

    std::cout << "\nAverage Latency (ms) Bar Chart:\n";
    for (const auto& result : results) {
        int barLength = static_cast<int>((result.avgLatency / maxAvgLatency) * barWidth);
        std::cout << result.wifiType << ": " << std::string(barLength, '=') << " " << result.avgLatency << " ms\n";
    }

    std::cout << "\nMaximum Latency (ms) Bar Chart:\n";
    for (const auto& result : results) {
        int barLength = static_cast<int>((result.maxLatency / maxMaxLatency) * barWidth);
        std::cout << result.wifiType << ": " << std::string(barLength, '=') << " " << result.maxLatency << " ms\n";
    }
}

// Function to log results to a log file
void WiFiSimulator::logResultsToFile(const std::vector<SimulationResult>& results) {
    std::ofstream logFile("simulation_log.txt", std::ios::app); // Open file in append mode

    if (logFile.is_open()) {
        logFile << "| Users | WiFi Type        | Throughput (Mbps) | Avg Latency (ms) | Max Latency (ms) |\n";
        logFile << "|-------|------------------|-------------------|------------------|------------------|\n";
        
        for (const auto& result : results) {
            logFile << "| " 
                    << std::setw(6) << result.numUsers << " | " 
                    << std::setw(16) << result.wifiType << " | "
                    << std::setw(17) << std::fixed << std::setprecision(4) << result.throughput << " | "
                    << std::setw(16) << std::fixed << std::setprecision(4) << result.avgLatency << " | "
                    << std::setw(16) << std::fixed << std::setprecision(4) << result.maxLatency << " |\n";
        }
        logFile.close(); // Close the file after writing
    } else {
        std::cerr << "Error opening log file!" << std::endl;
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

    // Print visualization
    printBarChart(results);

    // Log the results to a file
    logResultsToFile(results);
}
