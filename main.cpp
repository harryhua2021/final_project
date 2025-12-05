#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "patient.h"

unsigned int djb2_hash(const std::string& key) {
    unsigned int hash = 5381;
    for (size_t i = 0; i < key.length(); i++) {
        char c = key[i];
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

unsigned int mod_bucket_func(unsigned int hashcode, unsigned int cap) {
    return hashcode % cap;
}

// ------ Min-Heap Priority Queue for Triage System --------

// max patient capacity
const int MAX_PATIENTS = 100;

// patientStr: serialized patient data (e.g. "John Doe, GCS 12, strong headache")
// priority: priority level (1 = highest, 5 = lowest)
// seq: Arrival order to break ties (lower = higher priority)
struct TriageQueue {
    struct Entry {
        std::string patientStr;
        int priority;           
        unsigned long seq;
    };
    std::vector<Entry> heap;
    unsigned long counter = 0;

    bool empty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }

    // lower priority number = higher urgency; tie-break by arrival order
    bool higherUrgency(const Entry& a, const Entry& b) {
        if (a.priority != b.priority) return a.priority < b.priority;
        return a.seq < b.seq;
    }

    // creat patient and append them to the end of the heap
    void push(const std::string& pStr, int prio) {
        if (heap.size() >= MAX_PATIENTS) {
            std::cout << "Warning: Queue is full!\n";
            return;
        }
        Entry e{pStr, prio, counter++};
        heap.push_back(e);
        int i = heap.size() - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (!higherUrgency(heap[i], heap[parent])) break;
            std::swap(heap[i], heap[parent]);
            i = parent;
        }
    }
    //bubble up the highest priority patient to the front of the queue
    std::string top() {
        return empty() ? "" : heap.front().patientStr;
    }
    // returns the highest priority patient and removes them from the queue
    std::string pop() {
        if (empty()) return "";
        std::string result = heap[0].patientStr;
        heap[0] = heap.back();
        heap.pop_back();
    // replace root with last element and bubble down
        int i = 0;
        while (true) {
            int left = 2 * i + 1, right = 2 * i + 2;
            int best = i;
            if (left < (int)heap.size() && higherUrgency(heap[left], heap[best])) best = left;
            if (right < (int)heap.size() && higherUrgency(heap[right], heap[best])) best = right;
            if (best == i) break;
            std::swap(heap[i], heap[best]);
            i = best;
        }
        return result;
    }
};

//------- Hash Table for Patient Data -------

// occupied = false (Empty)
// occupied = true, deleted = true (Deleted)
// occupied = true, deleted = false (Occupied)
struct PatientRegistry {
    static const int CAP = 101;
    struct Slot {
        std::string id;
        std::string data;
        bool occupied = false;
        bool deleted = false;
    };
    std::vector<Slot> table = std::vector<Slot>(CAP);

     bool set(const std::string& id, const std::string& data) {
        unsigned int hashcode = djb2_hash(id);

        // Use Linear Probing to find existing key or reusable slot
        int firstDeleted = -1;
        for (unsigned int i = 0; i < CAP; ++i) {
            unsigned int idx = mod_bucket_func(hashcode + i, CAP);
            Slot& slot = table[idx];

            if (!slot.occupied) {
                break;
            }
            if (!slot.deleted && slot.id == id) {
                // Update existing
                slot.data = data;
                return true;
            }
            if (slot.deleted && firstDeleted == -1) {
                firstDeleted = static_cast<int>(idx);
            }
        }

        // Insert into first available slot
        for (unsigned int i = 0; i < CAP; ++i) {
            unsigned int idx = mod_bucket_func(hashcode + i, CAP);
            Slot& slot = table[idx];

            if (!slot.occupied || slot.deleted) {
                slot.id = id;
                slot.data = data;
                slot.occupied = true;
                slot.deleted = false;
                return true;
            }
        }

        return false; // Table is full
    }


    // Patient lookup by ID
    std::string get(const std::string& id) {
        unsigned int hashcode = djb2_hash(id);
        for (unsigned int i = 0; i < CAP; ++i) {
            unsigned int idx = mod_bucket_func(hashcode + i, CAP);
            Slot& slot = table[idx];
            // Patient not found
            if (!slot.occupied) {
                break;
            }
            // Patient found
            if (!slot.deleted && slot.id == id) {
                return slot.data;
            }
        }
        // return patient record
        return "";
    }
};

// ---------- Helper Functions ----------
int readPriority() {
    int p;
    while (true) {
        std::cout << "Triage priority (1-5, 1 = most critical): ";
        // loops until user enters integer 1 - 5
        if (std::cin >> p && p >= 1 && p <= 5) {
            std::cin.ignore();
            return p;
        }
        // Fault handling for invalid input
        std::cout << "Error: Please enter a number from 1 to 5.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

// ---------- Main Program for the interactive interface ----------
int main() {
    TriageQueue pq;
    PatientRegistry registry;
    unsigned long arrivalSeq = 0;

    std::cout << "\nEmergency Room Triage System \n";
    std::cout << "===============================================\n";

    while (true) {
        // program menu
        std::cout << "\n1. Add patient\n2. Treat next patient\n3. View next patient\n4. Lookup patient\n5. Exit\n> ";
        // reads valid input (integer) and drops the invalid ones
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        std::cin.ignore();

        if (choice == 1) {
            std::string id = readLine("Patient ID: ");
            //falut handling for no id input
            if (id.empty()) {
                std::cout << "Error: ID cannot be empty.\n";
                continue;
            }
            // calls the helper function to read priority
            int prio = readPriority();
            std::string info = readLine("Patient info: ");
            Patient p(id, prio, arrivalSeq++, info);
            // converts patient data to string
            std::string serialized = patientToString(p);
            // stores the stringed patient data in hashtable
            registry.set(id, serialized);
            // adds patient to priority queue
            pq.push(serialized, prio);
            // prints confirmation
            std::cout << "Patient " << id << " added with priority " << prio << ".\n";
            
        } else if (choice == 2) {
            //dequeue the next most urgent patient
            std::string p = pq.pop();
            if (p.empty()) {
                std::cout << "No patients in queue.\n";
            } else {
                Patient pat = stringToPatient(p);
                std::cout << "Treated patient: " << pat.id << " (Priority " << pat.triagePriority << ")\n";
            }

        } else if (choice == 3) {
            // returns the next patient without dequeue
            std::string p = pq.top();
            if (p.empty()) {
                std::cout << "No patients waiting.\n";
            } else {
                Patient pat = stringToPatient(p);
                std::cout << "Next patient: " << pat.id << " | Priority " << pat.triagePriority
                          << " | Info: " << pat.data << "\n";
            }

        } else if (choice == 4) {
            // lookup a patient by id
            std::string id = readLine("Enter patient ID to lookup: ");
            std::string data = registry.get(id);
            if (data.empty()) {
                std::cout << "Patient not found.\n";
            } else {
                Patient p = stringToPatient(data);
                std::cout << "Record: " << p.id << " | Priority " << p.triagePriority
                          << " | Arrival order: " << p.arrivalTime << " | Info: " << p.data << "\n";
            }

        } else if (choice == 5) {
            // prints exit message and exits
            std::cout << "Exiting.\n";
            break;
        } else {
            std::cout << "Invalid option. Please choose 1-5.\n";
        }
    }
    return 0;
}