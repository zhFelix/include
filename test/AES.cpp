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
		// 一次性测试加解密（同密钥）
		string msg;
		getline(cin, msg);
		string cipher = aes.encode(msg.c_str());
		cout << cipher << endl;
		string plain = aes.decode(cipher);
		cout << plain;
	} else if(mode==3) {
		// 演示密钥传递：加密端输出密钥(hex)+密文
		string msg;
		getline(cin, msg);
		string cipher = aes.encode(msg.c_str());
		string key_str = aes.get_key_str();
		// 转为十六进制字符串（64字符）
		string hex_key;
		for(unsigned char c : key_str) {
			char buf[3];
			sprintf(buf, "%02x", c);
			hex_key += buf;
		}
		cout << hex_key << endl << cipher;
	} else if(mode==4) {
		// 解密端：接收密钥(hex)+密文
		string hex_key, cipher;
		getline(cin, hex_key);
		getline(cin, cipher);
		unsigned char key_arr[32];
		for(int i=0;i<32;i++) {
			auto h2i = [](char c) -> int {
				if(c>='0'&&c<='9') return c-'0';
				if(c>='a'&&c<='f') return c-'a'+10;
				if(c>='A'&&c<='F') return c-'A'+10;
	return 0;
			};
			key_arr[i] = (h2i(hex_key[i*2])<<4) | h2i(hex_key[i*2+1]);
}
		AES dec_aes(key_arr);
		cout << dec_aes.decode(cipher);
	}
	return 0;
}

