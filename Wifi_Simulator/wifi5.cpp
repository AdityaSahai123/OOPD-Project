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
    //round-robin queue
    for (int i = 0; i < numUsers; ++i) {
        transmissionQueue.push(i);
    }
}

int WiFi5::getNextUserInQueue() {
    int nextUser = transmissionQueue.front();
    transmissionQueue.pop();
    transmissionQueue.push(nextUser);  
    return nextUser;
}

double WiFi5::simulateBroadcastPhase() {
    double broadcastTime = (PACKET_SIZE * 8) / (BANDWIDTH_MHZ * 1e6); 
    return broadcastTime; 
}

double WiFi5::simulateCSIPhase() {

    double csiTimePerUser = (CSI_PACKET_SIZE * 8) / (BANDWIDTH_MHZ * 1e6); 
    return csiTimePerUser * numUsers; // Total CSI phase time
}

double WiFi5::simulateParallelTransmission() {
    return MIMO_PARALLEL_TIME_MS * 1e-3; 
}

double WiFi5::simulateTransmission() {
    double cycleTime = simulateBroadcastPhase() + simulateCSIPhase() + simulateParallelTransmission();
    return cycleTime; // Total cycle time in seconds
}

double WiFi5::calculateThroughput() {

    double userPenaltyFactor = 1.0 / std::pow(numUsers, 0.7);
    
    // Maximum number of concurrent streams
    int activeStreams = std::min(numUsers, 4);  
     double pen = 100.7; 

    double adjustedThroughput = pen 
                                 * userPenaltyFactor 
                                 * (activeStreams / 4.0);
    
    return std::max(adjustedThroughput, 1.0);
}

double WiFi5::calculateAverageLatency() {
    // Average latency includes:
    // 1. Broadcast time
    // 2. Average wait during CSI phase
    // 3. Parallel transmission time

    double broadcastTime = simulateBroadcastPhase();
    double csiTimePerUser = (CSI_PACKET_SIZE * 8) / (BANDWIDTH_MHZ * 1e6); // Time for one CSI packet
    double avgCsiWait = (numUsers - 1) * csiTimePerUser / 2;

    // Total average latency
    return (broadcastTime + avgCsiWait + MIMO_PARALLEL_TIME_MS * 1e-3) * 1e3; // Return in ms
}

double WiFi5::calculateMaxLatency() {
  
    // Includes:
    // 1. Broadcast time
    // 2. Full CSI phase time
    // 3. Parallel transmission time

    double broadcastTime = simulateBroadcastPhase();
    double totalCsiTime = simulateCSIPhase();

    // Total max latency
    return (broadcastTime + totalCsiTime + MIMO_PARALLEL_TIME_MS * 1e-3) * 1e3; // Return in ms
}
