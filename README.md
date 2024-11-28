# OOPD PROJECT: WiFi Simulator
# Aditya Sahai |24009
## Project Overview

This WiFi Simulator is a comprehensive simulation of WiFi communication technologies, demonstrating the evolution from WiFi 4 to WiFi 6 through object-oriented programming principles.

## Problem Statement

The project simulates WiFi communication across three key WiFi standards:
1. **WiFi 4**: Single access point communication with channel sniffing and backoff mechanisms
2. **WiFi 5**: Introduction of multi-user MIMO with parallel transmissions
3. **WiFi 6**: OFDMA-based communication with channel subdivision

### Simulation Parameters
- Bandwidth: 20 MHz
- Modulation: 256-QAM
- Coding Rate: 5/6
- Packet Size: 1 KB

## Simulation Scenarios

The simulator evaluates performance for different user configurations:
- 1 user, 1 access point
- 10 users, 1 access point
- 100 users, 1 access point

## Performance Metrics

The simulation calculates:
- Throughput
- Average Latency
- Maximum Latency

### Example Results

| Users | WiFi Type        | Throughput (Mbps) | Avg Latency (ms) | Max Latency (ms) |
|-------|------------------|-------------------|------------------|------------------|
|     1 |           WiFi 4 |           16.9341 |          36.0005 |          36.0005 |
|     1 |           WiFi 5 |           25.1750 |          15.4096 |          15.4896 |
|     1 |   WiFi 6 (10 SC) |          110.2953 |           5.1800 |           5.5000 |
|     1 |    WiFi 6 (5 SC) |          105.5401 |           5.3600 |           6.0000 |
|     1 |    WiFi 6 (2 SC) |          109.6864 |           5.9000 |           7.5000 |

## Additional Features
- Bar graph visualization
- Detailed log file generation

## Build and Run Instructions

### Build the Project
```bash
make
```

### Run the Simulator
```bash
./wifi_simulator
```


