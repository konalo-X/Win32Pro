#include<iostream>
#include<algorithm>
#include<list>
#include<vector>
#include<list>
#include<functional>
bool func(int n)
	{
	return n > 5;
	}
int main()
{
	using namespace std;
	list<int> li = { 1,11,34,4,5,6,7,7,8,9 };
	for (auto x : li)
		cout << x << "  ";
	cout<< endl;;
	list<int>::iterator it;
	it=remove_if(li.begin(),li.end(), func);
	li.erase(it, li.end());
	
	
	for (auto x : li)
		cout << x << "  ";
	system("pause");
	return 0;
}