#include <iostream>
using namespace std;

void print_arr(int* arr, int size) {
    cout << "�}�C���e:";
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
    cout << "��J�}�C��l�j�p(�����!!!)" << endl;
    int size = 0;
    cout << "��J:";
    cin >> size;
    while (size <= 0) {
        cout << "�L��!!!\n���s��J:";
        cin >> size;
    }

    int current_capacity = size; 
    int* arr = new int[current_capacity](); 
    cout << "��J�}�C����:" << endl;
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }
    print_arr(arr, size);

    while (true) {
        cout << "�O�_�ٻݭn��J���?(Y/N):";
        char YN;
        cin >> YN;
        if (YN == 'Y' || YN == 'y') {
            cout << "�ٻݭn��J�h�j�����? :";
            int temp = 0;
            cin >> temp;

            if (size + temp > current_capacity) { // �ˬd�O�_�ݭn�X�R
                while (current_capacity < size + temp)
                {
                    current_capacity = current_capacity* 2;
                }
                int new_size = (current_capacity); // �X�R�쨬�����j�p
                change_size(arr, current_capacity, new_size);
                current_capacity = new_size; // ��s��e�e�q
                cout << "�}�C�w�X�R�A�s���j�p���G" << new_size << endl;
            }

            cout << "�п�J���:" << endl;
            for (int i = size; i < size + temp; i++) {
                cin >> arr[i];
            }
            size += temp; // ��s�j�p
            print_arr(arr, size);
        }
        else {
            break; 
        }
    }

    delete[] arr;
    return 0;
}
