#include <iostream>
#include <cmath>
using namespace std;

class Term {
public:
    int coef; // �t��
    int exp;  // ����
    Term* link; // ���V�U�@��������

    Term(int c = 0, int e = 0) : coef(c), exp(e), link(nullptr) {} // �غc�l�A��l�ƨt�ơB���Ƥ��쵲����
};

class Polynomial {
private:
    Term* header; // ����쵲��C�����Y�`�I

    // �M���h�������`�I
    void clear();
    // �ƻs�h���������e
    void copyFrom(const Polynomial& a);

public:
    // �غc�l�A��l�ƪŪ��h����
    Polynomial();
    // �ƻs�غc�l
    Polynomial(const Polynomial& a);
    // �Ѻc�l�A����O����
    ~Polynomial();
    // ��ȹB��l�A�`�׽ƻs�h����
    Polynomial& operator=(const Polynomial& a);
    // ��J�B��l�AŪ���h�������ഫ������쵲
    friend istream& operator>>(istream& is, Polynomial& x);
    // ��X�B��l�A�N�h�����ഫ���r��ÿ�X
    friend ostream& operator<<(ostream& os, const Polynomial& x);
    // �h�����[�k
    Polynomial operator+(const Polynomial& b) const;
    // �h������k�A�z�L�[�W�t�@�h�������t�ȹ�{
    Polynomial operator-(const Polynomial& b) const;
    // �h�������k
    Polynomial operator*(const Polynomial& b) const;
    // �p��h�����bx�B����
    double Evaluate(double x) const;
};

// �w�q Polynomial ���O�������禡

// �M���h�������`�I
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

// �ƻs�h���������e
void Polynomial::copyFrom(const Polynomial& a) {
    if (!a.header) return;
    header = new Term(); // ��l�ƪŪ����Y�`�I
    Term* tail = header; // �ΨӰl�[�s�`�I������
    Term* current = a.header->link;
    while (current != a.header) {
        Term* newTerm = new Term(current->coef, current->exp);
        tail->link = newTerm; // �s���s�`�I
        tail = newTerm; // ���ʧ��ݫ���
        current = current->link;
    }
    tail->link = header; // �]�m����쵲���c
}

// �غc�l�A��l�ƪŪ��h����
Polynomial::Polynomial() {
    header = new Term(); // �Ыت��Y�`�I
    header->link = header; // ���V�ۤv�A�Φ�����쵲
}

// �ƻs�غc�l
Polynomial::Polynomial(const Polynomial& a) : header(nullptr) {
    copyFrom(a);
}

// �Ѻc�l�A����O����
Polynomial::~Polynomial() {
    clear();
}

// ��ȹB��l
Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this != &a) {
        clear(); // ���M����e�h����
        copyFrom(a); // �M��ƻs�s�h����
    }
    return *this;
}


// �h�����[�k
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

        if (coef != 0) { // �u����Y�Ƥ����s�ɡA�~�K�[�쵲�G��
            Term* newTerm = new Term(coef, exp);
            tail->link = newTerm;
            tail = newTerm;
        }
    }
    tail->link = result.header; // ��������쵲
    return result;
}

// �h������k�A�z�L�[�W�t�@�h�������t�ȹ�{
Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial negB = b;
    Term* current = negB.header->link;
    while (current != negB.header) {
        current->coef = -current->coef; // ���t
        current = current->link;
    }
    return *this + negB; // �[�W�t���h����
}

// �h�������k
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
        result = result + temp; // �N�������G�ۥ[
        aCurrent = aCurrent->link;
    }
    return result;
}

// �p��h�����bx�B����
double Polynomial::Evaluate(double x) const {
    double result = 0;
    Term* current = header->link;
    while (current != header) {
        result += current->coef * pow(x, current->exp); // �p��C�@�����Ȩò֥[
        current = current->link;
    }
    return result;
}

// ��J�B��l�AŪ���h�������ഫ������쵲
istream& operator>>(istream& is, Polynomial& x) {
    x.clear(); // ���M����Ӫ��h����
    int n;
    is >> n; // Ū������
    if (n <= 0) return is;

    x.header = new Term(); // �ЫتŪ����Y�`�I
    Term* tail = x.header;

    // Ū���C�@�����Y�ƩM���ơA���쵲��h������
    for (int i = 0; i < n; ++i) {
        int c, e;
        is >> c >> e;
        Term* newTerm = new Term(c, e);
        tail->link = newTerm;
        tail = newTerm;
    }
    tail->link = x.header; // ��������쵲
    return is;
}

// ��X�B��l�A�N�h�����ഫ���r��ÿ�X
ostream& operator<<(ostream& os, const Polynomial& x) {
    if (!x.header || x.header->link == x.header) { // �p�G�h��������
        os << "0";
        return os;
    }

    Term* current = x.header->link;
    bool first = true;
    while (current != x.header) { // �M���h�������C�@��
        if (!first && current->coef > 0) os << "+"; // �Y���O�Ĥ@���B�Y�Ƭ����A�[�W��+��
        os << current->coef << "x^" << current->exp;
        current = current->link;
        if (current != x.header) os << " "; // �Y���O�̫�@���A�h�[�W�Ů�
        first = false;
    }
    return os;
}


int main() {
    Polynomial p1, p2;
    cout << "�п�J�Ĥ@�Ӧh���� (�榡: n c1 e1 c2 e2 ...): ";
    cin >> p1;
    cout << "�п�J�ĤG�Ӧh���� (�榡: n c1 e1 c2 e2 ...): ";
    cin >> p2;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "�Ĥ@�Ӧh���� P1: " << p1 << endl;
    cout << "�ĤG�Ӧh���� P2: " << p2 << endl;
    cout << "�h�����[�k���G: " << sum << endl;
    cout << "�h������k���G: " << diff << endl;
    cout << "�h�������k���G: " << prod << endl;

    double x;
    cout << "�п�J x ���ȥH�p��U�Ӧh��������: ";
    cin >> x;

    // �p�����ܩҦ��h��������
    cout << "P1(" << x << ") = " << p1.Evaluate(x) << endl;
    cout << "P2(" << x << ") = " << p2.Evaluate(x) << endl;
    cout << "P1 + P2(" << x << ") = " << sum.Evaluate(x) << endl;
    cout << "P1 - P2(" << x << ") = " << diff.Evaluate(x) << endl;
    cout << "P1 * P2(" << x << ") = " << prod.Evaluate(x) << endl;

    return 0;
}
