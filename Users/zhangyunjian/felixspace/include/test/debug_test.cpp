#include<iostream>
#include<vector>
#include<string>
#include "../include/AES2.hpp"
using namespace std;
int main() {
    AES aes;
    string plain = "AAAAAAAAAAAAAAAA";
    string cipher = aes.encode(plain.c_str());
    cout << "cipher: " << cipher << endl;
    string dec = aes.decode(cipher);
    cout << "dec length: " << dec.length() << endl;
    cout << "match: " << (plain == dec ? "yes" : "no") << endl;
    return 0;
}

