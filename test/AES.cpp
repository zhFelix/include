#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include "../include/AES2.hpp"
using namespace std;
int main() {
	int mode;
	scanf("%d", &mode);
	string tmp="";
	getline(cin, tmp);
	if(mode==0) {
		// 加密：输入明文 + 密钥(可选，留空自动随机)
		// 输出：密钥(Base64) + 密文(Base64)
		string msg, key_b64;
		getline(cin, msg);
		getline(cin, key_b64);
		AES aes;
		auto [cipher, k] = aes.encode(msg.c_str(), key_b64);
		cout << k << endl << cipher;
	} else if(mode==1) {
		// 解密：输入密钥(Base64) + 密文(Base64)
		// 输出：明文
		string key_b64, cipher;
		getline(cin, key_b64);
		getline(cin, cipher);
		AES aes;
		cout << aes.decode(cipher, key_b64);
	}
	return 0;
}
