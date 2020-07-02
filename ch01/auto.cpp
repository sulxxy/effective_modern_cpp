#include<iostream>
template<typename T>
class TypeDisplayer;

void case1(){
    int x = 131;
    int& rx = x;
    const int cx = 10;
    const int& crx = cx;

    auto& ax = x;    // int&
    auto& arx = rx;  // int&
    auto& acx = cx;  // const int&
    auto& acrx = crx; //const int&

    // TypeDisplayer<decltype(acrx)> ax_type;

}

void case2(){
    int x = 131;
    int& rx = x;
    const int cx = 10;
    const int& crx = cx;
    auto&& ax = x;    // int&
    auto&& arx = rx;  // int&
    auto&& acx = cx;  // const int&
    auto&& acrx = crx; //const int&
    // TypeDisplayer<decltype(acrx)> ax_type;
}

void case3(){
    int x = 131;
    int& rx = x;
    const int cx = 10;
    const int& crx = cx;
    auto ax = x;    // int
    auto arx = rx;  // int
    auto acx = cx;  // int
    auto acrx = crx; //int
    // TypeDisplayer<decltype(acrx)> ax_type;
}

void all_cases(){
    auto x = 131; // case 3, x is int
    const auto cx = 131; // case 3, cx is const int
    const auto& crx = x; // case 1, crx is const int&
    auto ccx = cx; // ccx is int
    auto& rcx = cx; // rcx is const int&
    int arr[] = {1,2,3};
    auto aarr = arr;    // aarr int *
    auto& ararr = arr;  // ararr is int (&)[3]
    // TypeDisplayer<decltype(ararr)> ax_type;
}
