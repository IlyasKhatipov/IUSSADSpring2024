#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Abstract base class for data structures
class DataStructure {
public:
    // Pure virtual functions for common operations
    virtual void insert(int x) = 0;
    virtual void remove() = 0;
    virtual bool search(int x) = 0;
    virtual ~DataStructure() {} // Virtual destructor
};

// Abstract class for sequential data structures, inheriting from DataStructure
class SequentialDataStructure : public virtual DataStructure {
public:
    // Pure virtual functions for sequential operations
    virtual void pushBack(int x) = 0;
    virtual void pushFront(int x) = 0;
    virtual void popBack() = 0;
    virtual void popFront() = 0;
    virtual ~SequentialDataStructure() {} // Virtual destructor
};

// Abstract class for dynamic data structures, inheriting from DataStructure
class DynamicDataStructure : public virtual DataStructure {
public:
    // Pure virtual functions for dynamic operations
    virtual void resize(int new_size) = 0;
    virtual void clear() = 0;
    virtual ~DynamicDataStructure() {} // Virtual destructor
};

// Concrete class representing a List data structure, inheriting from both SequentialDataStructure and DynamicDataStructure
class List : public SequentialDataStructure, public DynamicDataStructure {
private:
    vector<int> elements; // Vector to store elements

public:
    // Implementation of insert operation
    void insert(int x) override {
        elements.push_back(x);
    }

    // Implementation of remove operation
    void remove() override {
        if (!elements.empty()) {
            elements.pop_back();
        }
    }

    // Implementation of search operation
    bool search(int x) override {
        for (int element : elements) {
            if (element == x) {
                return true;
            }
        }
        return false;
    }

    // Implementation of pushBack operation
    void pushBack(int x) override {
        elements.push_back(x);
    }

    // Implementation of pushFront operation
    void pushFront(int x) override {
        elements.insert(elements.begin(), x);
    }

    // Implementation of popBack operation
    void popBack() override {
        if (!elements.empty()) {
            elements.pop_back();
        }
    }

    // Implementation of popFront operation
    void popFront() override {
        if (!elements.empty()) {
            elements.erase(elements.begin());
        }
    }

    // Implementation of resize operation
    void resize(int new_size) override {
        elements.resize(new_size);
    }

    // Implementation of clear operation
    void clear() override {
        elements.clear();
    }

    // Overload the << operator to output the list elements
    friend ostream& operator<<(ostream& out, const List& lst) {
        for (int i = 0; i < lst.elements.size(); ++i) {
            out << lst.elements[i] << " ";
        }
        return out;
    }
};

int main() {
    int n;
    cin >> n;

    List list; // Create a List object

    // Process each operation
    for (int i = 0; i < n; ++i) {
        string op;
        cin >> op;

        if (op == "insert") {
            int x;
            cin >> x;
            list.insert(x);
        } else if (op == "remove") {
            list.remove();
        } else if (op == "search") {
            int x;
            cin >> x;
            cout << (list.search(x) ? "YES" : "NO") << endl;
        }
    }

    cout << list << endl; // Output the list elements

    return 0;
}
