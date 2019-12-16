#ifndef INTERPRETER
#define INTERPRETER

#include "database.h"

using namespace std;

void display_result(string,vector<vector<string>> result);

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
		vector<vector<string>> get_column(int);
		vector<vector<string>> search_on_filter(string, function<bool(string)>);
		vector<vector<string>> delete_on_filter(string, function<bool(string)>);
};

#endif
