template<typename T>
class TypeDisplayer;

template<typename T>
void f1(T& param){
    TypeDisplayer<decltype(param)> paramType;
}
void case1(){
    int x = 131;
    int& rx = x;
    const int cx = 10;
    const int& crx = cx;
    int* px = &x;
    int arr[] = {1,2,3};
    // f1(x);   // int&
    // f1(rx);    // int&
    // f1(cx);     // const int&
    // f1(crx);  // const int&
    // f1(px);      // int*&
    f1(arr);       // int (&)[3]
}

template<typename T>
void f1pt(T* param){
    TypeDisplayer<decltype(param)> paramType;
}
void case1pt(){
    int x = 131;
    int& rx = x;
    int* px = &x;
    // f1pt(x);   // template argument deduction failed
    // f1pt(rx);    // template argument deduction failed
    // f1pt(px);      // int*
    // f1pt(&x);         // int*
}

template<typename T>
void f2(T&& param){
    TypeDisplayer<decltype(param)> paramType;
}
void case2(){
    int x = 131;
    int& rx = x;
    const int cx = 10;
    const int& crx = cx;
    // f2(x);   // int&
    // f2(rx);    // int&
    // f2(cx);     // const int&
    // f2(crx);  // const int&
}

template<typename T>
void f3(T param){
    TypeDisplayer<decltype(param)> paramType;
}
void case3(){
    int x = 131;
    int* px = &x;
    const int* cpx = &x;
    const int* const cpcx = &x;
    int& rx = x;
    const int cx = 10;
    const int& crx = cx;
    // f3(x);   // int
    // f3(rx);    // int
    // f3(cx);     // int
    // f3(crx);  // int
    // f3(px);        // int*
    // f3(cpx);         // const int*
    // f3(cpcx);          // const int*

}