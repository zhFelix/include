#include<bits/stdc++.h>
#include<AES.hpp>
using namespace std;
int main() {
	AES aes;
	int mode;
	scanf("%d", &mode);
	getchar();
	if(mode==0) {
		string msg;
		getline(cin, msg);
		vector<int> ans=aes.encode(msg.c_str());
		for(int i = 0;i<ans.size();i++) {
			cout<<ans[i]<<" ";
		}
	} else {
		vector<int> str;
		int x;
		while(cin>>x) {
			str.push_back(x);
		}
		string ans=aes.decode(str);
		cout<<ans;
	}
	return 0;
}

