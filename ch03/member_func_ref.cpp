#include<iostream>

using namespace std;

class Var{
public:
    Var(){
        cout << "construct a Var object" << endl;
    }

    Var(const Var& v){
        cout << "copy var" << endl;
    }

    // Var(Var&& v){
        // cout << "moving var" << endl;
    // }
};

class A{
public:
    Var& get_v() & {
        return v;
    }

    Var get_v() && {
        return move(v);
    }

private:
    Var v;
};

A makeA(){
    A a;
    return a;
}

class Foo{
    public:
    void foo() &{
        cout << "called by lvalue" << endl;
    }

    void foo() &&{
        cout << "called by rvalue" << endl;
    }
};

int main(){
    Foo f;
    f.foo();
    move(f).foo();
    auto a = makeA().get_v();
}