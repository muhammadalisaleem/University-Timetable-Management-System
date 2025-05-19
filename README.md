## University Timetable Management System

---

## Project Overview

The **University Timetable Management System** is a robust C++ application designed to efficiently store, manage, and query academic schedules for multiple rooms and days using advanced data structures. The system demonstrates a strong command over fundamental Data Structures and Algorithms (DSA) concepts, particularly linked lists and dynamic memory management.

The primary objective of this project is to build a scalable timetable manager that can handle complex queries such as searching by course, finding available classrooms, and displaying complete schedules, all while optimizing memory usage and maintaining code modularity.

---

## Motivation and Significance

University timetable management is a classical problem involving multiple entities such as courses, rooms, time slots, and days. Efficiently storing and accessing this information requires a carefully chosen data structure to ensure quick query times and easy updates.

This project uses a **linked list-based hierarchical data structure** which closely simulates real-world relationships between days, rooms, and classes. It avoids the overhead and inflexibility of fixed-size arrays and highlights dynamic memory allocation, pointer manipulation, and the importance of modular design in DSA.

---

## Core Data Structures and Design

### 1. **ClassNode (Singly Linked List)**  
Each `ClassNode` represents an individual class scheduled at a particular time slot in a room on a given day. It stores:

- **Time Slot**: A fixed interval string (e.g., "08:30 - 10:00")  
- **Course Name**: The course scheduled in that slot  
- **Slot Number**: Numeric representation of the time slot (1-8)  
- **Next Pointer**: Points to the next `ClassNode` in the linked list (classes in the same room)

This design models the timeline of classes per room as a linked list, enabling efficient traversal, insertion, and deletion of scheduled classes dynamically.

---

### 2. **RoomNode (Singly Linked List)**  
`RoomNode` represents a physical room within a university and contains:

- **Room Name**: Unique identifier of the room  
- **Head Pointer**: Points to the first `ClassNode` in that room's schedule  
- **Next Pointer**: Points to the next `RoomNode` in the day’s list of rooms

The `RoomNode` linked list forms a collection of rooms scheduled for a given day, allowing flexible addition and removal of rooms dynamically, without fixed upper bounds.

---

### 3. **TTADT (Timetable Abstract Data Type)**  
`TTADT` is the central managing class that maintains the timetable structure, consisting of:

- **Days Array**: An array of 6 pointers (Monday to Saturday), each pointing to the head of a linked list of `RoomNode`s for that day.  
- **Time Slots**: Fixed at 8, representing the university’s daily schedule slots.

The combination of an array (for constant-time day access) and linked lists (for dynamic room and class management) optimizes performance for both frequent queries and occasional updates.

---

## Algorithmic Approach

- **Insertion:**  
  When loading from the CSV file, the program parses each line, extracts the day, room, and 8 course slots. It uses linked list insertions to add `RoomNode`s if not already present and append `ClassNode`s for each scheduled course in the correct slot.

- **Searching:**  
  - To find all scheduled sections of a course, the system traverses all days, then all rooms, and then all classes, checking course names — a classical nested traversal optimized by early exits and string comparison.  
  - To find free classrooms, it iterates over a specific day and checks if any `ClassNode` for the specified slot is empty, efficiently identifying availability.

- **Memory Management:**  
  All string data is dynamically allocated and copied using secure functions (`strcpy_s`) to prevent buffer overflows. Linked list nodes are allocated/deallocated carefully to avoid memory leaks, demonstrating strong DSA discipline.

---

## Input File Specification

The system reads input from a CSV file named `timetable.csv`. Each line corresponds to a single day and room with scheduled classes for all time slots:

```

Day,Room,Course1,Course2,Course3,Course4,Course5,Course6,Course7,Course8

```

- **Day**: String representation (e.g., "Monday")  
- **Room**: Unique room identifier (e.g., "Room101")  
- **Course1 to Course8**: Course names or empty if no class is scheduled in that slot.

Example line:
```

Monday,Room101,Math101,Physics101,,Chemistry101,,,,English101

````

---

## User Interaction and Features

The program presents a command-line menu that supports:

1. **Loading Timetable from CSV File:**  
   Efficiently reads data and builds the linked list structures dynamically.

2. **Viewing the Complete Timetable:**  
   Prints the timetable day-wise, room-wise, and slot-wise in a readable format.

3. **Searching for a Course:**  
   Returns all occurrences of a course across days, rooms, and time slots, demonstrating comprehensive traversal and string matching.

4. **Finding Available Classrooms:**  
   For a specified day and time slot, lists all rooms with no scheduled class, useful for rescheduling or finding free spaces.

5. **Displaying a Section Timetable:**  
   Displays where and when a particular section has classes, emphasizing efficient filtering of linked list data.

6. **Graceful Exit:**  
   Properly deallocates all dynamic memory before program termination.

---

## Compilation and Execution

- Compile the project using Microsoft Visual Studio (recommended for `strcpy_s` support) or any C++17 compatible compiler.  
- Use the command:

```bash
g++ -o UniversityTimetable.exe main.cpp
````

* Place `timetable.csv` in the executable’s directory before running.
* Run the executable and interact via the menu-driven interface.

---

## Technical Highlights and Learning Outcomes

* **Advanced Linked List Usage:**
  Multi-level linked lists (days → rooms → classes) to model complex relationships dynamically.

* **Dynamic Memory and String Handling:**
  Mastery of `new`, `delete`, and safe string functions to manage memory effectively and securely.

* **Efficient Data Traversal:**
  Implemented multiple traversal algorithms for searching and filtering timetable data.

* **Modular and Maintainable Code:**
  Clear separation of concerns with distinct classes (`ClassNode`, `RoomNode`, `TTADT`), enabling scalability and ease of extension.

* **Practical Application of DSA:**
  Demonstrates how classical data structures solve real-world scheduling problems effectively.

---

## Possible Future Improvements

* Implement an interactive timetable editor to allow adding, updating, or deleting classes on the fly.
* Enhance performance by using balanced trees or hash maps for faster search operations.
* Build a graphical user interface (GUI) to improve user experience.
* Add functionality to save changes back to the CSV file.
* Integrate conflict detection algorithms to prevent overlapping schedules.

---

## Contact and Collaboration

For questions, suggestions, or collaborations, please reach out to me.

We welcome feedback and contributions to make this system more powerful and user-friendly.

---

## Acknowledgments

Special thanks to the course instructors and university resources that inspired this project and provided foundational knowledge in data structures and algorithms.

---

**Thank you for exploring the University Timetable Management System!**

```
