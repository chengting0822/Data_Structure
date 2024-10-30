#include <iostream>
using namespace std;

void print_arr(int* arr, int size) {
    cout << "陣列內容:";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << '\n';
}

void change_size(int*& arr, int size, int new_size) {
    int* new_arr = new int[new_size](); 
    copy(arr, arr + size, new_arr);
    delete[] arr;
    arr = new_arr;
}

int main() {
    cout << "輸入陣列原始大小(正整數!!!)" << endl;
    int size = 0;
    cout << "輸入:";
    cin >> size;
    while (size <= 0) {
        cout << "無效!!!\n重新輸入:";
        cin >> size;
    }

    int current_capacity = size; 
    int* arr = new int[current_capacity](); 
    cout << "輸入陣列元素:" << endl;
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }
    print_arr(arr, size);

    while (true) {
        cout << "是否還需要輸入資料?(Y/N):";
        char YN;
        cin >> YN;
        if (YN == 'Y' || YN == 'y') {
            cout << "還需要輸入多大的資料? :";
            int temp = 0;
            cin >> temp;

            if (size + temp > current_capacity) { // 檢查是否需要擴充
                while (current_capacity < size + temp)
                {
                    current_capacity = current_capacity* 2;
                }
                int new_size = (current_capacity); // 擴充到足夠的大小
                change_size(arr, current_capacity, new_size);
                current_capacity = new_size; // 更新當前容量
                cout << "陣列已擴充，新的大小為：" << new_size << endl;
            }

            cout << "請輸入資料:" << endl;
            for (int i = size; i < size + temp; i++) {
                cin >> arr[i];
            }
            size += temp; // 更新大小
            print_arr(arr, size);
        }
        else {
            break; 
        }
    }

    delete[] arr;
    return 0;
}
