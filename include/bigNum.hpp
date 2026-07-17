#include<iostream>
#include<string>
#include<algorithm>
#include<stdexcept>
#include<sstream> 
using namespace std;
class bigNum {
	private:
		string s;
		static string normalizeString(string value) {
			if(value.empty()) {
				return "0";
			}
			bool negative=false;
			if(value[0]=='-') {
				negative=true;
				value.erase(0, 1);
			} else if(value[0]=='+') {
				value.erase(0, 1);
			}
			if(value.empty()) {
				return "0";
			}
			size_t dotPos=value.find('.');
			string intPart, fracPart;
			if(dotPos==string::npos) {
				intPart=value;
			} else {
				intPart=value.substr(0, dotPos);
				fracPart=value.substr(dotPos+1);
			}
			if(intPart.empty()) {
				intPart="0";
			}
			size_t pos=intPart.find_first_not_of('0');
			if(pos==string::npos) {
				intPart="0";
			} else {
				intPart.erase(0, pos);
			}
			while(!fracPart.empty()&&fracPart.back()=='0') {
				fracPart.pop_back();
			}
			value=intPart;
			if(!fracPart.empty()) {
				value.push_back('.');
				value+=fracPart;
			}
			if(value=="0") {
				return "0";
			}
			return negative?"-"+value:value;
		}
		static bool isNegativeString(const string& value) {
			return !value.empty()&&value[0]=='-';
		}
		static string absString(const string& value) {
			return isNegativeString(value)?value.substr(1):value;
		}
		static string digitsOnly(const string& value) {
			string digits;
			for(char c : absString(value)) {
				if(c!='.') {
					digits.push_back(c);
				}
			}
			return digits.empty()?"0":digits;
		}
		static void splitAbsParts(const string& value, string& intPart, string& fracPart) {
			string body=absString(normalizeString(value));
			size_t dotPos=body.find('.');
			if(dotPos==string::npos) {
				intPart=body;
				fracPart.clear();
			} else {
				intPart=body.substr(0, dotPos);
				fracPart=body.substr(dotPos+1);
			}
		}
		static bool isIntegerString(const string& value) {
			return normalizeString(value).find('.')==string::npos;
		}
		static bool isOddDigitString(const string& digits) {
			string value=stripLeadingZerosDigits(digits);
			return (value.back()-'0')%2==1;
		}
		static string div2DigitString(const string& digits) {
			string value=stripLeadingZerosDigits(digits), result;
			int carry=0;
			for(char c : value) {
				int current=carry*10+(c-'0');
				result.push_back('0'+(current/2));
				carry=current%2;
			}
			return stripLeadingZerosDigits(result);
		}
		static bool isDivisibleBySmallInt(const string& digits, int divisor) {
			string value=stripLeadingZerosDigits(digits);
			if(divisor==2) {
				return ((value.back()-'0')%2)==0;
			}
			if(divisor==5) {
				char last=value.back();
				return last=='0'||last=='5';
			}
			return false;
		}
		static string divSmallDigitString(const string& digits, int divisor) {
			string value=stripLeadingZerosDigits(digits), result;
			int carry=0;
			for(char c : value) {
				int current=carry*10+(c-'0');
				result.push_back('0'+(current/divisor));
				carry=current%divisor;
			}
			return stripLeadingZerosDigits(result);
		}
		static string stripLeadingZerosDigits(const string& digits) {
			size_t pos=digits.find_first_not_of('0');
			if(pos==string::npos) {
				return "0";
			}
			return digits.substr(pos);
		}
		static string addDigitStrings(const string& a, const string& b) {
			string result;
			int i=a.length()-1, j=b.length()-1, carry=0;
			while(i>=0||j>=0||carry>0) {
				int sum=carry;
				if(i>=0) {
					sum+=a[i]-'0';
					i--;
				}
				if(j>=0) {
					sum+=b[j]-'0';
					j--;
				}
				result.push_back('0'+(sum%10));
				carry=sum/10;
			}
			reverse(result.begin(), result.end());
			return stripLeadingZerosDigits(result);
		}
		static string subDigitStrings(const string& a, const string& b) {
			string result;
			int i=a.length()-1, j=b.length()-1, borrow=0;
			while(i>=0) {
				int digit=(a[i]-'0')-borrow;
				if(j>=0) {
					digit-=b[j]-'0';
					j--;
				}
				if(digit<0) {
					digit+=10;
					borrow=1;
				} else {
					borrow=0;
				}
				result.push_back('0'+digit);
				i--;
			}
			reverse(result.begin(), result.end());
			return stripLeadingZerosDigits(result);
		}
		static string mulDigitStrings(const string& a, const string& b) {
			string left=stripLeadingZerosDigits(a), right=stripLeadingZerosDigits(b);
			if(left=="0"||right=="0") {
				return "0";
			}
			string result(left.length()+right.length(), '0');
			for(int i=left.length()-1;i>=0;i--) {
				for(int j=right.length()-1;j>=0;j--) {
					int mul=(left[i]-'0')*(right[j]-'0');
					int sum=(result[i+j+1]-'0')+mul;
					result[i+j+1]='0'+(sum%10);
					result[i+j]+=sum/10;
				}
			}
			return stripLeadingZerosDigits(result);
		}
		static int compareDigitStrings(const string& a, const string& b) {
			string left=stripLeadingZerosDigits(a), right=stripLeadingZerosDigits(b);
			if(left.length()!=right.length()) {
				return left.length()>right.length()?1:-1;
			}
			if(left==right) {
				return 0;
			}
			return left>right?1:-1;
		}
		static string divDigitStrings(const string& a, const string& b) {
			string numerator=stripLeadingZerosDigits(a), denominator=stripLeadingZerosDigits(b);
			if(denominator=="0") {
				throw invalid_argument("Division by zero!");
			}
			if(compareDigitStrings(numerator, denominator)<0) {
				return "0";
			}
			string quotient, current;
			for(char digit : numerator) {
				current.push_back(digit);
				current=stripLeadingZerosDigits(current);
				int count=0;
				while(compareDigitStrings(current, denominator)>=0) {
					current=subDigitStrings(current, denominator);
					count++;
				}
				quotient.push_back('0'+count);
			}
			return stripLeadingZerosDigits(quotient);
		}
		static string buildDecimalString(string digits, size_t fracLen) {
			digits=stripLeadingZerosDigits(digits);
			if(fracLen==0) {
				return digits;
			}
			if(digits.length()<=fracLen) {
				digits.insert(0, fracLen-digits.length()+1, '0');
			}
			digits.insert(digits.end()-fracLen, '.');
			return normalizeString(digits);
		}
		static string roundToPrecision(string value, size_t precision) {
			value=normalizeString(value);
			bool negative=isNegativeString(value);
			if(negative) {
				value.erase(0, 1);
			}
			size_t dotPos=value.find('.');
			if(dotPos==string::npos) {
				return negative?"-"+value:value;
			}
			string intPart=value.substr(0, dotPos), fracPart=value.substr(dotPos+1);
			if(fracPart.length()<=precision) {
				return normalizeString(negative?"-"+value:value);
			}
			int roundDigit=fracPart[precision]-'0';
			string digits=intPart+fracPart.substr(0, precision);
			if(roundDigit>=5) {
				digits=addDigitStrings(digits, "1");
			}
			string result=buildDecimalString(digits, precision);
			if(negative&&result!="0") {
				result.insert(0, "-");
			}
			return normalizeString(result);
		}
		static int compareAbsStrings(const string& a, const string& b) {
			string intA, fracA, intB, fracB;
			splitAbsParts(a, intA, fracA);
			splitAbsParts(b, intB, fracB);
			if(intA.length()!=intB.length()) {
				return intA.length()>intB.length()?1:-1;
			}
			if(intA!=intB) {
				return intA>intB?1:-1;
			}
			size_t fracLen=max(fracA.length(), fracB.length());
			fracA.append(fracLen-fracA.length(), '0');
			fracB.append(fracLen-fracB.length(), '0');
			if(fracA==fracB) {
				return 0;
			}
			return fracA>fracB?1:-1;
		}
		static string addAbsStrings(const string& a, const string& b) {
			string intA, fracA, intB, fracB;
			splitAbsParts(a, intA, fracA);
			splitAbsParts(b, intB, fracB);
			size_t fracLen=max(fracA.length(), fracB.length());
			fracA.append(fracLen-fracA.length(), '0');
			fracB.append(fracLen-fracB.length(), '0');
			return buildDecimalString(addDigitStrings(intA+fracA, intB+fracB), fracLen);
		}
		static string subAbsStrings(const string& a, const string& b) {
			string intA, fracA, intB, fracB;
			splitAbsParts(a, intA, fracA);
			splitAbsParts(b, intB, fracB);
			size_t fracLen=max(fracA.length(), fracB.length());
			fracA.append(fracLen-fracA.length(), '0');
			fracB.append(fracLen-fracB.length(), '0');
			return buildDecimalString(subDigitStrings(intA+fracA, intB+fracB), fracLen);
		}
		static int compareSignedStrings(const string& a, const string& b) {
			string left=normalizeString(a), right=normalizeString(b);
			bool leftNegative=isNegativeString(left), rightNegative=isNegativeString(right);
			if(leftNegative!=rightNegative) {
				return leftNegative?-1:1;
			}
			int absCompare=compareAbsStrings(absString(left), absString(right));
			if(!leftNegative) {
				return absCompare;
			}
			return -absCompare;
		}
		static bigNum powIntegerInternal(bigNum base, string exponentDigits) {
			bigNum result;
			result="1";
			exponentDigits=stripLeadingZerosDigits(exponentDigits);
			while(exponentDigits!="0") {
				if(isOddDigitString(exponentDigits)) {
					result=result*base;
				}
				base=base*base;
				exponentDigits=div2DigitString(exponentDigits);
			}
			return result;
		}
		void normalize() {
			s=normalizeString(s);
		}
	public:
		bigNum(): s("0") {}
		bigNum(const string& value): s(normalizeString(value)) {}
		bigNum(const char* value): s(normalizeString(string(value))) {}
		template<typename _T>
		bigNum(const _T& value) {
			stringstream ss;
			ss<<value;
			s=normalizeString(ss.str());
		}
		friend istream& operator>>(istream& in, bigNum& n) {
			in>>n.s;
			n.normalize();
			return in;
		}
		friend ostream& operator<<(ostream & out, const bigNum& n) {
			out<<n.s;
			return out;
		}
		int operator[](const int id) const {
			string value=digitsOnly(s);
			if(id<=0||id>value.length()) {
				throw out_of_range("It's out of range!");
			}
			return value[id-1]-'0';
		}
		size_t length() {
			return digitsOnly(s).length();
		}
		string::iterator begin() {
			return s.begin()+(isNegativeString(s)?1:0);
		}
		string::iterator end() {
			return s.end();
		}
		bool empty() {
			return s.empty();
		}
		int back() {
			return digitsOnly(s).back()-'0';
		}
		bigNum& operator=(const bigNum& b) {
			if(this!=&b) {
				s=b.s;
				normalize();
			}
			return *this;
		}
		bigNum& operator=(const string& b) {
			s=b;
			normalize();
			return *this;
		}
		template<typename _T>
		bigNum& operator=(const _T& b) {
			stringstream ss;
			ss<<b;
			s=ss.str();
			normalize();
			return *this;
		}
		bool operator>(const bigNum& b) {
			return compareSignedStrings(s, b.s)>0;
		}
		bool operator<(const bigNum& b) {
			return compareSignedStrings(s, b.s)<0;
		}
		bool operator==(const bigNum& b) {
			return compareSignedStrings(s, b.s)==0;
		}
		bool operator!=(const bigNum& b) {
			return compareSignedStrings(s, b.s)!=0;
		}
		bigNum operator+(bigNum b) {
			bigNum c;
			string a=normalizeString(s), num=normalizeString(b.s);
			bool leftNegative=isNegativeString(a), rightNegative=isNegativeString(num);
			string leftAbs=absString(a), rightAbs=absString(num);
			if(leftNegative==rightNegative) {
				c.s=addAbsStrings(leftAbs, rightAbs);
				if(leftNegative&&c.s!="0") {
					c.s.insert(0, "-");
				}
				return c;
			}
			int absCompare=compareAbsStrings(leftAbs, rightAbs);
			if(absCompare==0) {
				c.s="0";
				return c;
			}
			if(absCompare>0) {
				c.s=subAbsStrings(leftAbs, rightAbs);
				if(leftNegative&&c.s!="0") {
					c.s.insert(0, "-");
				}
			} else {
				c.s=subAbsStrings(rightAbs, leftAbs);
				if(rightNegative&&c.s!="0") {
					c.s.insert(0, "-");
				}
			}
			return c;
		}
		template<typename _T>
		bigNum operator+(_T b) {
			bigNum num;
			num=b;
			return *this+num;
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
			*this+=1;
			return *this;
		}
		bigNum operator++(int) {
			bigNum pre=*this;
			*this+=1;
			return pre;
		}
		bigNum operator-(bigNum b) {
			b.s=normalizeString(b.s);
			if(b.s!="0") {
				if(isNegativeString(b.s)) {
					b.s.erase(0, 1);
				} else {
					b.s.insert(0, "-");
				}
			}
			return *this+b;
		}
		bigNum operator-(long long other) {
			bigNum b;
			b=other;
			return *this-b;
		}
		template<typename _T>
		bigNum operator-(_T b) {
			bigNum num;
			num=b;
			return *this-num;
		}
		template<typename _T>
		bigNum operator-=(_T b) {
			*this=*this-b;
			return *this;
		}
		bigNum operator-=(bigNum b) {
			*this=*this-b;
			return *this;
		}
		bigNum operator*(bigNum b) {
			bigNum c;
			string a=normalizeString(s), num=normalizeString(b.s);
			bool leftNegative=isNegativeString(a), rightNegative=isNegativeString(num);
			string intA, fracA, intB, fracB;
			splitAbsParts(a, intA, fracA);
			splitAbsParts(num, intB, fracB);
			size_t fracLen=fracA.length()+fracB.length();
			c.s=buildDecimalString(mulDigitStrings(intA+fracA, intB+fracB), fracLen);
			if(leftNegative!=rightNegative&&c.s!="0") {
				c.s.insert(0, "-");
			}
			return c;
		}
		template<typename _T>
		bigNum operator*(_T b) {
			bigNum num;
			num=b;
			return *this*num;
		}
		template<typename _T>
		bigNum operator*=(_T b) {
			*this=*this*b;
			return *this;
		}
		bigNum operator*=(bigNum b) {
			*this=*this*b;
			return *this;
		}
		bigNum operator/(bigNum b) {
			const int precision=500;
			bigNum c;
			string a=normalizeString(s), num=normalizeString(b.s);
			if(num=="0") {
				throw invalid_argument("Division by zero!");
			}
			if(a=="0") {
				c.s="0";
				return c;
			}
			bool leftNegative=isNegativeString(a), rightNegative=isNegativeString(num);
			string intA, fracA, intB, fracB;
			splitAbsParts(a, intA, fracA);
			splitAbsParts(num, intB, fracB);
			string numeratorDigits=stripLeadingZerosDigits(intA+fracA);
			string denominatorDigits=stripLeadingZerosDigits(intB+fracB);
			int scaleShift=static_cast<int>(fracB.length())-static_cast<int>(fracA.length());
			string scaledNumerator=numeratorDigits;
			string scaledDenominator=denominatorDigits;
			if(scaleShift>=0) {
				scaledNumerator.append(scaleShift+precision+1, '0');
			} else {
				scaledNumerator.append(precision+1, '0');
				scaledDenominator.append(-scaleShift, '0');
			}
			string scaledQuotient=divDigitStrings(scaledNumerator, scaledDenominator);
			int roundDigit=scaledQuotient.back()-'0';
			string resultDigits=scaledQuotient.length()==1?"0":scaledQuotient.substr(0, scaledQuotient.length()-1);
			if(roundDigit>=5) {
				resultDigits=addDigitStrings(resultDigits, "1");
			}
			c.s=buildDecimalString(resultDigits, precision);
			if(leftNegative!=rightNegative&&c.s!="0") {
				c.s.insert(0, "-");
			}
			return c;
		}
		template<typename _T>
		bigNum operator/(_T b) {
			bigNum num;
			num=b;
			return *this/num;
		}
		template<typename _T>
		bigNum operator/=(_T b) {
			*this=*this/b;
			return *this;
		}
		bigNum operator/=(bigNum b) {
			*this=*this/b;
			return *this;
		}
		bigNum root(bigNum degree) {
			const int precision=500;
			string value=normalizeString(s), degreeStr=normalizeString(degree.s);
			if(!isIntegerString(degreeStr)||isNegativeString(degreeStr)||degreeStr=="0") {
				throw invalid_argument("Root degree must be a positive integer!");
			}
			if(isNegativeString(value)) {
				throw invalid_argument("Root only supports non-negative numbers!");
			}
			if(value=="0"||value=="1"||degreeStr=="1") {
				bigNum ans;
				ans=value;
				return ans;
			}
			bigNum one, epsilon, current, x, degreeNum, degreeMinusOneNum;
			one="1";
			epsilon.s="0.";
			epsilon.s.append(precision-1, '0');
			epsilon.s.push_back('1');
			current=value;
			degreeNum=degreeStr;
			degreeMinusOneNum=degreeNum-1;
			if(current<one) {
				x=one;
			} else {
				x=current;
			}
			for(int iter=0;iter<128;iter++) {
				bigNum prev=x;
				bigNum powered=powIntegerInternal(x, subDigitStrings(absString(degreeStr), "1"));
				bigNum next=(degreeMinusOneNum*x+current/powered)/degreeNum;
				bigNum diff=prev>next?prev-next:next-prev;
				x=next;
				if(diff<epsilon||diff==epsilon) {
					break;
				}
			}
			x.s=roundToPrecision(x.s, precision);
			return x;
		}
		template<typename _T>
		bigNum root(_T degree) {
			bigNum num;
			num=degree;
			return root(num);
		}
		bigNum sqrt() {
			bigNum two;
			two=2;
			return root(two);
		}
		bigNum pow(bigNum exponent) {
			bigNum one, baseValue, result;
			one="1";
			string value=normalizeString(s), expStr=normalizeString(exponent.s);
			if(expStr=="0") {
				return one;
			}
			if(value=="0"&&isNegativeString(expStr)) {
				throw invalid_argument("Zero cannot be raised to a negative exponent!");
			}
			bool negativeExp=isNegativeString(expStr);
			if(negativeExp) {
				expStr=absString(expStr);
			}
			baseValue=*this;
			if(isNegativeString(value)&&!isIntegerString(expStr)) {
				throw invalid_argument("Fractional exponent only supports non-negative base!");
			}
			if(isIntegerString(expStr)) {
				result=powIntegerInternal(baseValue, absString(expStr));
			} else {
				string intPart, fracPart;
				splitAbsParts(expStr, intPart, fracPart);
				string numeratorDigits=stripLeadingZerosDigits(intPart+fracPart);
				string denominatorDigits="1";
				denominatorDigits.append(fracPart.length(), '0');
				while(denominatorDigits!="1") {
					if(isDivisibleBySmallInt(numeratorDigits, 2)&&isDivisibleBySmallInt(denominatorDigits, 2)) {
						numeratorDigits=divSmallDigitString(numeratorDigits, 2);
						denominatorDigits=divSmallDigitString(denominatorDigits, 2);
					} else if(isDivisibleBySmallInt(numeratorDigits, 5)&&isDivisibleBySmallInt(denominatorDigits, 5)) {
						numeratorDigits=divSmallDigitString(numeratorDigits, 5);
						denominatorDigits=divSmallDigitString(denominatorDigits, 5);
					} else {
						break;
					}
				}
				bigNum rooted=root(denominatorDigits);
				result=powIntegerInternal(rooted, numeratorDigits);
				result.s=roundToPrecision(result.s, 500);
			}
			if(negativeExp) {
				return one/result;
			}
			return result;
		}
		template<typename _T>
		bigNum pow(_T exponent) {
			bigNum num;
			num=exponent;
			return pow(num);
		}
		bigNum operator%(bigNum b) {
			bigNum c;
			string a=normalizeString(s), num=normalizeString(b.s);
			if(!isIntegerString(a)||!isIntegerString(num)) {
				throw invalid_argument("Modulo only supports integers!");
			}
			if(num=="0") {
				throw invalid_argument("Division by zero!");
			}
			if(a=="0") {
				c.s="0";
				return c;
			}
			bool leftNegative=isNegativeString(a), rightNegative=isNegativeString(num);
			string leftAbs=absString(a), rightAbs=absString(num);
			string quotient=divDigitStrings(leftAbs, rightAbs);
			string remainder=subDigitStrings(leftAbs, mulDigitStrings(quotient, rightAbs));
			c.s=remainder;
			if(leftNegative&&c.s!="0") {
				c.s.insert(0, "-");
			}
			return c;
		}
		template<typename _T>
		bigNum operator%(_T b) {
			bigNum num;
			num=b;
			return *this%num;
		}
		template<typename _T>
		bigNum operator%=(_T b) {
			*this=*this%b;
			return *this;
		}
		bigNum operator%=(bigNum b) {
			*this=*this%b;
			return *this;
		}
		bigNum operator--() {
			bigNum one;
			one=1;
			*this=*this-one;
			return *this;
		}
		bigNum operator--(int) {
			bigNum pre=*this;
			bigNum one;
			one=1;
			*this=*this-one;
			return pre;
		}
};
