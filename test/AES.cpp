#include<iostream>
#include "../include/AES.hpp"
using namespace std;
int main() {
	AES aes;
	cout<<aes.sub("hello");
	return 0;
}

