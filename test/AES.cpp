#include<iostream>
#include<vector>
#include<string>
#include<cstdio>
#include "../include/AES2.hpp"
using namespace std;
int main() {
	AES aes;
	int mode;
	scanf("%d", &mode);
	string tmp="";
	getline(cin, tmp);
	if(mode==0) {
		string msg;
		getline(cin, msg);
		string ans=aes.encode(msg.c_str());
		cout<<ans;
	} else if(mode==1) {
		string str;
		getline(cin, str);
		string ans=aes.decode(str);
		cout<<ans;
	} else if(mode==2) {
		// 一次性测试加解密
		string msg;
		getline(cin, msg);
		string cipher = aes.encode(msg.c_str());
		cout << cipher << endl;
		string plain = aes.decode(cipher);
		cout << plain;
	} else if(mode==3) {
		// 演示密钥传递：加密端输出密钥+密文
		string msg;
		getline(cin, msg);
		string cipher = aes.encode(msg.c_str());
		string key_str = aes.get_key_str();
		vi key_vec;
		for(unsigned char c : key_str) key_vec.push_back(c);
		string key_b64 = AES::base64_encode(key_vec);
		cout << key_b64 << endl << cipher;
	} else if(mode==4) {
		// 解密端：接收密钥+密文
		string key_b64, cipher;
		getline(cin, key_b64);
		getline(cin, cipher);
		vi key_bytes = AES::base64_decode(key_b64);
		unsigned char key_arr[32];
		for(int i=0;i<32;i++) key_arr[i]=static_cast<unsigned char>(key_bytes[i]);
		AES dec_aes(key_arr);
		cout << dec_aes.decode(cipher);
	}
	return 0;
}

