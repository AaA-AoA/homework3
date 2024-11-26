//HW1 problem1

#include<iostream>
using namespace std;

int ack_r(int m,int n){
    if(m==0){
        return n+1;
    }
    else if(m>0 && n==0){
        return ack_r(m-1,1);
    }
    else{
        return ack_r(m-1,ack_r(m,n-1));
    }
}

int ack_nr(int m, int n) {
    while (m != 0) {
        if (m > 0 && n == 0) {
            // 當m > 0且n == 0時
            m = m - 1;
            n = 1;
        } else if (m > 0 && n > 0) {
            // 當m > 0且n > 0时
            int temp_n = n;
            n = n - 1;
            m = m - 1;
            //(m-1, ackermann(m, n-1))
            n = ack_nr(m + 1, temp_n - 1);  //手動展開遞歸
        }
    }
    return n + 1;  // 當m == 0時，直接返回n+1
}

int main(){
    int m,n;
    cout<<"input m n:";
    cin>>m>>n;
    cout<<"Ack_r("<<m<<","<<n<<") = "<<ack_r(m,n)<<endl;
    cout<<"Ack_nr("<<m<<","<<n<<") = "<<ack_nr(m,n)<<endl;
    return 0;
}