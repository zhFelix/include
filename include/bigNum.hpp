#include<iostream>
#include<string>
#include<algorithm>
#include<stdexcept>
#include<sstream> 
using namespace std;
class bigNum {
	private:
		string s;
	public:
		friend istream& operator>>(istream& in, bigNum& n) {
			in>>n.s;
			return in;
		}
		friend ostream& operator<<(ostream & out, const bigNum& n) {
			out<<n.s;
			return out;
		}
		int& operator[](const int id) {
			if(id>s.length()) {
				throw out_of_range("It's out of range!");
			}
			static int ans=s[id-1]-'0';
			return ans;
		}
		size_t length() {
			return s.length();
		}
		void push_back(const char c) {
			s.push_back(c);
		}
		string::iterator begin() {
			return s.begin();
		}
		string::iterator end() {
			return s.end();
		}
		bool empty() {
			return s.empty();
		}
		int back() {
			return s[s.length()-1];
		}
		bigNum& operator=(const bigNum& b) {
			if(this!=&b) {
				s=b.s;
			}
			return *this;
		}
		bigNum& operator=(const string& b) {
			s=b;
			return *this;
		}
		template<typename _T>
		bigNum& operator=(const _T& b) {
			stringstream ss;
			ss<<b;
			s=ss.str();
			return *this;
		}
		bool operator>(const bigNum& b) {
			return (s.length()!=b.s.length()?(s.length()>b.s.length()?true:false):false);
		}
		bool operator<(const bigNum& b) {
			return (s.length()!=b.s.length()?(s.length()<b.s.length()?true:false):false);
		}
		bool operator==(const bigNum& b) {
			return s==b.s;
		}
		bool operator!=(const bigNum& b) {
			return s!=b.s;
		}
		bigNum operator+(bigNum b) {
			bigNum c;
			int carry;
			if(s.length()<b.length()) {
				swap(*this, b);
			}
			int lena=s.length(), lenb=b.length();
			reverse(s.begin(), s.end());
			reverse(b.begin(), b.end());
			for(int i = 0;i<lena;i++) {
				int sum = (s[i]-'0')+(i<lenb?b[i+1]:0)+carry;
				c.push_back('0'+(sum%10));
				carry=sum/10;
			}
			if(carry>0) {
				c.push_back('0'+carry);
			}
			reverse(c.begin(), c.end());
			return c;
		}
		bigNum operator+(long long other) {
			bigNum c;
			stringstream ss;
			ss<<other;
			bigNum b;
			b=ss.str(); 
			int carry=0;
			if(s.length()<b.length()) {
				swap(*this, b);
			}
			int lena=s.length(), lenb=b.length();
			reverse(s.begin(), s.end());
			reverse(b.begin(), b.end());
			for(int i = 0;i<lena;i++) {
				int sum = (s[i]-'0')+(i<lenb?b[i+1]:0)+carry;
				c.push_back('0'+(sum%10));
				carry=sum/10;
			}
			if(carry>0) {
				c.push_back('0'+carry);
			}
			reverse(c.begin(), c.end());
			return c;
		}
		template<typename _T>
		bigNum operator+=(_T b) {
			*this=*this+b;
			return *this;
		}
		bigNum operator+=(bigNum b) {
			*this=*this+b;
			return *this;
		}
		bigNum operator++() {
			return *this+1;
		}
		bigNum operator++(int) {
			bigNum pre=*this;
			*this+=1;
			return pre;
		}
		bigNum operator-(bigNum b) {
			bigNum c;
			bool flag=false;
			bigNum num1=*this, num2=b;
			if(num1[1]=='-') {
				flag=true;
				num1.s=num1.s.substr(2);
			}
			if(num2[1]=='-') {
				num2.s=num2.s.substr(2);
			}
			bigNum big, small;
			if(num1>num2) {
				big=num1;
				small=num2;
			} else {
				big=num2;
				small=num1;
			}
			int carry=0;
			reverse(big.begin(), big.end());
			reverse(small.begin(), small.end());
			int lena=big.length(), lenb=small.length(), lenc=c.length();
			for(int i = 1;i<=lena;i++) {
				int digit = (i<lenb?big[i]-small[i]:big[i]);
				if(digit<0&&carry==0) {
					carry=1;
					digit+=10;
				} else if(digit<0) {
					digit+=10;
				}
				c.push_back('0'+digit);
			}
			while(carry>0&&!c.empty()) {
				int lastDigit=c.back();
				if(lastDigit<0) {
					lastDigit+=10;
				}
				c[lenc-1]=lastDigit+'0';
				carry=lastDigit==10?1:0;
			}
			if(flag&&!c.empty()) {
				c.s.insert(0, "-");
			}
			reverse(c.begin(), c.end());
			return c;
		}
};
