#include<iostream>
using namespace std;

class A;

void f(int val){
    cout << "call f(int)" << endl;
}

void f(A* a){
    cout << "call f(A*)" << endl;
}

int main(){
    f(NULL);
    f(nullptr);
}