//added functionality
#include "wifi4.h"
#include "constants.h"
#include <random>
#include <algorithm>
using namespace std;
WiFi4::WiFi4(int numUsers, shared_ptr<AccessPoint> ap)
    : numUsers(numUsers), ap(ap), averageLatency(0.0), maxLatency(0.0) {
    for (int i = 0; i < numUsers; ++i) {
        users.emplace_back(i);
    }
}

double WiFi4::calculateThroughput() {
    return simulateTransmission();  
}

double WiFi4::calculateAverageLatency() {
    return averageLatency;
}

double WiFi4::calculateMaxLatency() {
    return maxLatency;  
}

double WiFi4::simulateTransmission() {
    double totalTime = 0.0;
    double totalData = 0.0;
    double maxLatencyObserved = 0.0;
    double totalLatencyObserved = 0.0;
    double pen = 20.0;  
    int successfulTransmissions = 0;


    random_device rd;
    mt19937 gen(rd());
    
   
    uniform_real_distribution<> randomVariation(0.95, 1.05); 
    double randomFactor = randomVariation(gen);  
    pen *= randomFactor; 
    
    // Backoff
    const int MIN_CONTENTION_WINDOW = 16;
    const int MAX_CONTENTION_WINDOW = 1024;

    // User impact factor
    double userPenaltyFactor = 1.0 / pow(numUsers, 0.7);
    
    for (int i = 0; i < numUsers; ++i) {
        bool transmitted = false;
        double packetLatency = 0.0;
        int retries = 0;
        int currentContentionWindow = MIN_CONTENTION_WINDOW;

        while (!transmitted && retries < MAX_RETRIES) {
            //if channel is free
            if (!ap->getChannel()->isBusy()) {
                
                double transmissionTime = (PACKET_SIZE * 8) / (BANDWIDTH_MHZ * 1e6 * CODING_RATE);
                uniform_int_distribution<> backoffDist(0, currentContentionWindow - 1);
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
                maxLatencyObserved = max(maxLatencyObserved, packetLatency);
                currentContentionWindow = MIN_CONTENTION_WINDOW;
            } else {
                // Exponential backoff
                uniform_int_distribution<> backoffDist(0, currentContentionWindow - 1);
                double backoffTime = backoffDist(gen) * 9;
                
                totalTime += backoffTime;
                packetLatency += backoffTime;
                retries++;

                // increase contention window
                currentContentionWindow = min(currentContentionWindow * 2, MAX_CONTENTION_WINDOW);
            }
        }
    }

    if (successfulTransmissions > 0) {
        averageLatency = totalLatencyObserved / successfulTransmissions;
        maxLatency = maxLatencyObserved;
    } else {
        averageLatency = maxLatency = 0.0;
    }
    double adjustedThroughput = pen * userPenaltyFactor;
    
    double contentionPenalty = 1.0 + (log(numUsers + 1) * 0.3);
    adjustedThroughput /= contentionPenalty;


    return max(adjustedThroughput, 1.0);
}
