#include<iostream>
using namespace std;

class A{
public:
    A(){
        cout << "constructor" << endl;
    }

    A(const A& a){
        cout << "copy constructor" << endl;
    }

    A(A&& a){
        cout << "move constructor" << endl;
    }
    
    // A& operator=(A&& a){
        // cout << "move assignment" << endl;
    // }
};

int main(){
    A a1;
    A a2(move(a1));
    A a3(a1);
    a3 = a1;
}