#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <chrono>

class Patient {
public:
    int id;
    int triagePriority; // 1 (most critical) to 5 (least)
    std::chrono::steady_clock::time_point arrivalTime;
    std::string name;

    Patient(int id, const std::string& name, int priority);

    // For debugging
    void print() const;
};

// Custom comparator for min-heap: lower number = higher priority
// If priorities equal, earlier arrival goes first
struct PatientComparator {
    bool operator()(const Patient& a, const Patient& b) const;
};

#endif // PATIENT_H
