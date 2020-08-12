#include<iostream>
using namespace std;

class A{
public:
    A(){
        cout << "A::A()" <<endl;
    }
    A(int i, double d){
        cout << "A::A(int, double)" << endl;
    }
    A(const A& a){
        cout << "A::(const A&) " << endl;
    }

    A(initializer_list<char> il){
        cout << "A::{}" << endl;
    }

    operator float() const{
        cout << "A::float() " << endl;
    }

};

int main(){
    // A a{2, 1.0}; // can be compiled with flag -Wno-narrowing
    // A a2{a};
    A a{};
    A a2({});
}