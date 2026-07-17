#include<iostream>
#include "../include/bigNum.hpp"
using namespace std;
int main() {
	bigNum a, b;
	char op;
	cin>>a>>op>>b;
	try {
		switch(op) {
			case '+':
				cout<<a+b;
				break;
			case '-':
				cout<<a-b;
				break;
			case '*':
				cout<<a*b;
				break;
			case '/':
				cout<<a/b;
				break;
			case '%':
				cout<<a%b;
				break;
			default:
				cout<<"Invalid operator";
				break;
		}
	} catch(const exception& ex) {
		cout<<ex.what();
	}
	return 0;
}
