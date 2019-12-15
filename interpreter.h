/*Context contains the tables. Tables is a map of table name to table. Table is a map of column names to a vector of strings.*/
#ifndef INTERPRETER
#define INTERPRETER

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
	map<string, vector<string>> t;
	Table();
	Table(string,vector<string>);
	Table(const Table&);
	Table& operator=(const Table&);
	void add_row(map<string, string>);
	void display();
	vector<string> sel_row(int row_num);
	void del_row(int row_num); //have to change to take predicate

	~Table();
};

class Context
{
	private:
		map<string, Table> tables ;
		string table;	//To store context of which table is being accessed
		string column;	//To store context of which column is being accessed
		function<bool(string, string)> pred;//Predicate for where

	public:
		Context();
		~Context();
		void set_table(string,Table);
		void set_column(string);
		void set_filter(function<bool(string, string)>);
		void clear(); //To clear context state
		Table get_table();
		vector<string> get_column();
		vector<vector<string>> search_on_filter(function<bool(string)>);
		void delete_on_filter(function<bool(string, string)>);
};

class Expression
{
	virtual vector<string> interpret(Context ctx) = 0;
};

class Insert : public Expression
{

};

class Select : public Expression
{
	private:


};

class From : public Expression
{

};

class When : public Expression
{

};

class Delete : public Expression
{

};
#endif
