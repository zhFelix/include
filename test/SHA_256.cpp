#include<iostream>
#include<string>
#include<bitset>
#include<vector>
#include "../include/SHA_256.hpp"
using namespace std;
int main() {
    SHA_256 sha256;
    
    string t1 = sha256.hash("");
    cout << "empty: " << t1 << endl;
    cout << "expect: e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855" << endl;
    cout << endl;
    
    string t2 = sha256.hash("abc");
    cout << "abc:   " << t2 << endl;
    cout << "expect: ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad" << endl;
    cout << endl;
    
    string t3 = sha256.hash("hello world");
    cout << "hello: " << t3 << endl;
    cout << "expect: b94d27b9934d3e08a52e52d7da7dabfac484efe37a5380ee9088f7ace2efcde9" << endl;
    
    return 0;
}
