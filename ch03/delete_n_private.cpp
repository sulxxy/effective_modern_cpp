#include<iostream>
using namespace std;


class A{
public:
    A(){
        cout << "A::A()" << endl;
        // f();
        g();
    }

private:
    void f();
    void g() = delete;
};

int main(){
    A a;
}