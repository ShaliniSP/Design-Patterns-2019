#include "database.h"

Database::Database()
{}

void Database::add_table(string table_name, Table table)
{
	auto it= tables.find(table_name);
	if(it==tables.end())
	{
		//table is added for the first time
		tables.insert({table_name, table});
	}
	else
	{
		(it->second) = table;
		display_tables();
		//cout<<"\nTable changed: ";
		//(it->second).display();

	}
}

void Database::del_table(string table_name)
{
	tables.erase(table_name);
	cout<<"Table "<< table_name<< " delted.\n";

}

void Database::display_tables()
{
	auto it= tables.begin();
	cout<<"\nThe Tables in the database are:\n "<<endl;
	for(; it!=tables.end(); ++it)
	{

		cout<<"Table Name: " << it->first;
		(it->second).display();
	}
}

Database::~Database()
{}
