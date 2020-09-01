#include<iostream>
using namespace std;
/*
 * https://stackoverflow.com/questions/29164188/does-returning-a-local-object-require-move-semantics
 * http://www.cplusplus.com/forum/beginner/127815/
 * http://www.cplusplus.com/forum/beginner/127815/
*/

class A{
public:
    A(){
        cout << "A:A()" << endl;
    }

    A operator=(const A& a){
        cout << "copy assignment()" << endl;
        return *this;
    }

    // A(A&& a) = delete;   // error occurs in makeA() if this line is commented
    // A(A&& a) = default;
    /*
    A(A&& a){
        cout << "move constructor()" << endl;
    }
    */

    A(const A &a){
        cout << "copy constructor()" << endl;
    }

    ~A(){

    }
    int a;
};

A makeA(){
    A a;
    cout << &a << endl;
    return a;   // a is rvalue, in return statement
}
int main(){
    auto s = makeA();
    cout << &s << endl;
    return 0;
}