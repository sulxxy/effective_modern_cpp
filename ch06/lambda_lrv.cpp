#include <iostream>

void foo(int& t){
    std::cout << "foo for lvalue" << std::endl;
    t++;
    return;
}

void foo(int&& t){
    std::cout << "foo for rvalue" << std::endl;
    t--;
    return;
}

int main(){
    int a = 2;
    auto f = [](auto x) { return foo(x); };
    f(3);
    f(a);
    std::cout << a << std::endl;

    a = 2;
    auto f2 = [] (auto&& x) { return foo(std::forward<decltype(x)>(x));};
    f2(3);
    f2(a);
    std::cout << a << std::endl;
    return 0;
}