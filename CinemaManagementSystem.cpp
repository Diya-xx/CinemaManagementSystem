// ============================================================
// CINEMA MANAGEMENT SYSTEM - DSA EDITION (SINGLE FILE VERSION)
// All data structures + algorithms combined into one .cpp file
// so it compiles directly in Dev-C++ / Code::Blocks with zero
// include-path setup. Just open this ONE file and Compile & Run.
// ============================================================

#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <stack>
#include <string>
#include <vector>
using namespace std;

// ---------------- from Booking.h ----------------
using namespace std;


struct Booking {
    int id;
    string name;
    int movieIndex;
    int row, col;
    bool isVIP;
    double price;
    string bookingTime;

    // Doubly Linked List pointers
    Booking* prev;
    Booking* next;


    Booking(int id, string _name, int _movieIndex, int _row, int _col, bool _isVIP, double _price)
        : id(id), name(_name), movieIndex(_movieIndex), row(_row), col(_col),
          isVIP(_isVIP), price(_price), prev(nullptr), next(nullptr) {
        time_t now = time(nullptr);
        std::string t = std::ctime(&now);
        if (!t.empty() && t.back() == '\n') t.pop_back();  
        bookingTime = t;
    }

   
    int seatCode() const { return row * 100 + col; }
};

// ---------------- from DoublyLinkedList.h ----------------
using namespace std;


class DoublyLinkedList {
private:
    Booking* head;
    Booking* tail;
    int count;

    void displayRecursive(Booking* node) const {
        if (node == nullptr) return;
        cout << "  ID: " << node->id
             << " | Name: " << node->name
             << " | Movie#: " << node->movieIndex + 1
             << " | Seat: (" << node->row << "," << node->col << ")"
             << " | " << (node->isVIP ? "VIP" : "Regular")
             << " | Rs." << node->price
             << " | " << node->bookingTime << "\n";
        displayRecursive(node->next);
    }

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // O(1) insert at head
    Booking* insertFront(int id, string name, int movieIndex, int row, int col, bool isVIP, double price) {
        Booking* node = new Booking(id, name, movieIndex, row, col, isVIP, price);
        node->next = head;
        node->prev = nullptr;
        if (head != nullptr) head->prev = node;
        head = node;
        if (tail == nullptr) tail = node;
        count++;
        return node;
    }

    // O(1) removal GIVEN the node pointer (the whole reason we keep a Hash Table)
    void removeNode(Booking* node) {
        if (node == nullptr) return;
        if (node->prev) node->prev->next = node->next;
        else head = node->next;
        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
        delete node;
        count--;
    }

    void displayAll() const {
        if (head == nullptr) {
            cout << "  No active bookings.\n";
            return;
        }
        displayRecursive(head);
    }

    // Dump into a vector so Sorting / BST / reports can work off a snapshot
    vector<Booking*> toVector() const {
        vector<Booking*> v;
        Booking* temp = head;
        while (temp != nullptr) {
            v.push_back(temp);
            temp = temp->next;
        }
        return v;
    }

    int size() const { return count; }
    bool isEmpty() const { return head == nullptr; }
};

// ---------------- from HashTable.h ----------------
using namespace std;

// HASH TABLE (separate chaining, built from scratch — not unordered_map)
// Maps booking ID -> Booking* so search/cancel is O(1) average instead of
// the O(n) linear scan the original project used.
class HashTable {
private:
    static const int CAPACITY = 101; // prime size -> fewer collisions
    vector<list<pair<int, Booking*>>> table;

    int hashFunction(int id) const {
        return (id % CAPACITY + CAPACITY) % CAPACITY; // guard against negative ids
    }

public:
    HashTable() : table(CAPACITY) {}

    void insert(int id, Booking* b) {
        table[hashFunction(id)].push_back({id, b});
    }

    Booking* search(int id) const {
        int idx = hashFunction(id);
        for (auto& p : table[idx]) {
            if (p.first == id) return p.second;
        }
        return nullptr;
    }

    void remove(int id) {
        int idx = hashFunction(id);
        auto& chain = table[idx];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->first == id) {
                chain.erase(it);
                return;
            }
        }
    }

    bool exists(int id) const { return search(id) != nullptr; }
};

// ---------------- from BST.h ----------------
using namespace std;

// BINARY SEARCH TREE keyed by customer NAME.
// Gives us always-sorted-by-name traversal (in-order) and O(log n) average
// search by name, which the flat linked list can't do without a full scan.
struct BSTNode {
    Booking* data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Booking* b) : data(b), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;

    BSTNode* insertRec(BSTNode* node, Booking* b) {
        if (node == nullptr) return new BSTNode(b);
        if (b->name < node->data->name) node->left = insertRec(node->left, b);
        else node->right = insertRec(node->right, b);
        return node;
    }

    BSTNode* findMin(BSTNode* node) const {
        while (node->left != nullptr) node = node->left;
        return node;
    }

    // Standard BST deletion by (name, id) so duplicate names don't collide
    BSTNode* removeRec(BSTNode* node, const string& name, int id) {
        if (node == nullptr) return nullptr;
        if (name < node->data->name) {
            node->left = removeRec(node->left, name, id);
        } else if (name > node->data->name) {
            node->right = removeRec(node->right, name, id);
        } else if (node->data->id != id) {
            // same name, different booking -> duplicates always live in the right subtree
            node->right = removeRec(node->right, name, id);
        } else {
            // found the exact node to delete
            if (node->left == nullptr) { BSTNode* r = node->right; delete node; return r; }
            if (node->right == nullptr) { BSTNode* l = node->left; delete node; return l; }
            BSTNode* succ = findMin(node->right);
            node->data = succ->data;
            node->right = removeRec(node->right, succ->data->name, succ->data->id);
        }
        return node;
    }

    void inorderRec(BSTNode* node, vector<Booking*>& out) const {
        if (node == nullptr) return;
        inorderRec(node->left, out);
        out.push_back(node->data);
        inorderRec(node->right, out);
    }

    // Equal names always land in the RIGHT subtree on insert (see insertRec),
    // so once we find a match we only need to keep walking right to collect
    // every booking that shares this name — no double counting.
    void searchRec(BSTNode* node, const string& name, vector<Booking*>& out) const {
        if (node == nullptr) return;
        if (name < node->data->name) {
            searchRec(node->left, name, out);
        } else if (name > node->data->name) {
            searchRec(node->right, name, out);
        } else {
            out.push_back(node->data);
            searchRec(node->right, name, out);
        }
    }

public:
    BST() : root(nullptr) {}

    void insert(Booking* b) { root = insertRec(root, b); }
    void remove(const string& name, int id) { root = removeRec(root, name, id); }

    vector<Booking*> inorderTraversal() const {
        vector<Booking*> out;
        inorderRec(root, out);
        return out;
    }

    vector<Booking*> searchByName(const string& name) const {
        vector<Booking*> out;
        searchRec(root, name, out);
        return out;
    }
};

// ---------------- from MinHeap.h ----------------
using namespace std;

struct WaitEntry {
    string name;
    int priority;   // 0 = VIP, 1 = Regular
    int arrivalNo;  
};

class MinHeap {
private:
    vector<WaitEntry> heap;

    bool higherPriority(const WaitEntry& a, const WaitEntry& b) const {
        if (a.priority != b.priority) return a.priority < b.priority;
        return a.arrivalNo < b.arrivalNo;
    }

    void heapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (higherPriority(heap[i], heap[parent])) {
                swap(heap[i], heap[parent]);
                i = parent;
            } else break;
        }
    }

    void heapifyDown(int i) {
        int n = heap.size();
        while (true) {
            int left = 2 * i + 1, right = 2 * i + 2, best = i;
            if (left < n && higherPriority(heap[left], heap[best])) best = left;
            if (right < n && higherPriority(heap[right], heap[best])) best = right;
            if (best == i) break;
            swap(heap[i], heap[best]);
            i = best;
        }
    }

public:
    void push(const string& name, bool isVIP, int arrivalNo) {
        heap.push_back({name, isVIP ? 0 : 1, arrivalNo});
        heapifyUp(heap.size() - 1);
    }

    WaitEntry pop() {
        WaitEntry top = heap.front();
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
        return top;
    }

    bool isEmpty() const { return heap.empty(); }
    int size() const { return heap.size(); }

    void displayAll() const {
        if (heap.empty()) { cout << "  Waiting list is empty.\n"; return; }
        for (auto& e : heap)
            cout << "  " << e.name << (e.priority == 0 ? " [VIP]" : " [Regular]") << "\n";
    }
};

// ---------------- from SortAlgorithms.h ----------------
using namespace std;


// ---------- MERGE SORT: used to sort bookings by ID (report #1) ----------
inline void mergeById(vector<Booking*>& arr, int l, int m, int r) {
    vector<Booking*> left(arr.begin() + l, arr.begin() + m + 1);
    vector<Booking*> right(arr.begin() + m + 1, arr.begin() + r + 1);
    size_t i = 0, j = 0;
    int k = l;
    while (i < left.size() && j < right.size())
        arr[k++] = (left[i]->id <= right[j]->id) ? left[i++] : right[j++];
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

inline void mergeSortById(vector<Booking*>& arr, int l, int r) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    mergeSortById(arr, l, m);
    mergeSortById(arr, m + 1, r);
    mergeById(arr, l, m, r);
}

// ---------- QUICK SORT: used to sort bookings by price (report #2) ----------
inline int partitionByPrice(vector<Booking*>& arr, int low, int high) {
    double pivot = arr[high]->price;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j]->price <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

inline void quickSortByPrice(vector<Booking*>& arr, int low, int high) {
    if (low >= high) return;
    int pi = partitionByPrice(arr, low, high);
    quickSortByPrice(arr, low, pi - 1);
    quickSortByPrice(arr, pi + 1, high);
}

// ---------------- from BinarySearch.h ----------------
using namespace std;

// BINARY SEARCH on a sorted array of booked seat-codes.
// The Cinema class keeps `bookedCodes` sorted at all times (insertion-sort
// style single-element insert), so this runs in O(log n) instead of the
// O(n) scan used elsewhere.
inline bool binarySearchSeat(const vector<int>& sortedCodes, int target, int& comparisons) {
    int low = 0, high = (int)sortedCodes.size() - 1;
    comparisons = 0;
    while (low <= high) {
        comparisons++;
        int mid = low + (high - low) / 2;
        if (sortedCodes[mid] == target) return true;
        if (sortedCodes[mid] < target) low = mid + 1;
        else high = mid - 1;
    }
    return false;
}

// Insert into a sorted vector keeping it sorted (used on every booking)
inline void sortedInsert(vector<int>& sortedCodes, int code) {
    int i = sortedCodes.size() - 1;
    sortedCodes.push_back(code);
    while (i >= 0 && sortedCodes[i] > code) {
        sortedCodes[i + 1] = sortedCodes[i];
        i--;
    }
    sortedCodes[i + 1] = code;
}

inline void sortedRemove(vector<int>& sortedCodes, int code) {
    for (size_t i = 0; i < sortedCodes.size(); i++) {
        if (sortedCodes[i] == code) {
            sortedCodes.erase(sortedCodes.begin() + i);
            return;
        }
    }
}

// ---------------- from Cinema.h ----------------
using namespace std;

const int NUM_MOVIES = 3;
const int ROWS = 5;
const int COLS = 5;

class Cinema {
private:
    string movies[NUM_MOVIES] = {"Avengers", "Batman", "Spiderman"};
    double regularPrice = 500.0;
    double vipPrice = 900.0;
    int seats[NUM_MOVIES][ROWS][COLS] = {0};

    DoublyLinkedList bookingList;   // active bookings
    HashTable idIndex;              // id -> Booking* (O(1) lookup)
    BST nameIndex;                  // name -> Booking* (sorted reports)
    MinHeap waitingList;            // priority queue for full shows
    vector<int> bookedSeatCodes;    // kept sorted for Binary Search demo
    stack<Booking> undoStack;       // last-action undo (LIFO by nature)

    int nextArrivalNo = 1;
    int nextId = 1000;

public:
    void showMovies() const;
    void showSeatMap(int movieIndex) const;
    bool bookTicket(int movieIndex, int row, int col, const string& name);
    void joinWaitingList(const string& name, bool isVIP);
    void processWaitingListForFreedSeat(int movieIndex, int row, int col);
    bool cancelBooking(int id);
    void displayAllBookings() const;
    Booking* searchById(int id) const;
    vector<Booking*> searchByName(const string& name) const;
    void displaySortedByName() const;
    void displaySortedById() const;
    void displaySortedByPrice() const;
    void checkSeatViaBinarySearch(int row, int col) const;
    void showWaitingList() const;
    void undoLastAction();
    void revenueReport() const;
    int generateId();
    bool isValidMovie(int m) const { return m >= 0 && m < NUM_MOVIES; }
    bool isValidSeat(int r, int c) const { return r >= 0 && r < ROWS && c >= 0 && c < COLS; }
};

// ---------------- Cinema.cpp (method implementations) ----------------
using namespace std;

int Cinema::generateId() { return nextId++; }

void Cinema::showMovies() const {
    cout << "\n===== Now Showing =====\n";
    for (int i = 0; i < NUM_MOVIES; i++)
        cout << "  " << i + 1 << ". " << movies[i] << "\n";
    cout << "  (Row 0 = VIP @ Rs." << vipPrice << " | Rows 1-4 = Regular @ Rs." << regularPrice << ")\n";
}

void Cinema::showSeatMap(int movieIndex) const {
    if (!isValidMovie(movieIndex)) { cout << "Invalid movie.\n"; return; }
    cout << "\nSeat map for " << movies[movieIndex] << "  (0 = free, X = booked)\n   ";
    for (int c = 0; c < COLS; c++) cout << c << " ";
    cout << "\n";
    for (int r = 0; r < ROWS; r++) {
        cout << r << (r == 0 ? "V" : " ") << " ";
        for (int c = 0; c < COLS; c++)
            cout << (seats[movieIndex][r][c] ? "X " : "0 ");
        cout << "\n";
    }
}

bool Cinema::bookTicket(int movieIndex, int row, int col, const string& name) {
    if (!isValidMovie(movieIndex) || !isValidSeat(row, col)) {
        cout << "Invalid movie or seat.\n";
        return false;
    }
    if (seats[movieIndex][row][col] == 1) {
        cout << "Seat already booked!\n";
        return false;
    }
    bool isVIP = (row == 0);
    double price = isVIP ? vipPrice : regularPrice;
    int id = generateId();

    seats[movieIndex][row][col] = 1;
    Booking* node = bookingList.insertFront(id, name, movieIndex, row, col, isVIP, price);
    idIndex.insert(id, node);
    nameIndex.insert(node);
    sortedInsert(bookedSeatCodes, node->seatCode());
    undoStack.push(*node); // snapshot for undo

    cout << "Booking confirmed! ID: " << id << " | Seat (" << row << "," << col << ") | "
         << (isVIP ? "VIP" : "Regular") << " | Rs." << price << "\n";
    return true;
}

void Cinema::joinWaitingList(const string& name, bool isVIP) {
    waitingList.push(name, isVIP, nextArrivalNo++);
    cout << name << " added to the waiting list ("
         << (isVIP ? "VIP" : "Regular") << " priority).\n";
}

void Cinema::processWaitingListForFreedSeat(int movieIndex, int row, int col) {
    if (waitingList.isEmpty()) return;
    WaitEntry entry = waitingList.pop();
    cout << "Auto-assigning freed seat (" << row << "," << col << ") in "
         << movies[movieIndex] << " to " << entry.name << " from the waiting list.\n";
    bookTicket(movieIndex, row, col, entry.name);
}

bool Cinema::cancelBooking(int id) {
    Booking* node = idIndex.search(id);
    if (node == nullptr) {
        cout << "Booking not found!\n";
        return false;
    }
    int movieIndex = node->movieIndex, row = node->row, col = node->col;
    string name = node->name;

    undoStack.push(*node); // snapshot before it's gone
    seats[movieIndex][row][col] = 0;
    sortedRemove(bookedSeatCodes, node->seatCode());
    nameIndex.remove(node->name, node->id);
    idIndex.remove(id);
    bookingList.removeNode(node); // node is deleted inside here

    cout << "Booking " << id << " (" << name << ") cancelled!\n";
    processWaitingListForFreedSeat(movieIndex, row, col);
    return true;
}

void Cinema::displayAllBookings() const {
    cout << "\n===== All Active Bookings (insertion order) =====\n";
    bookingList.displayAll();
}

Booking* Cinema::searchById(int id) const {
    return idIndex.search(id);
}

vector<Booking*> Cinema::searchByName(const string& name) const {
    return nameIndex.searchByName(name);
}

void Cinema::displaySortedByName() const {
    cout << "\n===== Bookings Sorted by Name =====\n";
    auto v = nameIndex.inorderTraversal();
    if (v.empty()) { cout << "  No bookings.\n"; return; }
    for (auto* b : v)
        cout << "  " << b->name << " | ID: " << b->id << " | Seat (" << b->row << "," << b->col << ")\n";
}

void Cinema::displaySortedById() const {
    cout << "\n===== Bookings Sorted by ID  =====\n";
    auto v = bookingList.toVector();
    if (v.empty()) { cout << "  No bookings.\n"; return; }
    mergeSortById(v, 0, (int)v.size() - 1);
    for (auto* b : v)
        cout << "  ID: " << b->id << " | " << b->name << " | Rs." << b->price << "\n";
}

void Cinema::displaySortedByPrice() const {
    cout << "\n===== Bookings Sorted by Price  =====\n";
    auto v = bookingList.toVector();
    if (v.empty()) { cout << "  No bookings.\n"; return; }
    quickSortByPrice(v, 0, (int)v.size() - 1);
    for (auto* b : v)
        cout << "  Rs." << b->price << " | ID: " << b->id << " | " << b->name << "\n";
}

void Cinema::checkSeatViaBinarySearch(int row, int col) const {
    int code = row * 100 + col;
    int comparisons = 0;
    bool found = binarySearchSeat(bookedSeatCodes, code, comparisons);
    cout << "Binary Search result for seat (" << row << "," << col << "): "
         << (found ? "BOOKED" : "FREE")
         << "  (" << comparisons << " comparisons on " << bookedSeatCodes.size() << " booked seats)\n";
}

void Cinema::showWaitingList() const {
    cout << "\n===== Waiting List (Min-Heap, VIP priority first) =====\n";
    waitingList.displayAll();
}

void Cinema::undoLastAction() {
    if (undoStack.empty()) {
        cout << "Nothing to undo.\n";
        return;
    }
    Booking last = undoStack.top();
    undoStack.pop();

    // If the booking is still active, undo means "cancel it"
    Booking* stillActive = idIndex.search(last.id);
    if (stillActive != nullptr) {
        seats[last.movieIndex][last.row][last.col] = 0;
        sortedRemove(bookedSeatCodes, last.seatCode());
        nameIndex.remove(last.name, last.id);
        idIndex.remove(last.id);
        bookingList.removeNode(stillActive);
        cout << "Undo: booking " << last.id << " (" << last.name << ") reverted.\n";
    } else {
        cout << "Undo: booking " << last.id << " was already cancelled, nothing to revert.\n";
    }
}

void Cinema::revenueReport() const {
    auto v = bookingList.toVector();
    double total = 0;
    int vipCount = 0, regCount = 0;
    for (auto* b : v) {
        total += b->price;
        if (b->isVIP) vipCount++; else regCount++;
    }
    cout << "\n===== Revenue Report =====\n";
    cout << "  Active bookings : " << v.size() << " (VIP: " << vipCount << ", Regular: " << regCount << ")\n";
    cout << fixed << setprecision(2);
    cout << "  Total revenue   : Rs." << total << "\n";
}

// ---------------- main.cpp ----------------
using namespace std;

// Clears a bad cin state so the menu doesn't loop forever on non-numeric input
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        cin >> x;
        if (cin.fail()) {
            clearInput();
            cout << "Please enter a valid number.\n";
            continue;
        }
        return x;
    }
}

void printMenu() {
    cout << "\n============================================\n";
    cout << "     CINEMA MANAGEMENT SYSTEM (DSA Edition)\n";
    cout << "============================================\n";
    cout << " 1.  Show Movies\n";
    cout << " 2.  Show Seat Map\n";
    cout << " 3.  Book Ticket\n";
    cout << " 4.  Cancel Booking\n";
    cout << " 5.  Display All Bookings (insertion order)\n";
    cout << " 6.  Search Booking by ID       (Hash Table)\n";
    cout << " 7.  Search Booking by Name     (BST)\n";
    cout << " 8.  Sorted Report by Name      (BST in-order)\n";
    cout << " 9.  Sorted Report by ID        (Merge Sort)\n";
    cout << "10.  Sorted Report by Price     (Quick Sort)\n";
    cout << "11.  Check Seat Status          (Binary Search)\n";
    cout << "12.  Join Waiting List          (Priority Queue / Min-Heap)\n";
    cout << "13.  View Waiting List\n";
    cout << "14.  Undo Last Booking/Cancel   (Stack)\n";
    cout << "15.  Revenue Report\n";
    cout << "16.  Exit\n";
    cout << "Enter choice: ";
}

int main() {
    Cinema cinema;
    int choice;

    do {
        printMenu();
        cin >> choice;
        if (cin.fail()) { clearInput(); cout << "Invalid input!\n"; continue; }

        switch (choice) {
            case 1:
                cinema.showMovies();
                break;

            case 2: {
                cinema.showMovies();
                int m = readInt("Enter movie number (1-3): ") - 1;
                cinema.showSeatMap(m);
                break;
            }

            case 3: {
                cinema.showMovies();
                int m = readInt("Enter movie number (1-3): ") - 1;
                cinema.showSeatMap(m);
                int r = readInt("Enter row (0-4): ");
                int c = readInt("Enter col (0-4): ");
                cout << "Enter name: ";
                string name; cin >> ws; getline(cin, name);
                if (!cinema.bookTicket(m, r, c, name)) {
                    cout << "Would you like to join the waiting list instead? (1=Yes, 0=No): ";
                    int j; cin >> j;
                    if (j == 1) {
                        cout << "VIP priority? (1=Yes, 0=No): ";
                        int vip; cin >> vip;
                        cinema.joinWaitingList(name, vip == 1);
                    }
                }
                break;
            }

            case 4: {
                int id = readInt("Enter booking ID to cancel: ");
                cinema.cancelBooking(id);
                break;
            }

            case 5:
                cinema.displayAllBookings();
                break;

            case 6: {
                int id = readInt("Enter booking ID: ");
                Booking* b = cinema.searchById(id);
                if (b) cout << "Found: " << b->name << " | Seat (" << b->row << "," << b->col
                            << ") | " << (b->isVIP ? "VIP" : "Regular") << "\n";
                else cout << "Not found!\n";
                break;
            }

            case 7: {
                cout << "Enter name: ";
                string name; cin >> ws; getline(cin, name);
                auto results = cinema.searchByName(name);
                if (results.empty()) { cout << "Not found!\n"; break; }
                for (auto* b : results)
                    cout << "Found: ID " << b->id << " | Seat (" << b->row << "," << b->col << ")\n";
                break;
            }

            case 8: cinema.displaySortedByName(); break;
            case 9: cinema.displaySortedById(); break;
            case 10: cinema.displaySortedByPrice(); break;

            case 11: {
                int r = readInt("Enter row (0-4): ");
                int c = readInt("Enter col (0-4): ");
                cinema.checkSeatViaBinarySearch(r, c);
                break;
            }

            case 12: {
                cout << "Enter name: ";
                string name; cin >> ws; getline(cin, name);
                cout << "VIP priority? (1=Yes, 0=No): ";
                int vip; cin >> vip;
                cinema.joinWaitingList(name, vip == 1);
                break;
            }

            case 13: cinema.showWaitingList(); break;
            case 14: cinema.undoLastAction(); break;
            case 15: cinema.revenueReport(); break;

            case 16:
                cout << "Exiting... Thank you!\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 16);

    return 0;
}
