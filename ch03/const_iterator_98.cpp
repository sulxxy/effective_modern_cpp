#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int non_const_version(){
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(4);
    vector<int>::iterator it = find(vec.begin(), vec.end(), 4);
    vec.insert(it, 3);
    for (vector<int>::iterator beg=vec.begin(); beg != vec.end(); ++beg){
        cout << *beg << '\t';
    }
    cout << endl;
}

int const_version(){
    typedef std::vector<int>::iterator IterT;
    typedef std::vector<int>::const_iterator ConstIterT;
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(4);
    ConstIterT it = find(static_cast<ConstIterT>(vec.begin()), static_cast<ConstIterT>(vec.end()), 4);
    vec.insert(static_cast<IterT>(it), 3);  // unable to convert
}

int main(){
    non_const_version();
}