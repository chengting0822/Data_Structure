#include <iostream>
#include <cmath>
using namespace std;

class Term {
public:
    int coef; // 系數
    int exp;  // 指數
    Term* link; // 指向下一項的指標

    Term(int c = 0, int e = 0) : coef(c), exp(e), link(nullptr) {} // 建構子，初始化系數、指數及鏈結指標
};

class Polynomial {
private:
    Term* header; // 圓形鏈結串列的表頭節點

    // 清除多項式的節點
    void clear();
    // 複製多項式的內容
    void copyFrom(const Polynomial& a);

public:
    // 建構子，初始化空的多項式
    Polynomial();
    // 複製建構子
    Polynomial(const Polynomial& a);
    // 解構子，釋放記憶體
    ~Polynomial();
    // 賦值運算子，深度複製多項式
    Polynomial& operator=(const Polynomial& a);
    // 輸入運算子，讀取多項式並轉換為圓形鏈結
    friend istream& operator>>(istream& is, Polynomial& x);
    // 輸出運算子，將多項式轉換為字串並輸出
    friend ostream& operator<<(ostream& os, const Polynomial& x);
    // 多項式加法
    Polynomial operator+(const Polynomial& b) const;
    // 多項式減法，透過加上另一多項式的負值實現
    Polynomial operator-(const Polynomial& b) const;
    // 多項式乘法
    Polynomial operator*(const Polynomial& b) const;
    // 計算多項式在x處的值
    double Evaluate(double x) const;
};

// 定義 Polynomial 類別的成員函式

// 清除多項式的節點
void Polynomial::clear() {
    if (!header) return;
    Term* current = header->link;
    while (current != header) {
        Term* toDelete = current;
        current = current->link;
        delete toDelete;
    }
    delete header;
    header = nullptr;
}

// 複製多項式的內容
void Polynomial::copyFrom(const Polynomial& a) {
    if (!a.header) return;
    header = new Term(); // 初始化空的表頭節點
    Term* tail = header; // 用來追加新節點的指標
    Term* current = a.header->link;
    while (current != a.header) {
        Term* newTerm = new Term(current->coef, current->exp);
        tail->link = newTerm; // 連結新節點
        tail = newTerm; // 移動尾端指標
        current = current->link;
    }
    tail->link = header; // 設置圓形鏈結結構
}

// 建構子，初始化空的多項式
Polynomial::Polynomial() {
    header = new Term(); // 創建表頭節點
    header->link = header; // 指向自己，形成圓形鏈結
}

// 複製建構子
Polynomial::Polynomial(const Polynomial& a) : header(nullptr) {
    copyFrom(a);
}

// 解構子，釋放記憶體
Polynomial::~Polynomial() {
    clear();
}

// 賦值運算子
Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this != &a) {
        clear(); // 先清除當前多項式
        copyFrom(a); // 然後複製新多項式
    }
    return *this;
}


// 多項式加法
Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial result;
    Term* aCurrent = header->link;
    Term* bCurrent = b.header->link;
    Term* tail = result.header;

    while (aCurrent != header || bCurrent != b.header) {
        int coef, exp;
        if (aCurrent != header && (bCurrent == b.header || aCurrent->exp > bCurrent->exp)) {
            coef = aCurrent->coef;
            exp = aCurrent->exp;
            aCurrent = aCurrent->link;
        }
        else if (bCurrent != b.header && (aCurrent == header || bCurrent->exp > aCurrent->exp)) {
            coef = bCurrent->coef;
            exp = bCurrent->exp;
            bCurrent = bCurrent->link;
        }
        else {
            coef = aCurrent->coef + bCurrent->coef;
            exp = aCurrent->exp;
            aCurrent = aCurrent->link;
            bCurrent = bCurrent->link;
        }

        if (coef != 0) { // 只有當係數不為零時，才添加到結果中
            Term* newTerm = new Term(coef, exp);
            tail->link = newTerm;
            tail = newTerm;
        }
    }
    tail->link = result.header; // 完成圓形鏈結
    return result;
}

// 多項式減法，透過加上另一多項式的負值實現
Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial negB = b;
    Term* current = negB.header->link;
    while (current != negB.header) {
        current->coef = -current->coef; // 取負
        current = current->link;
    }
    return *this + negB; // 加上負的多項式
}

// 多項式乘法
Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial result;
    Term* aCurrent = header->link;

    while (aCurrent != header) {
        Polynomial temp;
        Term* bCurrent = b.header->link;
        Term* tail = temp.header;

        while (bCurrent != b.header) {
            int coef = aCurrent->coef * bCurrent->coef;
            int exp = aCurrent->exp + bCurrent->exp;
            Term* newTerm = new Term(coef, exp);
            tail->link = newTerm;
            tail = newTerm;
            bCurrent = bCurrent->link;
        }
        tail->link = temp.header;
        result = result + temp; // 將中間結果相加
        aCurrent = aCurrent->link;
    }
    return result;
}

// 計算多項式在x處的值
double Polynomial::Evaluate(double x) const {
    double result = 0;
    Term* current = header->link;
    while (current != header) {
        result += current->coef * pow(x, current->exp); // 計算每一項的值並累加
        current = current->link;
    }
    return result;
}

// 輸入運算子，讀取多項式並轉換為圓形鏈結
istream& operator>>(istream& is, Polynomial& x) {
    x.clear(); // 先清除原來的多項式
    int n;
    is >> n; // 讀取項數
    if (n <= 0) return is;

    x.header = new Term(); // 創建空的表頭節點
    Term* tail = x.header;

    // 讀取每一項的係數和指數，並鏈結到多項式中
    for (int i = 0; i < n; ++i) {
        int c, e;
        is >> c >> e;
        Term* newTerm = new Term(c, e);
        tail->link = newTerm;
        tail = newTerm;
    }
    tail->link = x.header; // 完成圓形鏈結
    return is;
}

// 輸出運算子，將多項式轉換為字串並輸出
ostream& operator<<(ostream& os, const Polynomial& x) {
    if (!x.header || x.header->link == x.header) { // 如果多項式為空
        os << "0";
        return os;
    }

    Term* current = x.header->link;
    bool first = true;
    while (current != x.header) { // 遍歷多項式的每一項
        if (!first && current->coef > 0) os << "+"; // 若不是第一項且係數為正，加上“+”
        os << current->coef << "x^" << current->exp;
        current = current->link;
        if (current != x.header) os << " "; // 若不是最後一項，則加上空格
        first = false;
    }
    return os;
}


int main() {
    Polynomial p1, p2;
    cout << "請輸入第一個多項式 (格式: n c1 e1 c2 e2 ...): ";
    cin >> p1;
    cout << "請輸入第二個多項式 (格式: n c1 e1 c2 e2 ...): ";
    cin >> p2;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "第一個多項式 P1: " << p1 << endl;
    cout << "第二個多項式 P2: " << p2 << endl;
    cout << "多項式加法結果: " << sum << endl;
    cout << "多項式減法結果: " << diff << endl;
    cout << "多項式乘法結果: " << prod << endl;

    double x;
    cout << "請輸入 x 的值以計算各個多項式的值: ";
    cin >> x;

    // 計算並顯示所有多項式的值
    cout << "P1(" << x << ") = " << p1.Evaluate(x) << endl;
    cout << "P2(" << x << ") = " << p2.Evaluate(x) << endl;
    cout << "P1 + P2(" << x << ") = " << sum.Evaluate(x) << endl;
    cout << "P1 - P2(" << x << ") = " << diff.Evaluate(x) << endl;
    cout << "P1 * P2(" << x << ") = " << prod.Evaluate(x) << endl;

    return 0;
}
