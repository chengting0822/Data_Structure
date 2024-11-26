#include <iostream>
using namespace std;

class Polynomial;
class Term {
    friend Polynomial;
    friend istream& operator>>(istream& in, Polynomial& p); // 多載 >>
    friend ostream& operator<<(ostream& out, const Polynomial& p); // 多載 <<
private:
    double coef; // 係數
    int exp; // 次方數
};

class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& p); // 多載 >>
    friend ostream& operator<<(ostream& out, const Polynomial& p); // 多載 <<
private:
    Term* termArray; // 非零項的陣列
    int capacity; // termArray 的大小
    int terms; // 非零項的數量
public:
    Polynomial(); // 建構子
    Polynomial Add(Polynomial poly); // 加法
    Polynomial Mult(Polynomial poly); // 乘法
    double Eval(double f); // 計算總數
    void NewTerm(const double newCoef, const int newExp); // 新增新的項
};

Polynomial::Polynomial() : capacity(2), terms(0) { // 建構子
    termArray = new Term[capacity];
}

void Polynomial::NewTerm(const double newCoef, const int newExp) { // 新增新的項
    if (terms == capacity) { // 空間不足
        capacity *= 2; // 空間翻倍
        Term* tmp = new Term[capacity];
        copy(termArray, termArray + terms, tmp);
        delete[] termArray; // 刪除原陣列，釋放記憶體
        termArray = tmp;
    }
    termArray[terms].coef = newCoef; // 放入係數
    termArray[terms++].exp = newExp; // 放入次方數
}

Polynomial Polynomial::Add(Polynomial b) { // 加法
    Polynomial poly3;
    int c1 = terms; // poly1的項數
    int c2 = b.terms; // poly2的項數
    int i1 = 0; // poly1處理到第幾個
    int i2 = 0; // poly2處理到第幾個
    while (c1 > 0 || c2 > 0) {
        if (termArray[i1].exp > b.termArray[i2].exp) {
            poly3.NewTerm(termArray[i1].coef, termArray[i1].exp);
            i1++;
            c1--;
        }
        else if (b.termArray[i2].exp > termArray[i1].exp) {
            poly3.NewTerm(b.termArray[i2].coef, b.termArray[i2].exp);
            i2++;
            c2--;
        }
        else if (b.termArray[i2].exp == termArray[i1].exp) {
            poly3.NewTerm((b.termArray[i2].coef + termArray[i1].coef), b.termArray[i2].exp);
            i1++;
            i2++;
            c1--;
            c2--;
        }
    }
    return poly3;
}

Polynomial Polynomial::Mult(Polynomial b) {
    Polynomial poly3;
    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < b.terms; j++) {
            double newCoef = termArray[i].coef * b.termArray[j].coef;
            int newExp = termArray[i].exp + b.termArray[j].exp;
            poly3.NewTerm(newCoef, newExp);
        }
    }

    // 合併相同次方的項
    for (int i = 0; i < poly3.terms; i++) {
        for (int j = i + 1; j < poly3.terms; j++) {
            if (poly3.termArray[i].exp == poly3.termArray[j].exp) {
                poly3.termArray[i].coef += poly3.termArray[j].coef;
                // 移動剩餘的項
                for (int k = j; k < poly3.terms - 1; k++) {
                    poly3.termArray[k] = poly3.termArray[k + 1];
                }
                poly3.terms--;
                j--; // 調整索引以重新檢查合併後的位置
            }
        }
    }

    // 按降冪排列
    for (int i = 0; i < poly3.terms - 1; i++) {
        for (int j = i + 1; j < poly3.terms; j++) {
            if (poly3.termArray[i].exp < poly3.termArray[j].exp) {
                Term temp = poly3.termArray[i];
                poly3.termArray[i] = poly3.termArray[j];
                poly3.termArray[j] = temp;
            }
        }
    }

    return poly3;
}

double Polynomial::Eval(double x) {
    double result = 0;
    for (int i = 0; i < terms; i++) {
        result += termArray[i].coef * pow(x, termArray[i].exp);
    }
    return result;
}


ostream& operator<<(ostream& out, const Polynomial& p) { // 多載 <<
    out << "(";
    out << p.termArray[0].coef << "X^" << p.termArray[0].exp;
    for (int i = 1; i < p.terms; i++) {
        if (p.termArray[i].coef > 0)
            out << " + " << p.termArray[i].coef << "X^" << p.termArray[i].exp;
        else
            out << " - " << p.termArray[i].coef * -1 << "X^" << p.termArray[i].exp;
    }
    out << ")";
    return out;
}

istream& operator>>(istream& in, Polynomial& p) { // 多載 >>
    double coeftemp = 0; // 暫存係數
    int exptemp = 0; // 暫存次方數
    char ch;

    while (1) {
        ch = in.peek(); // 檢查cin流上的第一個位元
        while (ch < '0' || ch > '9') { // 直到下一個位元是數字就代表是系數了就停下來準備輸入到coeftemp
            if (ch == '-') {
                in.get();
                ch = in.peek();
                while (ch < '0' && ch > '9') {
                    in.get();
                    ch = in.peek();
                }
                in >> coeftemp; // 讀取係數
                coeftemp *= -1;
                break;
            }
            in.get();
            ch = in.peek();
            if (ch >= '0' && ch <= '9') {
                in >> coeftemp; // 讀取係數
                break;
            }
        }
        bool check = true;

        ch = in.peek(); // 檢查cin流上的第一個位元
        while (ch < '0' || ch > '9') { // 反覆確認直到下一個位元是數字就代表是次方數了就停下來準備輸入到exptemp
            in.get();
            ch = in.peek();
        }
        in >> exptemp; // 讀取次方數
        p.NewTerm(coeftemp, exptemp); // 新增項目
        ch = in.peek(); // 檢查cin流上的第一個位元
        if (ch == ')') break;
        while (1) { // 反覆確認直到下一個位元，如果是 + or - 就代表後面還有至少一組要輸入，如果再過程中找到')'就代表已經結束輸入了
            in.get();
            ch = in.peek();
            if (ch == '+' || ch == '-') {
                break;
            }
            if (ch == ')') {
                check = false;
                break;
            }
        }
        if (!check)
            break;
    }
    in.get(); // 忽略換行符號（如果有的話）
    return in;
}

int main() {
    Polynomial poly1;
    Polynomial poly2;
    Polynomial poly3;
    Polynomial poly4;
    cout << "輸入範例:(5X^3+2X^2+1X^0) <== 括號一定要加(請使用降冪輸入!!!)" << endl;
    cout << "poly1:";
    cin >> poly1;
    cout << "poly2:";
    cin >> poly2;
    poly3 = poly1.Add(poly2);
    cout << endl << poly1 << " + " << poly2 << endl << "= " << poly3 << endl;
    poly4 = poly1.Mult(poly2);
    cout << endl << poly1 << " * " << poly2 << endl << "= " << poly4 << endl;
    double x=0; cout << "輸入一個值來計算多項式的值: ";
    cin >> x;
    cout << poly3 << ": x=" << "(" << x << ") = " << poly3.Eval(x) << endl;
    cout << poly4 << ": x=" << "(" << x << ") = " << poly4.Eval(x) << endl;
    return 0;
}
