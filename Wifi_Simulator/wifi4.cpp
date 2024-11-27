#include "wifi4.h"
#include "constants.h"
#include <random>
#include <algorithm>

WiFi4::WiFi4(int numUsers, std::shared_ptr<AccessPoint> ap)
    : numUsers(numUsers), ap(ap), averageLatency(0.0), maxLatency(0.0) {
    for (int i = 0; i < numUsers; ++i) {
        users.emplace_back(i);
    }
}

double WiFi4::calculateThroughput() {
    return simulateTransmission();  // Returns throughput in Mbps
}

double WiFi4::calculateAverageLatency() {
    return averageLatency;  // Return the calculated average latency
}

double WiFi4::calculateMaxLatency() {
    return maxLatency;  // Return the calculated max latency
}

double WiFi4::simulateTransmission() {
   
    
    double totalTime = 0.0;
    double totalData = 0.0;
    double maxLatencyObserved = 0.0;
    double totalLatencyObserved = 0.0;
    double baseThroughput = 20.0; 
    int successfulTransmissions = 0;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Backoff parameters
    const int MIN_CONTENTION_WINDOW = 16; // Initial minimum contention window
    const int MAX_CONTENTION_WINDOW = 1024; // Maximum contention window

    // User impact factor
    double userPenaltyFactor = 1.0 / std::pow(numUsers, 0.7);
    
    for (int i = 0; i < numUsers; ++i) {
        bool transmitted = false;
        double packetLatency = 0.0;
        int retries = 0;
        int currentContentionWindow = MIN_CONTENTION_WINDOW;

        while (!transmitted && retries < MAX_RETRIES) {
            // Check if channel is free
            if (!ap->getChannel()->isBusy()) {
                // Transmission time calculation
                double transmissionTime = (PACKET_SIZE * 8) / (BANDWIDTH_MHZ * 1e6 * CODING_RATE);
                
                // Simulate backoff before transmission
                std::uniform_int_distribution<> backoffDist(0, currentContentionWindow - 1);
                double backoffTime = backoffDist(gen) * 9;
                
                packetLatency += backoffTime;
                totalTime += backoffTime;

                // Transmission
                ap->getChannel()->setBusy(true);
                totalTime += transmissionTime;
                packetLatency += transmissionTime;
                ap->getChannel()->setBusy(false);

                totalData += PACKET_SIZE * 8;
                successfulTransmissions++;

                transmitted = true;
                totalLatencyObserved += packetLatency;
                maxLatencyObserved = std::max(maxLatencyObserved, packetLatency);

                // Reset contention window
                currentContentionWindow = MIN_CONTENTION_WINDOW;
            } else {
                // Exponential backoff
                std::uniform_int_distribution<> backoffDist(0, currentContentionWindow - 1);
                double backoffTime = backoffDist(gen) * 9;
                
                totalTime += backoffTime;
                packetLatency += backoffTime;
                retries++;

                // Exponentially increase contention window
                currentContentionWindow = std::min(currentContentionWindow * 2, MAX_CONTENTION_WINDOW);
            }
        }
    }

    // Calculate throughput and latency
    if (successfulTransmissions > 0) {
        averageLatency = totalLatencyObserved / successfulTransmissions;
        maxLatency = maxLatencyObserved;
    } else {
        averageLatency = maxLatency = 0.0;
    }

    // Throughput calculation with user penalty
    double adjustedThroughput = baseThroughput * userPenaltyFactor;
    
    // Contention penalty
    double contentionPenalty = 1.0 + (std::log(numUsers + 1) * 0.3);
    adjustedThroughput /= contentionPenalty;

    // Ensure minimum throughput
    return std::max(adjustedThroughput, 1.0);
}