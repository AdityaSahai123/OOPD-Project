#ifndef CONSTANTS_H
#define CONSTANTS_H

const double BANDWIDTH_MHZ = 20.0;
const int MAX_BACKOFF_TIME = 100;       // Maximum backoff time in milliseconds
const double MIMO_PARALLEL_TIME_MS = 15.0; // Time allowed for parallel communication in WiFi 5
const double OFDMA_PARALLEL_TIME_MS = 5.0; // Time allowed for parallel communication in WiFi 6
const double CODING_RATE = 5.0 / 6.0;
const int BITS_PER_SYMBOL = 8;         // For 256-QAM
const int PACKET_SIZE = 1024;          // Packet size in bytes (1 KB)
const int MAX_RETRIES = 5;             // Maximum number of retries
const int CSI_PACKET_SIZE = 200;       // Size of CSI packet in bytes (WiFi 5)
const int NUM_SUBCHANNELS = 10;        // Number of subchannels in WiFi 6 for OFDMA
const double GUARD_INTERVAL_MS = 0.0008;
#endif
