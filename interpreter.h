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

void display_result(string,vector<vector<string>> result);
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
	vector<string> sel_col (string colname);
	void del_row(int row_num); //have to change to take predicate

	~Table();
};

class Database
{
	public:
		map<string, Table> tables ;
		Database();
		~Database();
		// Database(const Database&);
		// Database& operator=(const Database&);
		void add_table(string, Table);
		void display_tables();
		void del_table(string);

};


class Context
{

	public:

		Database db;
		string table;	//To store context of which table is being accessed
		string column;	//To store context of which column is being accessed
		function<bool(string, string)> pred;//Predicate for where

		Context();
		Context(Database);
		~Context();
		void set_table(string);
		void set_column(string);
		void set_filter(function<bool(string, string)>);
		void clear(); //To clear context state
		Table get_table();
		vector<string> get_column();
		vector<vector<string>> search_on_filter(string, function<bool(string)>);
		void delete_on_filter(function<bool(string, string)>);
};

class Language
{
	public:
		virtual ~Language() = 0;
		virtual void tokenize() =0 ;
		virtual vector<vector<string>> evaluate_query(Context &) = 0;
};

class SQL : public Language
{
	private:
		string query;
	 	vector<string> tokens;
	public:
		SQL(string);
		void tokenize();
		vector<vector<string>> evaluate_query(Context &);
		~SQL();
};

class Expression
{
public:
	virtual vector<vector<string>> interpret(Context &ctx) = 0;
	virtual ~Expression() = 0;
};

class Values: public Expression
{
	private:
		map<string, string> row;
	public:
		Values();
		Values(map<string, string>);
		vector<vector<string>> interpret(Context &ctx);
};

class Insert : public Expression
{
	private:
		string table;
		Values values;
	public:
		Insert();
		Insert(string, Values);
		vector<vector<string>> interpret(Context &ctx);
};


// class Select : public Expression
// {
// 	private:

// };

// class From : public Expression
// {

// };

// class When : public Expression
// {

// };

// class Delete : public Expression
// {

// };
#endif
