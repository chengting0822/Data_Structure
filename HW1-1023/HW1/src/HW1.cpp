#include <iostream>
using namespace std;
long long temp[5][10000000]; 
long long  count1 = 0;
long long  count2 = 0;
long long ackermann1(long long m, long long n)//一般遞迴
{
    if (m == 0) {
        count2++;
        return n + 1;
    }
    if (n == 0) {
        count2++;
        return ackermann1(m - 1, 1);
    }
    count2++;
    return ackermann1(m - 1, ackermann1(m, n - 1));
}
long long ackermann(long long m, long long n) //優化遞迴
{
    if (temp[m][n] != -1) {
        count1++;
        return temp[m][n];
    }
    if (m == 0) {
        count1++;
        return temp[m][n] = n + 1;
    }
    if (n == 0) {
        count1++;
        return temp[m][n] = ackermann(m - 1, 1);
    }
    count1++;
    return temp[m][n] = ackermann(m - 1, ackermann(m, n - 1));
}

int main() {
    long long m, n;

    //初始化 temp 陣列
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 10000000; ++j) {
            temp[i][j] = -1;  
        }
    }

    cout << "請輸入 m 和 n 的值：";
    cin >> m >> n;
    // 計算 Ackermann 函數
    long long result = ackermann(m, n);
    long long result1 = ackermann1(m, n);
    
    cout <<"一般遞迴:"<<endl<< "A(" << m << ", " << n << ") = " << result << "\n" << "總共遞迴:" << count2 << "次" << endl;
    cout << "優化遞迴:" << endl << "A(" << m << ", " << n << ") = " << result1 << "\n" << "總共遞迴:" << count1 << "次" << endl;


    return 0;
}
