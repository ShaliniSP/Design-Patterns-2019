#ifndef TABLE
#define TABLE
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <algorithm>
#include <cstring>

using namespace std;

class Table
{
private:
	string name;
public:
	int size;
	map<string, vector<string>> t;
	Table();
	Table(string,vector<string>);
	Table(const Table&);
	Table& operator=(const Table&);
	void add_row(map<string, string>);
	void display();
	vector<string> sel_row(int row_num);
	vector<string> sel_col (string colname);
	void del_row(int row_num); 

	~Table();
};
#endif