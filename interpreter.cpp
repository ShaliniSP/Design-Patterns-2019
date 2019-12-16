#include "interpreter.h"
#include "language.h"
#include "expression.h"
#include "predicates.h"
#include <string>

Context::Context()
{

}
Context::Context( Database database) : db(database)
{

}

void Context::set_table(string table_name)
{

	table = table_name;
}

void Context::set_column(string column_name)
{
	column = column_name;
}

void Context::set_filter(function<bool(string, string)> predicate)
{
	pred = predicate;
}

void Context::clear()
{
	table = "";
	column = "";
	pred = nullptr;
}

Table Context::get_table()
{
	for(auto tab : db.tables)
	{
		if(tab.first == table)
		{
			//cout << tab.second.size;
			return tab.second;
		}
	}
}


vector<string> Context::get_column()
{
	Table current_table = get_table();
	vector<string> current_column;

	for(auto col: current_table.t)
	{
		if(col.first == column)
		{
			current_column = col.second;
			return current_column;
			break;
		}
	}

}


vector<vector<string>> Context::get_column(int i)
{
	Table current_table = get_table();
	vector<vector<string>> current_columns;

	for(auto col: current_table.t)
	{
		current_columns.push_back(col.second);
	}
	return current_columns;
}

vector<vector<string>> Context::search_on_filter(string column_name, function<bool(string)> pred) //Return indices - rows where filter is true. Needs to be passed to sel_row to return the row
{
	Table t = get_table();
	vector<string> filter_column = t.sel_col(column_name);
	vector<int> indices;
	for (auto it = find_if(filter_column.begin(), filter_column.end(), pred); it != filter_column.end(); it = find_if(++it, filter_column.end(), pred))
	{
	    indices.push_back(it - filter_column.begin());
	}
	vector<vector<string>> selected_rows;


	for(auto i : indices)
	{
		if(column=="*")
		{


			selected_rows.push_back(t.sel_row(i));
		}
		else
		{
			vector<string> selected_column = t.sel_col(column);
			vector<string> temp;
			temp.push_back(selected_column[i]);
			selected_rows.push_back(temp);
		}
	}

	return selected_rows;
}

vector<vector<string>> Context::delete_on_filter(string column_name, function<bool(string)> pred)
{
	Table t = get_table();
	string num_of_del_rows;
	if(column_name == " ")
	{
		num_of_del_rows = to_string(t.size);
		for(int i = t.size-1; i >= 0; --i)
			t.del_row(i);
	}

	else
	{
		vector<string> filter_column = t.sel_col(column_name);
		vector<int> indices;
		for (auto it = find_if(filter_column.begin(), filter_column.end(), pred); it != filter_column.end(); it = find_if(++it, filter_column.end(), pred))
		{
		    indices.push_back(it - filter_column.begin());
		}

	  	for (auto rit = indices.rbegin(); rit!= indices.rend(); ++rit)
		{
			cout << *rit;
			t.del_row(*rit);
		}
		num_of_del_rows = to_string(indices.size());
	}

	db.tables[table] = t;
	return {{"Deleted ", num_of_del_rows, " Rows"}};
}

Context::~Context()
{}

