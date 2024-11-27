#include "wifi5.h"
#include "constants.h"
#include <algorithm>
#include<math.h>
WiFi5::WiFi5(int numUsers, std::shared_ptr<AccessPoint> ap) : numUsers(numUsers), ap(ap) {
    for (int i = 0; i < numUsers; ++i) {
        users.emplace_back(i);
    }
    initializeRoundRobin();
}

void WiFi5::initializeRoundRobin() {
    // Initialize round-robin queue
    for (int i = 0; i < numUsers; ++i) {
        transmissionQueue.push(i);
    }
}

int WiFi5::getNextUserInQueue() {
    int nextUser = transmissionQueue.front();
    transmissionQueue.pop();
    transmissionQueue.push(nextUser);  // Put back at end for round-robin
    return nextUser;
}

double WiFi5::simulateBroadcastPhase() {
    // Time for AP to send a broadcast packet
    double broadcastTime = (PACKET_SIZE * 8) / (BANDWIDTH_MHZ * 1e6); // Convert bandwidth to bps
    return broadcastTime; // Return time in seconds
}

double WiFi5::simulateCSIPhase() {
    // Each user sequentially sends a 200-byte CSI packet
    double csiTimePerUser = (CSI_PACKET_SIZE * 8) / (BANDWIDTH_MHZ * 1e6); // Time per CSI packet
    return csiTimePerUser * numUsers; // Total CSI phase time
}

double WiFi5::simulateParallelTransmission() {
    // All users transmit in parallel for 15ms
    return MIMO_PARALLEL_TIME_MS * 1e-3; // Convert ms to seconds
}

double WiFi5::simulateTransmission() {
    // One complete transmission cycle:
    double cycleTime = simulateBroadcastPhase() + simulateCSIPhase() + simulateParallelTransmission();
    return cycleTime; // Total cycle time in seconds
}

double WiFi5::calculateThroughput() {
    // Base throughput for WiFi 5 (802.11ac)
   
    
    // Introduce a performance degradation factor
    // As users increase, throughput should decrease
    double userPenaltyFactor = 1.0 / std::pow(numUsers, 0.7);
    
    // Maximum number of concurrent streams
    int activeStreams = std::min(numUsers, 4);  // Max 4 streams typical for WiFi 5
     double baseThroughput = 100.7; 

    double adjustedThroughput = baseThroughput 
                                 * userPenaltyFactor 
                                 * (activeStreams / 4.0);
    
    // Ensure throughput doesn't go negative
    return std::max(adjustedThroughput, 1.0);
}

double WiFi5::calculateAverageLatency() {
    // Average latency includes:
    // 1. Broadcast time
    // 2. Average wait during CSI phase
    // 3. Parallel transmission time

    double broadcastTime = simulateBroadcastPhase();
    double csiTimePerUser = (CSI_PACKET_SIZE * 8) / (BANDWIDTH_MHZ * 1e6); // Time for one CSI packet

    // Average wait time during CSI phase (round-robin scheduling)
    double avgCsiWait = (numUsers - 1) * csiTimePerUser / 2;

    // Total average latency
    return (broadcastTime + avgCsiWait + MIMO_PARALLEL_TIME_MS * 1e-3) * 1e3; // Return in ms
}

double WiFi5::calculateMaxLatency() {
    // Maximum latency occurs when a user is last in the round-robin queue
    // Includes:
    // 1. Broadcast time
    // 2. Full CSI phase time
    // 3. Parallel transmission time

    double broadcastTime = simulateBroadcastPhase();
    double totalCsiTime = simulateCSIPhase();

    // Total max latency
    return (broadcastTime + totalCsiTime + MIMO_PARALLEL_TIME_MS * 1e-3) * 1e3; // Return in ms
}
