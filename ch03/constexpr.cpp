#include<iostream>

constexpr long int fib(int n){
    return (n <= 1) ? n : fib(n-1) + fib(n-2);
}

int main(){
    long int a = fib(45);
    std::cout << a << std::endl;
    return 0;
}