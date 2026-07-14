# 🎬 Cinema Management System – DSA Edition

A console‑based C++ application that demonstrates **real‑world usage of core Data Structures and Algorithms** for managing movie ticket bookings. Built as a single‑file project for easy compilation in Dev‑C++, Code::Blocks, or any C++ compiler.

## ✨ Features

- ✅ **Book / Cancel tickets** for 3 movies (Avengers, Batman, Spiderman).  
- ✅ **VIP (Row 0)** and **Regular (Rows 1‑4)** seating with different pricing.  
- ✅ **Waiting List** with priority (VIPs get preference) – implemented using a **Min‑Heap**.  
- ✅ **Undo** the last booking or cancellation – powered by a **Stack**.  
- ✅ **Fast search** by Booking ID – using a **Hash Table** (O(1) average).  
- ✅ **Fast search** by Customer Name – using a **Binary Search Tree** (O(log n) average).  
- ✅ **Sorted reports** – By Name (BST in‑order), By ID (Merge Sort), By Price (Quick Sort).  
- ✅ **Seat availability check** using **Binary Search** on a sorted list of booked seats.  
- ✅ **Revenue report** showing total income and VIP/Regular counts.

---

## 🧠 Data Structures & Algorithms Used

| Data Structure / Algorithm | Purpose |
|----------------------------|---------|
| **Doubly Linked List** | Maintains active bookings in insertion order (O(1) insert/delete given pointer). |
| **Hash Table** (chaining) | Maps Booking ID → Booking pointer for O(1) search/cancel. |
| **Binary Search Tree** (BST) | Sorts and searches bookings by customer name. |
| **Min‑Heap** (Priority Queue) | Implements the waiting list – VIPs get higher priority. |
| **Stack** | Stores history for Undo operation (LIFO). |
| **Merge Sort** | Sorts bookings by ID for sorted report. |
| **Quick Sort** | Sorts bookings by price for sorted report. |
| **Binary Search** | Checks if a seat is booked in O(log n) time. |

---

## 📋 Menu Options Overview

| Option | Description |
|--------|-------------|
| 1 | Show Movies |
| 2 | Show Seat Map |
| 3 | Book Ticket |
| 4 | Cancel Booking |
| 5 | Display All Bookings (insertion order) |
| 6 | Search Booking by ID (Hash Table) |
| 7 | Search Booking by Name (BST) |
| 8 | Sorted Report by Name (BST in‑order) |
| 9 | Sorted Report by ID (Merge Sort) |
| 10 | Sorted Report by Price (Quick Sort) |
| 11 | Check Seat Status (Binary Search) |
| 12 | Join Waiting List (Min‑Heap) |
| 13 | View Waiting List |
| 14 | Undo Last Action (Stack) |
| 15 | Revenue Report |
| 16 | Exit |

---

## 🛠️ How to Compile & Run

### Using Dev‑C++ / Code::Blocks
1. Open Dev‑C++ or Code::Blocks.  
2. Create a new project or just open the single `.cpp` file.  
3. Press **F9** (Compile) and **F10** (Run) – no extra libraries or include paths needed.
   
## 👤 Author
Diya
## License:
MIT License -feel free to use, modify, and distribute
