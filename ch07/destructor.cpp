#include<thread>
#include<iostream>

using namespace std;

void f(){
    cout << "f1 " << endl;
}


int main(){
    {
        thread t(f);
    }
    while(true){
        cout << "ddd" << endl;
    }
    return 0;
}