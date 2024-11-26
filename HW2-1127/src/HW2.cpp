#include <iostream>
using namespace std;

class Polynomial;
class Term {
    friend Polynomial;
    friend istream& operator>>(istream& in, Polynomial& p); // �h�� >>
    friend ostream& operator<<(ostream& out, const Polynomial& p); // �h�� <<
private:
    double coef; // �Y��
    int exp; // �����
};

class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& p); // �h�� >>
    friend ostream& operator<<(ostream& out, const Polynomial& p); // �h�� <<
private:
    Term* termArray; // �D�s�����}�C
    int capacity; // termArray ���j�p
    int terms; // �D�s�����ƶq
public:
    Polynomial(); // �غc�l
    Polynomial Add(Polynomial poly); // �[�k
    Polynomial Mult(Polynomial poly); // ���k
    double Eval(double f); // �p���`��
    void NewTerm(const double newCoef, const int newExp); // �s�W�s����
};

Polynomial::Polynomial() : capacity(2), terms(0) { // �غc�l
    termArray = new Term[capacity];
}

void Polynomial::NewTerm(const double newCoef, const int newExp) { // �s�W�s����
    if (terms == capacity) { // �Ŷ�����
        capacity *= 2; // �Ŷ�½��
        Term* tmp = new Term[capacity];
        copy(termArray, termArray + terms, tmp);
        delete[] termArray; // �R����}�C�A����O����
        termArray = tmp;
    }
    termArray[terms].coef = newCoef; // ��J�Y��
    termArray[terms++].exp = newExp; // ��J�����
}

Polynomial Polynomial::Add(Polynomial b) { // �[�k
    Polynomial poly3;
    int c1 = terms; // poly1������
    int c2 = b.terms; // poly2������
    int i1 = 0; // poly1�B�z��ĴX��
    int i2 = 0; // poly2�B�z��ĴX��
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

    // �X�֬ۦP���誺��
    for (int i = 0; i < poly3.terms; i++) {
        for (int j = i + 1; j < poly3.terms; j++) {
            if (poly3.termArray[i].exp == poly3.termArray[j].exp) {
                poly3.termArray[i].coef += poly3.termArray[j].coef;
                // ���ʳѾl����
                for (int k = j; k < poly3.terms - 1; k++) {
                    poly3.termArray[k] = poly3.termArray[k + 1];
                }
                poly3.terms--;
                j--; // �վ���ޥH���s�ˬd�X�᪺֫��m
            }
        }
    }

    // �������ƦC
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


ostream& operator<<(ostream& out, const Polynomial& p) { // �h�� <<
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

istream& operator>>(istream& in, Polynomial& p) { // �h�� >>
    double coeftemp = 0; // �Ȧs�Y��
    int exptemp = 0; // �Ȧs�����
    char ch;

    while (1) {
        ch = in.peek(); // �ˬdcin�y�W���Ĥ@�Ӧ줸
        while (ch < '0' || ch > '9') { // ����U�@�Ӧ줸�O�Ʀr�N�N��O�t�ƤF�N���U�ӷǳƿ�J��coeftemp
            if (ch == '-') {
                in.get();
                ch = in.peek();
                while (ch < '0' && ch > '9') {
                    in.get();
                    ch = in.peek();
                }
                in >> coeftemp; // Ū���Y��
                coeftemp *= -1;
                break;
            }
            in.get();
            ch = in.peek();
            if (ch >= '0' && ch <= '9') {
                in >> coeftemp; // Ū���Y��
                break;
            }
        }
        bool check = true;

        ch = in.peek(); // �ˬdcin�y�W���Ĥ@�Ӧ줸
        while (ch < '0' || ch > '9') { // ���нT�{����U�@�Ӧ줸�O�Ʀr�N�N��O����ƤF�N���U�ӷǳƿ�J��exptemp
            in.get();
            ch = in.peek();
        }
        in >> exptemp; // Ū�������
        p.NewTerm(coeftemp, exptemp); // �s�W����
        ch = in.peek(); // �ˬdcin�y�W���Ĥ@�Ӧ줸
        if (ch == ')') break;
        while (1) { // ���нT�{����U�@�Ӧ줸�A�p�G�O + or - �N�N��᭱�٦��ܤ֤@�խn��J�A�p�G�A�L�{�����')'�N�N��w�g������J�F
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
    in.get(); // ��������Ÿ��]�p�G�����ܡ^
    return in;
}

int main() {
    Polynomial poly1;
    Polynomial poly2;
    Polynomial poly3;
    Polynomial poly4;
    cout << "��J�d��:(5X^3+2X^2+1X^0) <== �A���@�w�n�[(�Шϥέ�����J!!!)" << endl;
    cout << "poly1:";
    cin >> poly1;
    cout << "poly2:";
    cin >> poly2;
    poly3 = poly1.Add(poly2);
    cout << endl << poly1 << " + " << poly2 << endl << "= " << poly3 << endl;
    poly4 = poly1.Mult(poly2);
    cout << endl << poly1 << " * " << poly2 << endl << "= " << poly4 << endl;
    double x=0; cout << "��J�@�ӭȨӭp��h��������: ";
    cin >> x;
    cout << poly3 << ": x=" << "(" << x << ") = " << poly3.Eval(x) << endl;
    cout << poly4 << ": x=" << "(" << x << ") = " << poly4.Eval(x) << endl;
    return 0;
}
