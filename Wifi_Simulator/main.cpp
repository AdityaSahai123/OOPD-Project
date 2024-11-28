#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <chrono>
#include <ctime>
#include "wifi_simulator.h"

class TeeStream {
    std::ostream& console;
    std::ofstream& logFile;
public:
    TeeStream(std::ostream& console, std::ofstream& logFile) 
        : console(console), logFile(logFile) {}

    template<typename T>
    TeeStream& operator<<(const T& data) {
        auto timestamp = getCurrentTimestamp();
        console << "[" << timestamp << "] " << data;
        logFile << "[" << timestamp << "] " << data;
        return *this;
    }

    // Handle endl and other manipulators
    TeeStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
        console << manip;
        logFile << manip;
        return *this;
    }

    static std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
        return std::string(buffer);
    }
};

void displayMenu(TeeStream& output) {
    output << "Select the number of users to simulate:\n";
    output << "1. 1 user\n";
    output << "2. 10 users\n";
    output << "3. 100 users\n";
    output << "4. Simulate all options\n";
    output << "Enter your choice: ";
}

int main() {
    std::ofstream logFile("simulation_log.txt", std::ios::app);
    TeeStream output(std::cout, logFile);

    while (true) {
        displayMenu(output);

        int choice;
        std::cin >> choice;

        if (choice < 1 || choice > 4) {
            output << "Invalid choice. Please try again.\n";
            continue;
        }

        std::vector<int> users;

        switch (choice) {
            case 1:
                users.push_back(1);
                break;
            case 2:
                users.push_back(10);
                break;
            case 3:
                users.push_back(100);
                break;
            case 4:
                users = {1, 10, 100};
                break;
        }

        for (int numUsers : users) {
            output << "\nSimulating with " << numUsers << " user(s):\n";
            WiFiSimulator simulator(numUsers);
            simulator.runSimulation();
        }

        // Ask if user wants to run another simulation
        char runAgain;
        output << "Would you like to run another simulation? (y/n): ";
        std::cin >> runAgain;

        if (runAgain == 'n' || runAgain == 'N') {
            break;
        }
    }

    return 0;
}
