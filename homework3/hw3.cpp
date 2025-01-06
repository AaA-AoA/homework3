#include<iostream>
#include<cmath>
#include<string>
using namespace std;

struct Node{
    int coef;  // 係數
    int exp;   // 指數
    Node* link; // 下一節點
};

class Polynomial{
    private:
        Node* header; // link 的頭
        void AddTerm(int coef, int exp); // 新增多項式
        void Clear(); // 清空多項式
    public:
        Node* getHeader() const { return header; }
        Polynomial(); // 建構子
        Polynomial(const Polynomial& a); // copy 建構子
        ~Polynomial(); // 解構子
        Polynomial& operator=(const Polynomial& a); // 值的運算
        Polynomial operator+(const Polynomial& b) const; // 加法
        Polynomial operator-(const Polynomial& b) const; // 減法
        Polynomial operator*(const Polynomial& b) const; // 乘法
        float Evaluate(float x) const; // 多項式的值

        friend istream& operator>>(istream& is, Polynomial& x); // 輸入
        friend ostream& operator<<(ostream& os, const Polynomial& x); // 輸出
};

// 預設建構子，初始化多項式，建立一個表頭節點，並使其指向自己 (形成循環)
Polynomial::Polynomial() : header(new Node{0, 0, nullptr}) {
    header->link = header;
}

// 複製建構子，利用賦值運算子完成複製
Polynomial::Polynomial(const Polynomial& a) : Polynomial() {
    *this = a;
}

// 解構子，釋放記憶體，清空多項式並刪除表頭節點
Polynomial::~Polynomial() {
    Clear();
    delete header;
}

// 賦值運算子，將多項式 a 賦值給當前多項式
Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this != &a) {  // 避免自我賦值
        Clear(); // 先清除當前多項式
        for (Node* p = a.header->link; p != a.header; p = p->link) {
            AddTerm(p->coef, p->exp); // 複製 a 的每一項到當前多項式
        }
    }
    return *this;
}

// 多項式加法運算，返回當前多項式與 b 的和
Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial result; // 存放結果的多項式
    Node* p1 = header->link;
    Node* p2 = b.header->link;

    // 遍歷兩個多項式，按照指數大小逐項相加
    while (p1 != header || p2 != b.header) {
        if (p1 == header || (p2 != b.header && p2->exp > p1->exp)) {
            result.AddTerm(p2->coef, p2->exp); // 加入 p2 的項
            p2 = p2->link;
        } else if (p2 == b.header || p1->exp > p2->exp) {
            result.AddTerm(p1->coef, p1->exp); // 加入 p1 的項
            p1 = p1->link;
        } else {
            int newCoef = p1->coef + p2->coef; // 指數相同，係數相加
            if (newCoef) result.AddTerm(newCoef, p1->exp);
            p1 = p1->link;
            p2 = p2->link;
        }
    }
    return result;
}

// 多項式減法運算，返回當前多項式與 b 的差
Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial neg_b(b); // 複製 b
    for (Node* p = neg_b.header->link; p != neg_b.header; p = p->link) {
        p->coef = -p->coef; // 將 b 的每一項變號
    }
    return *this + neg_b; // 呼叫加法運算子
}

// 多項式乘法運算，返回當前多項式與 b 的乘積
Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial result;
    for (Node* p1 = header->link; p1 != header; p1 = p1->link) {
        for (Node* p2 = b.header->link; p2 != b.header; p2 = p2->link) {
            result.AddTerm(p1->coef * p2->coef, p1->exp + p2->exp);
        }
    }
    return result;
}

// 計算多項式在 x 處的值
float Polynomial::Evaluate(float x) const {
    float result = 0;
    for (Node* p = header->link; p != header; p = p->link) {
        result += p->coef * pow(x, p->exp); // 逐項累加
    }
    return result;
}

// 新增項目到多項式中
void Polynomial::AddTerm(int coef, int exp) {
    if (!coef) return;
    Node* p = header;
    while (p->link != header && p->link->exp > exp) p = p->link;
    if (p->link != header && p->link->exp == exp) {
        p->link->coef += coef;
        if (!p->link->coef) { // 如果係數變為 0，刪除此項
            Node* temp = p->link;
            p->link = temp->link;
            delete temp;
        }
    } else {
        p->link = new Node{coef, exp, p->link}; // 插入新項
    }
}

// 清空多項式，釋放所有項目
void Polynomial::Clear() {
    while (header->link != header) {
        Node* temp = header->link;
        header->link = temp->link;
        delete temp;
    }
}

// 多項式輸入
istream& operator>>(istream& is, Polynomial& x) {
    x.Clear();
    int n, coef, exp;
    cout << "Enter number of terms: ";
    is >> n;
    while (n--) {
        cout << "Enter coefficient and exponent: ";
        is >> coef >> exp;
        x.AddTerm(coef, exp);
    }
    return is;
}

// 多項式輸出
ostream& operator<<(ostream& os, const Polynomial& x) { // 修改此處，const 引用
    bool first = true;
    for (Node* p = x.getHeader()->link; p != x.getHeader(); p = p->link) {
        if (!first) os << (p->coef > 0 ? " + " : " - ");
        if (abs(p->coef) != 1 || p->exp == 0) os << abs(p->coef);
        if (p->exp) os << "x" << (p->exp != 1 ? "^" + std::to_string(p->exp) : "");
        first = false;
    }
    if (first) os << "0";
    return os;
}

int main() {
    Polynomial p1, p2; // 宣告兩個 Polynomial 物件 p1 和 p2，表示兩個多項式
    cout << "Enter the first polynomial:" << endl;
    cin >> p1; // 輸入第一個多項式

    cout << "Enter the second polynomial:" << endl;
    cin >> p2; // 輸入第二個多項式

    // 輸出兩個多項式
    cout << "First polynomial: " << p1 << endl;
    cout << "Second polynomial: " << p2 << endl;

    // 計算並輸出兩個多項式的和、差和積
    cout << "Sum: " << (p1 + p2) << endl;
    cout << "Difference: " << (p1 - p2) << endl;
    cout << "Product: " << (p1 * p2) << endl;

    // 評估第一個多項式在特定 x 值下的結果
    float x;
    cout << "Enter a value to evaluate the first polynomial: ";
    cin >> x;
    cout << "Value of the first polynomial at " << x << ": " << p1.Evaluate(x) << endl;

    return 0; // 程式結束
}
