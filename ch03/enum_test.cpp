#include<iostream>
#include<cstdint>

enum Color:std::uint8_t{  // can be compiled on c++11, but not 98
    red,
    white,
    blue
};

enum class XT;

class A;

int main(){
    std::uint32_t col = red;
    std::cout << col << std::endl;
    std::cout << sizeof(XT) << std::endl;
    std::cout << sizeof(Color) << std::endl;
}