/* Ali Saleem (23L-2638)
   Ammar Hassan (23L-2614)

    DataStructure_Project
    University Timetable Management System

   */

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// Structure to hold a single class (time, course, and slot number)
struct ClassNode {
    char* time;           // Class time 
    char* courseName;     // Course name 
    ClassNode* next;      // Next class in the list
    int slot;             // Slot number (0 to 7)

    ClassNode(const char* t, const char* c, int s) {
        time = new char[strlen(t) + 1];
        strcpy_s(time, strlen(t) + 1, t);
        courseName = new char[strlen(c) + 1];
        strcpy_s(courseName, strlen(c) + 1, c);
        next = 0;
        slot = s;
    }

    ~ClassNode() {
        delete[] time;
        delete[] courseName;
    }
};

// Structure to hold a room and its classes
struct RoomNode {
    char* roomName;       // Room name
    ClassNode* classes;   // List of classes in this room
    RoomNode* next;       // Next room in the list

    RoomNode(const char* name) {
        roomName = new char[strlen(name) + 1];
        strcpy_s(roomName, strlen(name) + 1, name);
        classes = 0;
        next = 0;
    }

    ~RoomNode() {
        delete[] roomName;
        ClassNode* current = classes;
        while (current) {
            ClassNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
};

class TTADT {
private:
    static const int DAYS = 6;    // 6 days in a week
    static const int SLOTS = 8;   // 8 time slots per day
    RoomNode** days;              // Array of room lists for each day

    // Make a time string for a slot 
    void getTimeForSlot(int slot, char* time) {

        const char* times[] = {
            "08:30-10:00", "10:00-11:30", "11:30-13:00", "13:00-14:30",
            "14:30-16:00", "16:00-17:30", "17:30-19:00", "19:00-20:30"
        };
        strcpy_s(time, strlen(times[slot]) + 1, times[slot]);
    }

    // Convert day name to number 
    int dayNameToIndex(const char* day) {
        const char* dayNames[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
        for (int i = 0; i < DAYS; i++) {
            if (strcmp(day, dayNames[i]) == 0) {
                return i;
            }
        }
        return -1; // Wrong day name
    }

    // Remove extra spaces from a string
    void trimWhitespace(char* str) {
        // Move to first non-space
        char* start = str;
        while (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r') {
            start++;
        }
        // Copy from start to str
        int len = strlen(start);
        for (int i = 0; i <= len; i++) {
            str[i] = start[i];
        }
        // Remove trailing spaces
        while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n' || str[len - 1] == '\r')) {
            str[len - 1] = '\0';
            len--;
        }
    }

    // Split a CSV line into day, room, and courses
    bool parseCSVLine(char* line, char* day, char* room, char courses[][100]) {
        char* token = line;
        char* next = strchr(token, ',');

        if (!next) return false;
        *next = '\0';
        trimWhitespace(token);

        if (strlen(token) == 0) return false;
        strcpy_s(day, strlen(token) + 1, token);
        token = next + 1;

        next = strchr(token, ',');

        if (!next) return false;
        *next = '\0';
        trimWhitespace(token);

        if (strlen(token) == 0) return false;
        strcpy_s(room, strlen(token) + 1, token);
        token = next + 1;

        int courseIndex = 0;
        while (token && courseIndex < SLOTS) {
            next = strchr(token, ',');
            if (next) *next = '\0';
            trimWhitespace(token);
            strcpy_s(courses[courseIndex], strlen(token) + 1, token);
            token = next ? next + 1 : 0;
            courseIndex++;
        }
        // Fill remaining slots with empty strings
        while (courseIndex < SLOTS) {
            courses[courseIndex][0] = '\0';
            courseIndex++;
        }
        return true;
    }

    // Find a room or make a new one
    RoomNode* findOrCreateRoom(RoomNode** head, const char* roomName) {
        RoomNode* current = *head;
        RoomNode* prev = 0;

        while (current) {
            if (strcmp(current->roomName, roomName) == 0) {
                return current;
            }
            prev = current;
            current = current->next;
        }

        RoomNode* newRoom = new RoomNode(roomName);
        if (prev) {
            prev->next = newRoom;
        }
        else {
            *head = newRoom;
        }
        return newRoom;
    }

public:
    TTADT() {
        days = new RoomNode * [DAYS];
        for (int i = 0; i < DAYS; i++) {
            days[i] = 0;
        }
    }

    ~TTADT() {
        for (int i = 0; i < DAYS; i++) {
            RoomNode* current = days[i];
            while (current) {
                RoomNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] days;
    }

    void load(char filename[]) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Can't open file " << filename << endl;
            return;
        }

        char line[1024];
        char day[20];
        char room[50];
        char courses[SLOTS][100];

        while (file.getline(line, 1024)) {
            if (strlen(line) == 0) continue;

            if (!parseCSVLine(line, day, room, courses)) {
                continue;
            }

            int dayIndex = dayNameToIndex(day);
            if (dayIndex < 0) {
                continue;
            }

            RoomNode* roomNode = findOrCreateRoom(&days[dayIndex], room);

            for (int slot = 0; slot < SLOTS; slot++) {
                if (strlen(courses[slot]) == 0) continue;

                char time[20];
                getTimeForSlot(slot, time);

                ClassNode* newNode = new ClassNode(time, courses[slot], slot);

                // Add class to the room's list, keep slots in order
                ClassNode** head = &roomNode->classes;
                ClassNode* current = *head;
                ClassNode* prev = 0;

                while (current && current->slot < slot) {
                    prev = current;
                    current = current->next;
                }

                newNode->next = current;
                if (prev) {
                    prev->next = newNode;
                }
                else {
                    *head = newNode;
                }
            }
        }

        file.close();
    }

    void print() {
        const char* dayNames[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
        for (int i = 0; i < DAYS; i++) {
            cout << "\n" << dayNames[i] << ":\n";
            bool hasClasses = false;
            RoomNode* room = days[i];
            while (room) {
                bool roomHasClasses = false;
                for (int slot = 0; slot < SLOTS; slot++) {
                    char time[20];
                    getTimeForSlot(slot, time);
                    ClassNode* classForSlot = 0;
                    for (ClassNode* temp = room->classes; temp; temp = temp->next) {
                        if (temp->slot == slot) {
                            classForSlot = temp;
                            break;
                        }
                    }
                    if (classForSlot && strlen(classForSlot->courseName) > 0) {
                        if (!roomHasClasses) {
                            cout << "  " << room->roomName << ":\n";
                            roomHasClasses = true;
                            hasClasses = true;
                        }
                        cout << "    " << classForSlot->time << ": " << classForSlot->courseName << "\n";
                    }
                    else {
                        if (!roomHasClasses) {
                            cout << "  " << room->roomName << ":\n";
                            roomHasClasses = true;
                            hasClasses = true;
                        }
                        cout << "    " << time << ": ----\n";
                    }
                }
                room = room->next;
            }
            if (!hasClasses) {
                cout << "  No classes scheduled.\n";
            }
        }
    }

    void printCourseInfo(const char* courseName) {
        cout << "Sections for course " << courseName << ":\n";
        bool found = false;
        const char* dayNames[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

        for (int day = 0; day < DAYS; day++) {
            RoomNode* room = days[day];
            while (room) {
                ClassNode* cls = room->classes;
                while (cls) {
                    if (strcmp(cls->courseName, courseName) == 0) {
                        found = true;
                        cout << "  - " << dayNames[day] << " " << cls->time
                            << " in " << room->roomName << "\n";
                    }
                    cls = cls->next;
                }
                room = room->next;
            }
        }

        if (!found) {
            cout << "No sections found for course " << courseName << "\n";
        }
    }

    void DetermineFreeClassrooms(const char* dayName, int timeSlot) {
        int dayIndex = dayNameToIndex(dayName);
        if (dayIndex < 0 || dayIndex >= DAYS || timeSlot < 0 || timeSlot >= SLOTS) {
            cout << "Invalid day or time slot\n";
            return;
        }

        cout << "Available classrooms on " << dayName << " at slot " << timeSlot << ":\n";
        bool foundFree = false;

        // collect all rooms that are occupied at this time
        RoomNode* occupiedRooms = nullptr;
        RoomNode* lastOccupied = nullptr;
        RoomNode* currentRoom = days[dayIndex];

        while (currentRoom) {
            ClassNode* currentClass = currentRoom->classes;
            while (currentClass) {
                if (currentClass->slot == timeSlot && strlen(currentClass->courseName) > 0) {
                    
                    RoomNode* newOccupied = new RoomNode(currentRoom->roomName);
                    if (!occupiedRooms) {
                        occupiedRooms = newOccupied;
                    }
                    else {
                        lastOccupied->next = newOccupied;
                    }
                    lastOccupied = newOccupied;
                    break;
                }
                currentClass = currentClass->next;
            }
            currentRoom = currentRoom->next;
        }

        // Now check all rooms and see which aren't in the occupied list
        currentRoom = days[dayIndex];
        while (currentRoom) {
            bool isOccupied = false;
            RoomNode* occupied = occupiedRooms;
            while (occupied) {
                if (strcmp(occupied->roomName, currentRoom->roomName) == 0) {
                    isOccupied = true;
                    break;
                }
                occupied = occupied->next;
            }

            if (!isOccupied) {
                cout << "  - " << currentRoom->roomName << "\n";
                foundFree = true;
            }
            currentRoom = currentRoom->next;
        }

        while (occupiedRooms) {
            RoomNode* temp = occupiedRooms;
            occupiedRooms = occupiedRooms->next;
            delete temp;
        }

        if (!foundFree) {
            cout << "No classrooms available at this time\n";
        }
    }

    void PrintTimetableOfSection(const char* section) {
        cout << "Timetable for section " << section << ":\n";
        const char* dayNames[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
        char times[SLOTS][20];

        // Precompute all time strings
        for (int slot = 0; slot < SLOTS; slot++) {
            getTimeForSlot(slot, times[slot]);
        }

        // Print header
        cout << "Day       ";
        for (int slot = 0; slot < SLOTS; slot++) {
            cout << "| " << times[slot] << " ";
        }
        cout << "\n";

        // For each day, check if the section has any class
        for (int day = 0; day < DAYS; day++) {
            cout << dayNames[day] << " ";
            if (strlen(dayNames[day]) < 8) cout << "\t"; 

            
            const char* slotEntries[SLOTS];
            for (int i = 0; i < SLOTS; i++) {
                slotEntries[i] = "----";
            }

            // Search all rooms for this section's classes
            RoomNode* room = days[day];
            while (room) {
                ClassNode* cls = room->classes;
                while (cls) {
                    if (strstr(cls->courseName, section) != nullptr) {
                        slotEntries[cls->slot] = room->roomName;
                    }
                    cls = cls->next;
                }
                room = room->next;
            }

            // Print the day's row
            for (int slot = 0; slot < SLOTS; slot++) {
                cout << "| " << slotEntries[slot] << " ";
                
                if (strlen(slotEntries[slot]) < 4) cout << "\t";
            }
            cout << "\n";
        }
    }
};

int main() {
    TTADT timetable;
    char filename[100] = "timetable.csv"; 
    int choice;

    cout << "University Timetable Management System\n";
    cout << "-------------------------------------\n";

    do {
        cout << "\nMain Menu:\n";
        cout << "1. Load Timetable from File\n";
        cout << "2. View Complete Timetable\n";
        cout << "3. Search Course Information\n";
        cout << "4. Find Available Classrooms\n";
        cout << "5. View Section Timetable\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
        case 1: {
            timetable.load(filename);
            cout << "Timetable loaded from " << filename << "\n";
            break;
        }
        case 2: {
            cout << "\nComplete Timetable:\n";
            timetable.print();
            break;
        }
        case 3: {
            char course[100];
            cout << "Enter course name to search (e.g. CS101): ";
            cin.getline(course, 100);
            timetable.printCourseInfo(course);
            break;
        }
        case 4: {
            char day[20];
            int slot;
            cout << "Enter day (e.g. Monday): ";
            cin.getline(day, 20);
            cout << "Enter time slot (0-7): ";
            cin >> slot;
            cin.ignore();
            timetable.DetermineFreeClassrooms(day, slot);
            break;
        }
        case 5: {
            char section[50];
            cout << "Enter section name (e.g. BCS-1A): ";
            cin.getline(section, 50);
            timetable.PrintTimetableOfSection(section);
            break;
        }
       
        case 0: {
            cout << "Exiting program...\n";
            break;
        }
        default: {
            cout << "Invalid choice! Please try again.\n";
            break;
        }
        }
    } while (choice != 0);

    return 0;
}