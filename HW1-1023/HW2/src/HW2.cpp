#include <iostream>
#include <cmath>

using namespace std;

void generatePowerSet(char array[], int n) {
    int total = static_cast<int>(pow(2, n));
    char* subset = new char[n];
    int totalLoopCount = 0; // 計數器，記錄總迴圈次數

    for (int i = 0; i < total; i++) {
        totalLoopCount++; // 每次進入外層迴圈時增加計數
        int subsetSize = 0;
        for (int j = 0; j < n; j++) {
            totalLoopCount++; // 每次進入內層迴圈時增加計數
            if (i & (1 << j)) {
                subset[subsetSize++] = array[j];
            }
        }

        // 輸出子集
        cout << "{ ";
        for (int k = 0; k < subsetSize; k++) {
            cout << subset[k] << " ";
        }
        cout << "}" << endl;
    }

    cout << "總共執行了 " << totalLoopCount << " 次迴圈。" << endl; // 輸出總迴圈次數

    delete[] subset;
}

int main() {
    // 使用者輸入
    int number;
    cout << "輸入有幾個元素: ";
    cin >> number;

    char* array = new char[number]; // 動態配置陣列
    cout << "輸入"<<number<<"個元素:";
    for (int i = 0; i < number; i++) {
        cin >> array[i]; // 讀取輸入
    }
    // 生成子集
    generatePowerSet(array, number);

    delete[] array;
    return 0;
}
