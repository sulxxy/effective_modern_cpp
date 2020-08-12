#include<iostream>
using namespace std;

class Var{
public:
    Var() = default;
    Var(Var&& v){
        cout << "Var::move constructor" << endl;
    }

    Var(const Var& v){
        cout << "Var::copy constructor" << endl;
    }
};

class A{
public:
    A(){
        cout << "constructor" << endl;
    }

    // A(A&& a) = default;

    // ~A(){
        // cout << "A::destructor" << endl;
    // }
private:
    Var v;
};

int main(){
    A a1;
    A a3(move(a1));
}