#ifndef DATABASE
#define DATABASE

#include "table.h"

class Database
{
	public:
		map<string, Table> tables ;
		Database();
		~Database();
		void add_table(string, Table);
		void display_tables();
		void del_table(string);

};
#endif