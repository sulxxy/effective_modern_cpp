#include<iostream>
using namespace std;


void foo(){
    static int a = 10;
    
    auto f = [=] {return a; };
    a++;
    return;
}

int main(){

}