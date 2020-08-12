#include<iostream>

using namespace std;

class Base{
public:
    virtual void foo(){
        cout << "this is base" << endl;
    }
};

class A : public Base{
public:
    virtual void foo() const{
        cout << "this is A" << endl;
    }
};

int main(){
    Base* pa = new A();
    pa->foo();
}