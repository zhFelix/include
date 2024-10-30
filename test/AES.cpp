#include<bits/stdc++.h>
#include<AES.hpp>
using namespace std;
int main() {
	AES aes;
	char* n="hello";
	int len=strlen(n);
	const char* zero=char(128-len%128);
	for(int i = 1;i<=zero;i++) {
		strcat(n, zero);
	}
	vector<int> tmp=aes.sub(n);
	auto ans=aes.desub(tmp);
	cout<<ans;
	return 0;
}

