#include "wifi6.h"
#include "constants.h"
#include <cmath>
#include <algorithm>
#include <iostream> 

WiFi6::WiFi6(int numUsers, int numSubchannels, double bandwidthMHz, int packetSize)
    : numUsers(numUsers), numSubchannels(numSubchannels), bandwidthMHz(bandwidthMHz), packetSize(packetSize) {

    for (int i = 0; i < numUsers; ++i) {
        users.emplace_back(i); 
    }

    if (numSubchannels > bandwidthMHz / 2) {
        std::cerr << "Warning: Number of subchannels exceeds practical limit. Adjusting to max possible." << std::endl;
        numSubchannels = static_cast<int>(bandwidthMHz / 2);
    }
}

double WiFi6::simulateTransmission() {
    double totalTime = 0.0;

    // CSI phase-200B
    totalTime += (CSI_PACKET_SIZE * 8 * numUsers) / (bandwidthMHz * 1e6);  
    
    //round robin
    int rounds = static_cast<int>(std::ceil((double)numUsers / numSubchannels));
    totalTime += rounds * OFDMA_PARALLEL_TIME_MS * 1e-3;  

    totalTime += rounds * GUARD_INTERVAL_MS * 1e-3;  

    return totalTime;  // Return total time spent
}

double WiFi6::calculateThroughput() {
    double subchannelBandwidth = bandwidthMHz / numSubchannels;  // Bandwidth per subchannel
    double baseThroughput = subchannelBandwidth * BITS_PER_SYMBOL * CODING_RATE;  // Per subchannel

    // Adjust throughput based on contention (heuristic model)
    double userPenalty = std::pow((double)numUsers / numSubchannels, 1.3);  
    baseThroughput /= userPenalty;

    // Max throughput can't exceed total channel capacity
    double maxThroughput = bandwidthMHz * BITS_PER_SYMBOL * CODING_RATE;
    baseThroughput = std::min(baseThroughput * numSubchannels, maxThroughput);

    return std::max(baseThroughput, 1.0001);  // Ensure minimum throughput is non-zero
}

double WiFi6::calculateAverageLatency() {
    double baseLatency = OFDMA_PARALLEL_TIME_MS;  // Base OFDMA time

    //simulating delays
    double contentionFactor = ((double)numUsers / numSubchannels) * 1.2;  
    return baseLatency + contentionFactor * 1.5;  
}

double WiFi6::calculateMaxLatency() {
    double baseLatency = OFDMA_PARALLEL_TIME_MS; 
    double maxLatencyFactor = ((double)numUsers / numSubchannels) * 2.0;  
    return baseLatency + maxLatencyFactor * 2.5;  
}
