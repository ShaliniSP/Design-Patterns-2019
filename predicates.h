#ifndef PREDICATES
#define PREDICATES
#include<string>
using namespace std;

class isEqual
{
	private:
		string filter_string;
	public:
		isEqual(string s);// : filter_string(s){}
		bool operator()(string check);
		// {
		// 	return filter_string==check;
		// }
};
class isNotEqual
{
	private:
		string filter_string;
	public:
		isNotEqual(string s);// : filter_string(s){}
		bool operator()(string check);
		// {
		// 	return filter_string!=check;
		// }
};
class startsWith
{
	private:
		string filter_string;
	public:
		startsWith(string s);// : filter_string(s){}
		bool operator()(string check);
		// {
		// 	return check.find(filter_string)==0;
		// }
};
#endif