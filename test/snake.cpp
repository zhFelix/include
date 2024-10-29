#include<bits/stdc++.h>
#include<snake.hpp>
using namespace std;
int main() {
	snake s(10, 'K', 'W', 'S', 'A', 'D', 'A');
	cout<<s;
	char op;
	for(int i = 1;i<=10;i++) {
		cin>>op;
		s.move(op);
	}
	return 0;
}

