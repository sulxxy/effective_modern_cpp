#include <iostream>
using namespace std;

int func1(){
    int val = 10;
    auto f1 = [val] () mutable { return ++val; };
    auto res = f1();
    cout << res << endl;  // 11
    cout << val << endl;  // 10
    return 0;
}

int func2(){
    int val = 10;
    auto f2 = [&val] { return ++val; };
    auto res = f2();
    cout << res << endl; // 11
    cout << val << endl; // 11
    return 0;
}

int main(){
    func1();
    func2();
}