#include<iostream>
#include<vector>
#include<string>
#include "../include/SM4.hpp"
using namespace std;
int main() {
    SM4 sm4;
    string plain = "你好世界";
    string cipher = sm4.encode(plain.c_str());
    cout << "cipher: " << cipher << endl;
    string dec = sm4.decode(cipher);
    cout << "dec: " << dec << endl;
    cout << "match: " << (plain == dec ? "OK" : "FAIL") << endl;
    return 0;
}
