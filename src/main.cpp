#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <sstream>
#include "Patient.h"

using PatientQueue = std::priority_queue<Patient, std::vector<Patient>, PatientComparator>;
using PatientRegistry = std::unordered_map<int, Patient>;

int main() {
    PatientQueue triageQueue;
    PatientRegistry registry;
    int nextId = 1;

    std::cout << "=== Emergency Room Triage MVP ===\n";
    std::cout << "Commands:\n";
    std::cout << "  intake <name> <priority(1-5)>  - Add patient\n";
    std::cout << "  next                           - Treat next patient\n";
    std::cout << "  lookup <id>                    - View patient info\n";
    std::cout << "  quit                           - Exit\n\n";

    std::string command;
    while (std::getline(std::cin, command)) {
        if (command.empty()) continue;

        std::istringstream iss(command);
        std::string op;
        iss >> op;

        if (op == "quit") {
            break;
        } else if (op == "intake") {
            std::string name;
            int priority;
            if (iss >> name >> priority && priority >= 1 && priority <= 5) {
                Patient p(nextId, name, priority);
                triageQueue.push(p);
                registry[nextId] = p;
                std::cout << "Admitted: ID " << nextId << " (" << name << ", P" << priority << ")\n";
                nextId++;
            } else {
                std::cout << "Usage: intake <name> <priority(1-5)>\n";
            }
        } else if (op == "next") {
            if (triageQueue.empty()) {
                std::cout << "No patients in queue.\n";
            } else {
                Patient p = triageQueue.top();
                triageQueue.pop();
                std::cout << "Treating: ";
                p.print();
                // Optional: remove from registry after treatment
                // registry.erase(p.id);
            }
        } else if (op == "lookup") {
            int id;
            if (iss >> id) {
                auto it = registry.find(id);
                if (it != registry.end()) {
                    it->second.print();
                } else {
                    std::cout << "Patient ID " << id << " not found.\n";
                }
            } else {
                std::cout << "Usage: lookup <id>\n";
            }
        } else {
            std::cout << "Unknown command. Try: intake, next, lookup, quit\n";
        }
    }

    return 0;
}
