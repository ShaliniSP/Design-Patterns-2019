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
	void del_row(int row_num); //have to change to take predicate
	void interpret(char []);
	void selectfrom(string colname);
	void deletewhere(string colname,string val);

	~table();
};

class Context
{

		public:
			Context();
			~Context();

};
#endif
