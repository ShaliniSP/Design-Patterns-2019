/*Context contains the tables. Tables is a map of table name to table. Table is a map of column names to a vector of strings.*/
#ifndef INTERPRETER
#define INTERPRETER

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstring>
using namespace std;

class table
{
private:
	map<string, vector<string>> t;


public:
	table(vector<string>);
	//table(const table&) = delete;
	//table& operator=(const table&) = delete;
	void add_row(map<string, string>);
	void display();
	void sel_row(string column_name, /*function pointer with bool*/); 
	void del_row(int row_num); //have to change to take predicate
	
	~table();
};

class Context
{
	private:
		map<string, vector<table>> tables ;
		table *t;	//To store context of which table is being accessed
		string column;	//To store context of which column is being accessed
		//Predicate for where

	public:
		Context();
		~Context();
		//setTable()
		//setColumn

};

class Expression
{
	virtual vector<string> interpret(Context ctx) = 0;
}

class Insert : public Expression
{

}

class Select : public Expression
{

}

class From : public Expression
{

}

class When : public Expression
{

}

class Delete : public Expression
{

}
#endif
