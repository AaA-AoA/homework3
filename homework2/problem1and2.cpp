#include<iostream>
using namespace std;

class Term {
    friend class Polynomial;
private:
    float coef; // 係數
    int exp;    // 指數
};

class Polynomial {
private:
    Term* termArray; // 非零項的數組
    int capacity;    // 數組大小
    int terms;       // 非零項數量

public:
    // 初始化空多項式
    Polynomial() : capacity(10), terms(0) {
        termArray = new Term[capacity];
    }

    // 添加多項式
    Polynomial Add(const Polynomial& poly) {
        Polynomial result;
        int i = 0, j = 0;

        // 合併兩個多項式的項
        while (i < terms && j < poly.terms) {
            if (termArray[i].exp == poly.termArray[j].exp) {
                float newCoef = termArray[i].coef + poly.termArray[j].coef;
                if (newCoef != 0) {
                    result.addTerm(newCoef, termArray[i].exp);
                }
                i++;
                j++;
            } else if (termArray[i].exp > poly.termArray[j].exp) {
                result.addTerm(termArray[i].coef, termArray[i].exp);
                i++;
            } else {
                result.addTerm(poly.termArray[j].coef, poly.termArray[j].exp);
                j++;
            }
        }

        // 添加剩餘項
        while (i < terms) {
            result.addTerm(termArray[i].coef, termArray[i].exp);
            i++;
        }
        while (j < poly.terms) {
            result.addTerm(poly.termArray[j].coef, poly.termArray[j].exp);
            j++;
        }

        return result;
    }

    // 多項式乘法
    Polynomial Mult(const Polynomial& poly) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;
                result.addTerm(newCoef, newExp);
            }
        }
        return result;
    }

    // 多項式求值
    float Eval(float x) {
        float result = 0;
        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef * power(x, termArray[i].exp);
        }
        return result;
    }

    // 添加一個項到多項式
    void addTerm(float coef, int exp) {
        if (terms >= capacity) {
            capacity *= 2;
            Term* newArray = new Term[capacity];
            for (int i = 0; i < terms; i++) {
                newArray[i] = termArray[i];
            }
            delete[] termArray;
            termArray = newArray;
        }
        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        terms++;
    }

    // 輸出多項式
    void print() {
        for (int i = 0; i < terms; i++) {
            if (i > 0 && termArray[i].coef > 0) cout << " + ";
            cout << termArray[i].coef << "x^" << termArray[i].exp;
        }
        cout << endl;
    }

    // 計算 x^y
    float power(float x, int y) {
        float result = 1;
        for (int i = 0; i < y; i++) {
            result *= x;
        }
        return result;
    }
};

int main() {
    Polynomial p1, p2;

    // 添加數據到多項式 p1 和 p2
    p1.addTerm(3, 2); // 3x^2
    p1.addTerm(4, 0); // +4
    p2.addTerm(1, 1); // x
    p2.addTerm(2, 0); // +2

    // 輸出多項式
    cout << "P1: ";
    p1.print();
    cout << "P2: ";
    p2.print();

    // 加法測試
    Polynomial sum = p1.Add(p2);
    cout << "Sum: ";
    sum.print();

    // 乘法測試
    Polynomial product = p1.Mult(p2);
    cout << "Product: ";
    product.print();

    // 求值測試
    cout << "P1(2): " << p1.Eval(2) << endl;

    return 0;
}
