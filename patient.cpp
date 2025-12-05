#include "patient.h"
#include <sstream>

std::string patientToString(const Patient& p) {
    //creates an output stream seperated by "|" to be used in the file
    std::ostringstream oss;
    oss << p.id << "|" << p.triagePriority << "|" << p.arrivalTime << "|" << p.data;
    return oss.str();
}

// converts a string to a patient

Patient stringToPatient(const std::string& s) {
    if (s.empty()) return Patient();
    std::istringstream iss(s);
    std::string token;
    std::getline(iss, token, '|');
    std::string id = token;
    std::getline(iss, token, '|');
    int prio = std::stoi(token);
    std::getline(iss, token, '|');
    unsigned long time = std::stoul(token);
    std::string data;
    std::getline(iss, data, '\0'); // rest is data
    return Patient(id, prio, time, data);
}