#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to calculate the sum of elements within a specified range
int calculateSum(int* arr, int start, int end) {
    int sum = 0;
    for (int i = start; i <= end; ++i) {
        sum += *(arr + i);
    }
    return sum;
}

// Function to set an element of the array using a pointer
void setElement(int* arr, int idx, int val) {
    *(arr + idx) = val;
}

int main() {
    int n, m;
    cin >> n >> m;

    // Reading the array
    vector<int> data(n);
    int* arr = &data[0];
    for (int i = 0; i < n; ++i) {
        cin >> *(arr + i);
    }

    // Processing operations
    for (int i = 0; i < m; ++i) {
        string op;
        cin >> op;

        if (op == "sum") {
            int start, end;
            cin >> start >> end;
            // Calculate sum using pointers and references
            cout << calculateSum(arr, start, end) << endl;
        } else if (op == "set") {
            int idx, val;
            cin >> idx >> val;
            // Set value using pointers and references
            setElement(arr, idx, val);
        }
    }

    // Output the modified array
    for (int i = 0; i < n; ++i) {
        cout << *(arr + i) << " ";
    }
    cout << endl;

    return 0;
}
