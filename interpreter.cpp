#include "interpreter.h"

table::table(vector<string> column_names)
{
	for(auto name : column_names)
	{
		t.insert(pair<string,vector<string> >(name, vector<string>()));
	}
}

void table::add_row(map<string, string> row)
{
	for(auto r: row)
	{
		t[r.first].push_back(r.second);
	}
}

void table::display()//change to make it display like a table
{
	for(auto col : t)
	{
		cout << col.first << " : ";
		for(auto row : col.second)
			cout << row << " ";
		cout << '\n';
	}
}

void table::del_row(int row_num)//delete does not work. have to use references??
{
	for(auto col: t)
	{
		vector<string> v = col.second;
		v.erase(v.begin() + row_num);
		col.second = v;
	}
}

table::~table()
{

}
