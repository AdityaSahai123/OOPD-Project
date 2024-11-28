#include "wifi6.h"
#include "constants.h"
#include <cmath>
#include <algorithm>
#include <iostream> 
using namespace std;
WiFi6::WiFi6(int numUsers, int numSubchannels, double bandwidthMHz, int packetSize)
    : numUsers(numUsers), numSubchannels(numSubchannels), bandwidthMHz(bandwidthMHz), packetSize(packetSize) {

    for (int i = 0; i < numUsers; ++i) {
        users.emplace_back(i); 
    }

    if (numSubchannels > bandwidthMHz / 2) {
        cerr << "Warning: Number of subchannels exceeds practical limit. Adjusting to max possible." << endl;
        numSubchannels = static_cast<int>(bandwidthMHz / 2);
    }
}

double WiFi6::simulateTransmission() {
    double totalTime = 0.0;

    // CSI phase-200B
    totalTime += (CSI_PACKET_SIZE * 8 * numUsers) / (bandwidthMHz * 1e6);  
    
    //round robin
    int rounds = static_cast<int>(ceil((double)numUsers / numSubchannels));
    totalTime += rounds * OFDMA_PARALLEL_TIME_MS * 1e-3;  

    totalTime += rounds * GUARD_INTERVAL_MS * 1e-3;  

    return totalTime;  // Return total time spent
}

double WiFi6::calculateThroughput() {
    double subchannelBandwidth = bandwidthMHz / numSubchannels;
    double baseThroughput = subchannelBandwidth * BITS_PER_SYMBOL * CODING_RATE;
    
    double usersPerSubchannel = static_cast<double>(numUsers) / numSubchannels;
    double userPenalty = pow(usersPerSubchannel, 0.9); 
    
    double adjustedThroughput = baseThroughput / userPenalty;
    
    double maxThroughput = bandwidthMHz * BITS_PER_SYMBOL * CODING_RATE;
    double finalThroughput = min(adjustedThroughput * numSubchannels, maxThroughput);
    finalThroughput *= 0.8;  
    
    double randomFactor = 0.95 + (static_cast<double>(rand()) / RAND_MAX) * 0.1;  // Random between 0.95 and 1.05
    finalThroughput *= randomFactor;
    
    return max(finalThroughput, 1.0001);
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
