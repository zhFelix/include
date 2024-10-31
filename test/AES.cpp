#include<bits/stdc++.h>
#include<AES.hpp>
using namespace std;
int main() {
	AES aes;
	vector<int> tmp=aes.code("Hello World!");
	auto ans=aes.desub(tmp);
	for(auto i:tmp) {
		cout<<i<<" ";
	}
	cout<<ans;
	return 0;
}

