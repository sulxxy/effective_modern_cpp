#include<iostream>

namespace std{
    int distance(int a, int b){  // can be compiled on c++11, but not c++98
        return a - b;
    }
}

// g++ -dM -E -x c++  /dev/null | grep -F __cplusplus
// 201402L
int main(){
    std::distance(3, 2);
    return 0;
}

