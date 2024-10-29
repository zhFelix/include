#include<string>
#include<iostream>
using namespace std;
class qrcode {
	private:
		string s;
		int ssize;
		int size[40]={21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61, 65, 69, 73, 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 129, 133, 137, 141, 145, 149, 153, 157, 161, 165, 169, 173, 177};
		bool qr[180][180];
	public:
		qrcode(string _s) {
			s=_s;
		}
		friend istream& operator>>(istream& in, qrcode& a) {
			in>>a.s;
			return in;
		}
		friend ostream& operator<<(const ostream& out, qrcode& a) {
			for(int i = 1;i<=ssize;i++) {
				for(int j = 1;j<=ssize;j++) {
					out<<a.qr[i][j];
				}
				out<<'\n';
			}
			return out;
		}
		string btd(int )
};
