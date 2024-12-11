#include <iostream>
using namespace std;

// Term 類別定義
class Term {
    friend class Polynomial; // 讓 Polynomial 類別訪問私有成員
private:
    float coef; // 項的係數
    int exp;    // 項的指數
public:
    Term(float c = 0, int e = 0) : coef(c), exp(e) {}
    float getCoef() const { return coef; }
    int getExp() const { return exp; }
    void setCoef(float c) { coef = c; }
    void setExp(int e) { exp = e; }
};

// Polynomial 類別定義
class Polynomial {
    friend ostream& operator<<(ostream& os, const Polynomial& p);
    friend istream& operator>>(istream& is, Polynomial& p);

private:
    Term* termArray; // 存放多項式項目
    int capacity;    // 最大容量
    int terms;       // 當前項目數量

public:
    Polynomial() {
        terms = 0;
        capacity = 10;
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }

    // 自定義賦值運算子
    Polynomial& operator=(const Polynomial& poly) {
        if (this == &poly) { // 防止自我賦值
            return *this;
        }
        delete[] termArray;
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            termArray[i] = poly.termArray[i];
        }
        return *this;
    }

    // 多項式相加
    Polynomial Add(const Polynomial& poly) {
        Polynomial result;
        int i = 0, j = 0;

        while (i < this->terms && j < poly.terms) {
            if (this->termArray[i].getExp() == poly.termArray[j].getExp()) {
                // 係數相加
                float newCoef = this->termArray[i].getCoef() + poly.termArray[j].getCoef();
                if (newCoef != 0) { // 只加不為零的項
                    result.addTerm(newCoef, this->termArray[i].getExp());
                }
                i++;
                j++;
            } else if (this->termArray[i].getExp() > poly.termArray[j].getExp()) {
                result.addTerm(this->termArray[i].getCoef(), this->termArray[i].getExp());
                i++;
            } else {
                result.addTerm(poly.termArray[j].getCoef(), poly.termArray[j].getExp());
                j++;
            }
        }

        // 處理剩餘的項
        while (i < this->terms) {
            result.addTerm(this->termArray[i].getCoef(), this->termArray[i].getExp());
            i++;
        }
        while (j < poly.terms) {
            result.addTerm(poly.termArray[j].getCoef(), poly.termArray[j].getExp());
            j++;
        }

        return result;
    }

    // 多項式相乘
    Polynomial Mult(const Polynomial& poly) {
        Polynomial result;
        for (int i = 0; i < this->terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                // 係數相乘，指數相加
                float newCoef = this->termArray[i].getCoef() * poly.termArray[j].getCoef();
                int newExp = this->termArray[i].getExp() + poly.termArray[j].getExp();
                if (newCoef != 0) { // 只加不為零的項
                    result.addOrCombine(newCoef, newExp);
                }
            }
        }
        return result;
    }

    // 計算多項式在某點的值
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++) {
            float termValue = 1;
            for (int j = 0; j < termArray[i].getExp(); j++) {
                termValue *= x; // 手動計算次方
            }
            result += termArray[i].getCoef() * termValue;
        }
        return result;
    }

private:
    // 添加或合併項
    void addOrCombine(float coef, int exp) {
        for (int i = 0; i < terms; i++) {
            if (termArray[i].getExp() == exp) {
                termArray[i].setCoef(termArray[i].getCoef() + coef);
                if (termArray[i].getCoef() == 0) { // 移除係數為 0 的項
                    removeTerm(i);
                }
                return;
            }
        }
        addTerm(coef, exp);
    }

    // 添加新項
    void addTerm(float coef, int exp) {
        if (terms == capacity) {
            capacity *= 2;
            Term* newTermArray = new Term[capacity];
            for (int i = 0; i < terms; i++) {
                newTermArray[i] = termArray[i];
            }
            delete[] termArray;
            termArray = newTermArray;
        }
        termArray[terms++] = Term(coef, exp);
    }

    // 移除項 (當係數為 0 時)
    void removeTerm(int index) {
        for (int i = index; i < terms - 1; i++) {
            termArray[i] = termArray[i + 1];
        }
        terms--;
    }
};

// 重設 >> 運算子
istream& operator>>(istream& is, Polynomial& p) {
    int n;
    cout << "Enter the number of terms: ";
    is >> n;
    for (int i = 0; i < n; ++i) {
        float coef;
        int exp;
        cout << "Enter coefficient and exponent for term " << i + 1 << ": ";
        is >> coef >> exp;
        p.addTerm(coef, exp);
    }
    return is;
}

// 重設 << 運算子
ostream& operator<<(ostream& os, const Polynomial& p) {
    bool firstTerm = true;
    for (int i = 0; i < p.terms; ++i) {
        if (p.termArray[i].getCoef() == 0) continue;

        if (!firstTerm && p.termArray[i].getCoef() > 0) {
            os << " + ";
        }

        if (p.termArray[i].getCoef() < 0) {
            os << p.termArray[i].getCoef();
        } else if (firstTerm) {
            os << p.termArray[i].getCoef();
        } else {
            os << p.termArray[i].getCoef();
        }

        if (p.termArray[i].getExp() > 0) {
            os << "x^" << p.termArray[i].getExp();
        }

        firstTerm = false;
    }
    if (firstTerm) {
        os << "0";
    }
    return os;
}

int main() {
    Polynomial poly1, poly2, result;

    // 輸入多項式
    cout << "Enter the first polynomial:" << endl;
    cin >> poly1;

    cout << "Enter the second polynomial:" << endl;
    cin >> poly2;

    // 輸出多項式
    cout << "First Polynomial: " << poly1 << endl;
    cout << "Second Polynomial: " << poly2 << endl;

    // 計算和
    result = poly1.Add(poly2);
    cout << "Sum: " << result << endl;

    // 計算積
    result = poly1.Mult(poly2);
    cout << "Product: " << result << endl;

    // 計算指定值的結果
    float value;
    cout << "Enter a value to evaluate the first polynomial: ";
    cin >> value;
    cout << "P1(" << value << ") = " << poly1.Eval(value) << endl;

    return 0;
}
