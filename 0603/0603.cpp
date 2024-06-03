//#include <iostream>
//#include <list>
//#include <deque>
//#include <memory>
//
//struct Process {
//    int pid; // 프로세스 ID
//    bool isForeground; // FG: true, BG: false
//    Process(int id, bool fg) : pid(id), isForeground(fg) {}
//};
//
//struct StackNode {
//    std::list<Process> processList;
//};
//
//using Stack = std::deque<StackNode>;
//
//
//
//Stack dynamicQueue;
//
//void initializeQueue() {
//    StackNode initialNode;
//    dynamicQueue.push_back(initialNode);
//}
//
//
//void enqueue(Process process) {
//    if (process.isForeground) {
//        dynamicQueue.back().processList.push_back(process);
//    }
//    else {
//        dynamicQueue.front().processList.push_back(process);
//    }
//}
//
//
//Process dequeue() {
//    if (dynamicQueue.empty()) {
//        throw std::runtime_error("Queue is empty");
//    }
//
//    StackNode& topNode = dynamicQueue.back();
//    Process process = topNode.processList.front();
//    topNode.processList.pop_front();
//
//    if (topNode.processList.empty()) {
//        dynamicQueue.pop_back();
//    }
//
//    return process;
//}
//
//
//void promote() {
//    if (dynamicQueue.size() < 2) return; // 프로모션할 노드가 없음
//
//    StackNode& topNode = dynamicQueue.back();
//    StackNode& nextNode = dynamicQueue[dynamicQueue.size() - 2];
//
//    nextNode.processList.splice(nextNode.processList.end(), topNode.processList);
//
//    if (topNode.processList.empty()) {
//        dynamicQueue.pop_back();
//    }
//}
//
//void split_n_merge(size_t threshold) {
//    std::deque<StackNode> tempQueue;
//
//    while (!dynamicQueue.empty()) {
//        StackNode& topNode = dynamicQueue.back();
//        if (topNode.processList.size() > threshold) {
//            auto splitPoint = std::next(topNode.processList.begin(), topNode.processList.size() / 2);
//            StackNode newNode;
//            newNode.processList.splice(newNode.processList.end(), topNode.processList, splitPoint, topNode.processList.end());
//            tempQueue.push_front(newNode);
//        }
//        tempQueue.push_front(topNode);
//        dynamicQueue.pop_back();
//    }
//
//    while (!tempQueue.empty()) {
//        dynamicQueue.push_back(tempQueue.front());
//        tempQueue.pop_front();
//    }
//}
//
//
//int main() {
//    initializeQueue();
//
//    Process p1(1, true);
//    Process p2(2, false);
//    Process p3(3, true);
//
//    enqueue(p1);
//    enqueue(p2);
//    enqueue(p3);
//
//    Process p = dequeue();
//    std::cout << "Dequeued Process: " << p.pid << std::endl;
//
//    promote();
//
//    split_n_merge(2);
//
//    return 0;
//}

//part 2222222

//#define _CRT_SECURE_NO_WARNINGS
//
//#include <iostream>
//#include <vector>
//#include <deque>
//#include <list>
//#include <thread>
//#include <chrono>
//#include <string>
//#include <sstream>
//#include <memory>
//#include <mutex>
//#include <cstring> // Include this header for strcpy_s
//
//struct Process {
//    int pid;
//    bool isForeground;
//    bool promoted;
//    int sleepTime;
//    Process(int id, bool fg) : pid(id), isForeground(fg), promoted(false), sleepTime(0) {}
//};
//
//struct StackNode {
//    std::list<Process> processList;
//};
//
//using Stack = std::deque<StackNode>;
//
//Stack dynamicQueue;
//std::list<Process> waitQueue;
//std::mutex mtx; // Declare the mutex
//int pidCounter = 0;
//
//void initializeQueue() {
//    StackNode initialNode;
//    dynamicQueue.push_back(initialNode);
//}
//
//void enqueue(Process process) {
//    std::lock_guard<std::mutex> lock(mtx); // Use std::lock_guard to lock the mutex
//    if (process.isForeground) {
//        dynamicQueue.back().processList.push_back(process);
//    }
//    else {
//        dynamicQueue.front().processList.push_back(process);
//    }
//}
//
//Process dequeue() {
//    std::lock_guard<std::mutex> lock(mtx); // Use std::lock_guard to lock the mutex
//    if (dynamicQueue.empty()) {
//        throw std::runtime_error("Queue is empty");
//    }
//
//    StackNode& topNode = dynamicQueue.back();
//    Process process = topNode.processList.front();
//    topNode.processList.pop_front();
//
//    if (topNode.processList.empty()) {
//        dynamicQueue.pop_back();
//    }
//
//    return process;
//}
//
//void promote() {
//    std::lock_guard<std::mutex> lock(mtx); // Use std::lock_guard to lock the mutex
//    if (dynamicQueue.size() < 2) return;
//
//    StackNode& topNode = dynamicQueue.back();
//    StackNode& nextNode = dynamicQueue[dynamicQueue.size() - 2];
//
//    nextNode.processList.splice(nextNode.processList.end(), topNode.processList);
//
//    if (topNode.processList.empty()) {
//        dynamicQueue.pop_back();
//    }
//}
//
//void split_n_merge(size_t threshold) {
//    std::lock_guard<std::mutex> lock(mtx); // Use std::lock_guard to lock the mutex
//    std::deque<StackNode> tempQueue;
//
//    while (!dynamicQueue.empty()) {
//        StackNode& topNode = dynamicQueue.back();
//        if (topNode.processList.size() > threshold) {
//            auto splitPoint = std::next(topNode.processList.begin(), topNode.processList.size() / 2);
//            StackNode newNode;
//            newNode.processList.splice(newNode.processList.end(), topNode.processList, splitPoint, topNode.processList.end());
//            tempQueue.push_front(newNode);
//        }
//        tempQueue.push_front(topNode);
//        dynamicQueue.pop_back();
//    }
//
//    while (!tempQueue.empty()) {
//        dynamicQueue.push_back(tempQueue.front());
//        tempQueue.pop_front();
//    }
//}
//
//void monitor(int interval) {
//    while (true) {
//        std::this_thread::sleep_for(std::chrono::seconds(interval));
//        std::lock_guard<std::mutex> lock(mtx); // Use std::lock_guard to lock the mutex
//        std::cout << "Running: []" << std::endl;
//        std::cout << "---------------------------" << std::endl;
//        std::cout << "DQ: ";
//        for (auto& node : dynamicQueue) {
//            std::cout << "[";
//            for (auto& process : node.processList) {
//                std::cout << process.pid << (process.isForeground ? "F" : "B") << " ";
//            }
//            std::cout << "] ";
//        }
//        std::cout << "(bottom/top)" << std::endl;
//        std::cout << "---------------------------" << std::endl;
//        std::cout << "WQ: ";
//        for (auto& process : waitQueue) {
//            std::cout << "[" << process.pid << (process.isForeground ? "F" : "B") << ":" << process.sleepTime << "] ";
//        }
//        std::cout << std::endl;
//    }
//}
//
//void shell(int interval) {
//    while (true) {
//        // Shell command execution simulation
//        std::this_thread::sleep_for(std::chrono::seconds(interval));
//        std::lock_guard<std::mutex> lock(mtx); // Use std::lock_guard to lock the mutex
//        std::cout << "Shell: Executing command..." << std::endl;
//        // Simulate command execution
//        Process process(pidCounter++, true);
//        enqueue(process);
//    }
//}
//
//char** parse(const char* command) {
//    std::istringstream iss(command);
//    std::vector<std::string> tokens;
//    std::string token;
//    while (iss >> token) {
//        tokens.push_back(token);
//    }
//    char** args = new char* [tokens.size() + 1];
//    for (size_t i = 0; i < tokens.size(); ++i) {
//        args[i] = new char[tokens[i].size() + 1];
//        strcpy_s(args[i], tokens[i].size() + 1, tokens[i].c_str()); // Use strcpy_s for copying strings
//    }
//    args[tokens.size()] = new char[1];
//    args[tokens.size()][0] = '\0';
//    return args;
//}
//
//void exec(char** args) {
//    // Placeholder for actual command execution
//    std::cout << "Executing: ";
//    for (int i = 0; args[i] != nullptr && args[i][0] != '\0'; ++i) {
//        std::cout << args[i] << " ";
//    }
//    std::cout << std::endl;
//
//    // Free the allocated memory
//    for (int i = 0; args[i] != nullptr; ++i) {
//        delete[] args[i];
//    }
//    delete[] args;
//}
//
//int main() {
//    try {
//        initializeQueue();
//
//        std::thread monitorThread(monitor, 5);
//        std::thread shellThread(shell, 10);
//
//        monitorThread.join();
//        shellThread.join();
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//        return 1;
//    }
//    catch (...) {
//        std::cerr << "Unknown exception occurred" << std::endl;
//        return 2;
//    }
//
//    return 0;
//}


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

void readCommands(std::vector<std::string>& commands) {
    std::ifstream infile("commands.txt");
    std::string line;
    while (std::getline(infile, line)) {
        commands.push_back(line);
    }
}

#include <sstream>

std::vector<std::string> parseCommand(const std::string& command) {
    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void executeEcho(const std::vector<std::string>& args) {
    for (size_t i = 1; i < args.size(); ++i) {
        std::cout << args[i] << " ";
    }
    std::cout << std::endl;
}

// Dummy, gcd, prime, and sum functions need to be implemented similarly
// Assume these functions exist and handle the respective commands

void executeCommand(const std::string& command, bool isForeground) {
    auto tokens = parseCommand(command);
    if (tokens.empty()) return;

    std::string cmd = tokens[0];
    if (cmd == "echo") {
        executeEcho(tokens);
    }
    // Handle other commands similarly

    if (isForeground) {
        // Wait for the command to complete if it's a foreground process
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else {
        // Background processes run independently
        std::thread([tokens]() {
            executeEcho(tokens);
            }).detach();
    }
}

void monitor() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        // Display system state, such as running processes
        std::cout << "Monitor: Displaying system state..." << std::endl;
    }
}


int main() {
    std::vector<std::string> commands;
    readCommands(commands);

    std::thread monitorThread(monitor);
    monitorThread.detach();

    for (const auto& command : commands) {
        bool isForeground = command.find('&') == std::string::npos;
        std::string actualCommand = command;
        if (!isForeground) {
            actualCommand.erase(std::remove(actualCommand.begin(), actualCommand.end(), '&'), actualCommand.end());
        }
        executeCommand(actualCommand, isForeground);
        std::this_thread::sleep_for(std::chrono::seconds(2));  // Simulate Y-second interval
    }

    return 0;
}

