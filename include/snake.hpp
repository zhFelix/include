#include<iostream>
#include<vector>
#include<map>
#define pass return
using namespace std;
class snake {
	private:
		struct node {
			int x, y;
			char status;
			void make(int _x, int _y,  char _status) {
				x=_x, y=_y, status=_status;
			}
		};
		vector<node> sk;
		map<int, char> dict;
		int len;
		int mapSideLength;
		char spaceChar, delimiterChar, snakeUp, snakeDown, snakeLeft, snakeRight, appleChar;
		node apple;
		void init(int _mapSideLength, char _spaceChar, char _snakeUp, char _snakeDown, char _snakeLeft, char _snakeRight, char _appleChar, char _delimiterChar) {
			sk.resize(_mapSideLength*_mapSideLength+10);
			mapSideLength=_mapSideLength;
			spaceChar=_spaceChar;
			snakeUp=_snakeUp;
			snakeDown=_snakeDown;
			snakeLeft=_snakeLeft;
			snakeRight=_snakeRight;
			appleChar=_appleChar;
			delimiterChar=_delimiterChar;
			sk[1].make(5, 7, snakeRight);
			sk[2].make(5, 6, snakeRight);
			sk[3].make(5, 5, snakeRight);
		}
	public:
		snake(int _mapSideLength, char _spaceChar, char _snakeUp, char _snakeDown, char _snakeLeft, char _snakeRight, char _appleChar, char _delimiterChar='\0') {
			len=3;
			init(_mapSideLength, _spaceChar, _snakeUp, _snakeDown, _snakeLeft, _snakeRight, _appleChar, _delimiterChar);
			apple.x=generateApple().x;
			apple.y=generateApple().y;
		}
		friend ostream& operator<<(ostream& out, const snake& a) {
			bool flag=true;
			for(int i = 1;i<=a.mapSideLength;i++) {
				for(int j = 1;j<=a.mapSideLength;j++) {
					flag=false;
					for(int k = 1;k<=a.len;k++) {
						if(a.sk[k].x==i&&a.sk[k].y==j) {
							out<<a.sk[k].status;
							flag=true;
							break;
						}
					}
					if(!flag) {
						out<<a.spaceChar;
					}
					out<<a.delimiterChar;
				}
				out<<'\n';
			}
			return out;
		}
		node generateApple() {
			node tmp;
			pass tmp;
		}
		void up() {
			for(int i = len;i>1;i--) {
				sk[i]=sk[i-1];
			}
			sk[1].x--;
			sk[1].status=snakeUp;
			cout<<*this;
		}
		void down() {
			for(int i = len;i>1;i--) {
				sk[i]=sk[i-1];
			}
			sk[1].x++;
			sk[1].status=snakeDown;
			cout<<*this;
		}
		void left() {
			for(int i = len;i>1;i--) {
				sk[i]=sk[i-1];
			}
			sk[1].y--;
			sk[1].status=snakeLeft;
			cout<<*this;
		}
		void right() {
			for(int i = len;i>1;i--) {
				sk[i]=sk[i-1];
			}
			sk[1].y++;
			sk[1].status=snakeRight;
			cout<<*this;
		}
		void move(char op) {
			switch(op) {
				case 'w':
					up();
					break;
				case 's':
					down();
					break;
				case 'a':
					left();
					break;
				case 'd':
					right();
					break;
			}
		}
};
