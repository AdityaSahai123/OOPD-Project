#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <chrono>
#include <ctime>
#include "wifi_simulator.h"
using namespace std;
class BaseLogger {
protected:
    static string getCurrentTimestamp() {
        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
        return string(buffer);
    }

    virtual void writeLog(const string& timestamp, const string& data) = 0;
};

class TeeStream : public BaseLogger {
    ostream& console;
    ofstream& logFile;

protected:
    void writeLog(const string& timestamp, const string& data) override {
        console << "[" << timestamp << "] " << data;
        logFile << "[" << timestamp << "] " << data;
    }

public:
    TeeStream(ostream& console, ofstream& logFile) 
        : console(console), logFile(logFile) {}

    template<typename T>
    TeeStream& operator<<(const T& data) {
        auto timestamp = getCurrentTimestamp();
        writeLog(timestamp, to_string(data));
        return *this;
    }

    TeeStream& operator<<(const string& data) {
        auto timestamp = getCurrentTimestamp();
        writeLog(timestamp, data);
        return *this;
    }
    TeeStream& operator<<(const char* data) {
        auto timestamp = getCurrentTimestamp();
        writeLog(timestamp, string(data));
        return *this;
    }

    TeeStream& operator<<(ostream& (*manip)(ostream&)) {
        console << manip;
        logFile << manip;
        return *this;
    }
};


class MenuDisplay : public BaseLogger {
    TeeStream& output;

protected:
    void writeLog(const string& timestamp, const string& data) override {
        output << data;
    }

public:
    MenuDisplay(TeeStream& tee) : output(tee) {}

    void display() {
        output << "Select the number of users to simulate:\n";
        output << "1. 1 user\n";
        output << "2. 10 users\n";
        output << "3. 100 users\n";
        output << "4. Simulate all options\n";
        output << "Enter your choice: ";
    }
};

int main() {
    ofstream logFile("simulation_log.txt", ios::app);
    TeeStream output(cout, logFile);
    MenuDisplay menu(output);

    while (true) {
        menu.display();

        int choice;
        cin >> choice;

        if (choice < 1 || choice > 4) {
            output << "Invalid choice. Please try again.\n";
            continue;
        }

        vector<int> users;

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

        char runAgain;
        output << "Would you like to run another simulation? (y/n): ";
        cin >> runAgain;

        if (runAgain == 'n' || runAgain == 'N') {
            break;
        }
    }

    return 0;
}