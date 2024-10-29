#include<iostream>
#include<vector>
#include<queue>
#include<functional>
using namespace std;
template<class _T, class Container=vector<_T>, class Compare=less<_T> >
class heat {
	public:
		heat() = default;
		template<class inputIterator>
		heat(inputIterator first, inputIterator last) {
			while(first!=last) {
				_c.push_back(*first);
				first++;
			}
			for(int i = ((_c.size()-2)/2);i>0;i--) {
				adjust_down(i);
			}
		}
		void adjust_up(int child);
		void adjust_down(int parent);
		bool empty() const;
		size_t size() const;
		const _T& top() const;
		void push(const _T& x);
		void pop();
	private:
		Container _c;
		Compare cmp;
};
