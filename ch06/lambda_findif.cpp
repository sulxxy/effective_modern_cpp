#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<functional>
using namespace std;

bool check_size(const string& s, string::size_type sz){
    return s.size() >= sz;
}

int test_bind(){
    vector<string> vec{"abc", "a", "abcd", "abcde", "ab"};
    string::size_type sz{3};
    auto res = find_if(vec.cbegin(), vec.cend(), bind(check_size, placeholders::_1, sz));
    cout << *res << endl;
    return 0;
}

int test_lambda(){
    vector<string> vec{"abc", "a", "abcd", "abcde", "ab"};
    string::size_type sz{3};
    auto f = [=] (const string& s)->bool { return s.size() >= sz; };
    auto res = find_if(vec.cbegin(), vec.cend(), f);
    cout << *res << endl;
    return 0;
}

int main(){
    test_lambda();
}