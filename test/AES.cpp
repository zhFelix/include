#include<bits/stdc++.h>
#include<AES.hpp>
using namespace std;
int main() {
	AES aes;
	cout<<aes.desub(aes.sub("hello"));
	return 0;
}

