#include "wifi6.h"
#include "constants.h"
#include <cmath>
#include <algorithm>

WiFi6::WiFi6(int numUsers, int numSubchannels, double bandwidthMHz, int packetSize)
    : numUsers(numUsers), numSubchannels(numSubchannels), bandwidthMHz(bandwidthMHz), packetSize(packetSize) {

    for (int i = 0; i < numUsers; ++i) {
        users.emplace_back(i);  // Adding each user to the vector
    }
}

double WiFi6::simulateTransmission() {
    double totalTime = 0.0;

    // CSI phase (200 bytes per user)
    totalTime += (CSI_PACKET_SIZE * 8 * numUsers) / (bandwidthMHz * 1e6);  
    // Simulating round-robin OFDMA scheduling
    int rounds = static_cast<int>(std::ceil((double)numUsers / numSubchannels));
    totalTime += rounds * OFDMA_PARALLEL_TIME_MS * 1e-3;  // Time in seconds

    return totalTime;  // Return total time spent on transmission
}

double WiFi6::calculateThroughput() {
    double subchannelBandwidth = bandwidthMHz / numSubchannels;  // Bandwidth per subchannel
    double baseThroughput = subchannelBandwidth * BITS_PER_SYMBOL * CODING_RATE;  // Per subchannel

    
    double userPenalty = std::pow((double)numUsers / numSubchannels, 1.3);  // Softer penalty
    baseThroughput /= userPenalty;

    // Max throughput can't exceed total channel capacity
    double maxThroughput = bandwidthMHz * BITS_PER_SYMBOL * CODING_RATE;
    baseThroughput = std::min(baseThroughput * numSubchannels, maxThroughput);

    return std::max(baseThroughput, 1.0001);
}

double WiFi6::calculateAverageLatency() {
    double baseLatency = OFDMA_PARALLEL_TIME_MS;  // Base OFDMA time
    double contentionFactor = ((double)numUsers / numSubchannels) * 1.2;  // Reduced contention multiplier
    return baseLatency + contentionFactor * 1.5;  // ms
}

double WiFi6::calculateMaxLatency() {
    double baseLatency = OFDMA_PARALLEL_TIME_MS;  
    double maxLatencyFactor = ((double)numUsers / numSubchannels) * 2.0;  // Account for delays
    return baseLatency + maxLatencyFactor * 2.5;  // ms
}
