#include<iostream>
#include<vector>
#include<string>
#include "../include/SM4.hpp"
using namespace std;
int main(int argc, char** argv) {
    if(argc==1) {
        SM4 sm4;
        string plain = "你好世界";
        string cipher = sm4.encode(plain.c_str());
        cout << cipher << endl;
    } else {
        string input;
        getline(cin, input);
        SM4 sm4;
        cout << sm4.decode(input) << endl;
    }
    return 0;
}
