#include <iostream>
#include <string>

using namespace std;


template <typename T>
int binarySearch(T data[], int n, const T& target) {
    int left = 0, right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (data[mid] == target) {
            return mid;
        }
        else if (data[mid] < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}

int main() {
    char type;
    int n, q;
    while (cin >> type >> n >> q) {
        if (type == 'i') {
            int data[30];
            for (int i = 0; i < n; ++i) {
                cin >> data[i];
            }


            while (q--) {
                int target;
                cin >> target;
                cout << binarySearch(data, n, target) << endl;
            }
        }
        else if (type == 's') {
            string data[30];
            for (int i = 0; i < n; ++i) {
                cin >> data[i];
            }


            while (q--) {
                string target;
                cin >> target;
                cout << binarySearch(data, n, target) << endl;
            }
        }
        cout << endl;
    }

    return 0;
}
