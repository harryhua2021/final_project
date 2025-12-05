#ifndef PATIENT_H
#define PATIENT_H

#include <string>

struct Patient {
    std::string id;
    int triagePriority;    // 1 (most critical) to 5 (least critical)
    unsigned long arrivalTime; // sequence number
    std::string data;      // additional info (e.g., symptoms)

    Patient() = default;
    Patient(const std::string& _id, int _prio, unsigned long _time, const std::string& _data)
        : id(_id), triagePriority(_prio), arrivalTime(_time), data(_data) {}
};

// serialize patient to string for storage in Prority queue and hash table
std::string patientToString(const Patient& p);
Patient stringToPatient(const std::string& s);

#endif