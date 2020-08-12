#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int main(){
    vector<int> vec{1,2,4};
    auto it = find(vec.cbegin(), vec.cend(), 4);
    vec.insert(it, 3);
    for (auto i : vec){
        cout << i << '\t';
    }
    cout << endl;
}