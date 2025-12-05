#### Final project - Emergency Room Triage System ####

## Introduction ##
In emergency medicine, patients must be treated according to clinical urgency—not the order in which they arrive. This project implements a realistic Emergency Room Triage System using two core data structures: a custom min-heap priority queue that orders patients by triage level (1 = most critical to 5 = least urgent), with arrival time used as a tiebreaker, and a hash table-based patient registry enabling O(1) average-time lookups for status checks and updates. This mini-app is designed as a minimally viable product (MVP), its modular design supports future expansions such as an overflow transport queue, bed capacity monitoring, patient history tracking, or other integration with hospital information systems.

## Features ##
- Interactive menu to add, treat, view, and lookup patients.
- Automatic prioritization: Priority 1 (critical) > 2 > 3 > 4 > 5 (non-urgent)
- Tie-breaking by arrival order for same priority patients.
- Input validation and error recovery.
- Persistent patient records. 

## How the Data Structure Works ##

/ Priority Queue (Treatment priority using Min-heap) /
- Logic: Stored as a std:: vector where each node at index i has children at 2*i+1 and 2*i+2.
- Ordering Lower triage number  =  higher urgency, Equal priorities use earlier arrival time.
- Operation: Patient insertion and Deletion are both O(log n), patient extraction is O(log n). Patient lookup is O(1).
/ Hash Table (Patient Registry) /
- Logic: Fixed-size array with 101 slots using DJB2 hash + linear probing.
- Collision handling: Probes hash, hash+1, hash+2, ... modulo table size.
- Deletion: Uses to lazy deletion to preserve probe sequences.

## File Structure ## 
er-triage/
|-- CMakeLists.txt   # Build configuration
|-- main.cpp         # Main application
|-- patient.cpp      # serialization/deserialization logic
|-- patient.h        # Patient Struct + function declarations

Installation / Setup
1. Clone the project directory with the file structure above.
2. Build with CMake with the following commands:
    mkdir build
    cd build
    cmake ..
    make
3. Run the application:
    ./ertriage

## Usage ##
After running ./ertriage, use the interactive menu:
1. Add patient
2. Treat next patient
3. View next patient
4. Lookup patient by ID
5. Exit

Example Workflow:
> 1
Patient ID: 101
Triage Priority: 1
Patient info: Unresponsive, no pulse

> 1
Patient ID: 102
Triage Priority: 2
Patient info: responsive, chest pain

> 2
Treated patient ID: 101 (Priority 1)

> 3
Next patient: P101 | Priority 1 | Info: Unresponsive, no pulse

> 4
Enter patient ID to lookup: P101
Record: P101 | Priority 1 | Arrival order: 0 | Info: Unresponsive, no pulse

## Application Demo ##
This system mirrors real ER protocols in the following ways:

- A Priority 1 (cardiac arrest) patient jumps ahead of a Priority 4 (arm fracture), even if they arrived later.
- The hash table allows instant access to any patient’s full record—critical for updating status or handoffs.
- The min-heap ensures the most critical patient is always treated next, reducing preventable deaths.
- It proves that simple, well-chosen data structures can model life-or-death decision logic efficiently—even on limited hardware.

## Testing ## 
/ Manual scenario testing: /
Mixed priority arrivals (e.g., P1 after P2 → P1 treated first)
Tie-breaking (same priority → earlier arrival wins)
Lookup of treated patients
Invalid input recovery
Edge cases:
Empty queue/registry
Full queue (100 patients)
Empty ID or out-of-range priority
To test yourself:

Run the app
Try the example workflow above
Verify ordering with this sequence:
Add P201 (Prio 2) → Add P202 (Prio 1) → "View next" should show P202
## References ## 
https://github.com/cu-cspb2270-2025/hw-05-priority-queue-harryhua2021 <hw-05-prioirty-queue>
https://github.com/cu-cspb2270-2025/hw-06-hash-tables-harryhua2021 <hw-06-hash-tables>
https://www.scribd.com/document/566740235/Hospital-management
https://www.geeksforgeeks.org/cpp/hospital-management-system-in-c/

## Future Improvements ## 
Add dynamic reprioritization (e.g., downgrade patients that are stablized.)
Implement transport queue for overflow handling
Add bed capacity tracking with BST
Save/load patient data to file

