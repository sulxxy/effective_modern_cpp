#include<iostream>
#include<unordered_map>
#include<string>

using namespace std;

template<typename T>
class TD;

int main(){
    unordered_map<int, int> map;
    map.insert({1, 164});
    map.insert({2, 12});
    map.insert({3, 3});

    cout << "address of map: " << addressof(map) << endl;
    unsigned i = 0;
    // TD<decltype(map.cbegin())> iter_type;
    for (const pair<int, int>& p:map){
        cout << "addr of map[" << i++ << "]: " << addressof(p) << ", value: (" << p.first << ", " << p.second << ")" << endl;
    }

    i = 0;
    cout << "Using auto: " << endl;
    for (const auto& p : map){
        cout << "addr of map[" << i++ << "]: " << addressof(p) << ", value: (" << p.first << ", " << p.second << ")" << endl;
        TD<decltype(p)> p_type;
    }

    return 0;
}