#include "Patient.h"
#include <iostream>

Patient::Patient(int id, const std::string& name, int priority)
    : id(id), triagePriority(priority), name(name) {
    arrivalTime = std::chrono::steady_clock::now();
}

void Patient::print() const {
    auto now = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - arrivalTime).count();
    std::cout << "ID: " << id
              << " | Name: " << name
              << " | Priority: " << triagePriority
              << " | Wait: " << diff << "s\n";
}

bool PatientComparator::operator()(const Patient& a, const Patient& b) const {
    if (a.triagePriority != b.triagePriority) {
        return a.triagePriority > b.triagePriority; // min-heap: smaller priority number = higher urgency
    }
    return a.arrivalTime > b.arrivalTime; // earlier arrival wins
}
