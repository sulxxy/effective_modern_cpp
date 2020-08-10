#include<iostream>
class A{
public:
    int var = 10;
};

int main(){
    A a;
    std::cout << a.var;
    return 0;
}