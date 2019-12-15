#include "interpreter.h"
#include <string>

Table::Table()
{

}

Table::Table(string table_name, vector<string> column_names)
{
	name=table_name;
	for(auto name : column_names)
	{
		t.insert(pair<string,vector<string> >(name, vector<string>()));
	}
}

void Table::add_row(map<string, string> row)
{
	for(auto r: row)
	{
		t[r.first].push_back(r.second);
	}
	// Context c= Context();
	// string table_name = name;
	// //Table table = t;
	// c.set_table(table_name, t);
}

void Table::display()//change to make it display like a table
{
	cout<<endl;
	for(auto col : t)
	{
		cout << col.first << " : ";
		for(auto row : col.second)
			cout << row << " ";
		cout << '\n';
	}
	cout<< '\n';
}

void Table::del_row(int row_num)//delete does not work. have to use references??
{

	for(auto col: t)
	{
		vector<string>::iterator ite = col.second.begin()+row_num;

		//auto rem = v.begin() + row_num;
		//cout<<*rem<<"\n";
    //t[col.first][row_num]=" ";
		// for (auto it = col.second.begin(); it != col.second.end(); ++it)
		// 	cout << ' ' << *it;
		// cout<<"\n_____\n";
		col.second.erase(col.second.begin()+row_num);
		// for (auto it = col.second.begin(); it != col.second.end(); ++it)
		// 	cout << ' ' << *it;
		//cout<<"\n";


		t[col.first] = col.second;

		//cout<<t[col.first][0]<<"_";

	}
}

vector<string> Table::sel_row(int row_num)//returns row as vector of strings
{
	vector<string> row;
	for(auto col: t)
	{
		vector<string>::iterator ite = col.second.begin()+row_num;
		row.push_back(*ite);
	}
	return row;
}

Table::Table(const Table& rhs)
{
	t = rhs.t;
}

Table& Table::operator=(const Table& rhs)
{
	if(this != &rhs)
	{
		t = rhs.t;
	}
	return *this;
}

Table::~Table()
{

}

Context::Context()
{

}

void Context::set_table(string table_name, Table table)
{
	auto it= tables.find(table_name);
	if(it==tables.end())
	{
		//table is added for the first time
		tables.insert({table_name, table});
		for(auto t : tables)
		{
			cout<<"The table inserted : "<<t.first;
			t.second.display();
		}
	}
	else
	{
		(it->second) = table;
		cout<<"\nTable changed: ";
		(it->second).display();

	}
	// for(auto t : tables)
	// {
	// 	if(t.first == table_name)
	// 	{
	// 		table = tables[t.first];
	// 		break;
	// 	}
	// }
}

void Context::set_column(string column_name)
{
	column = column_name;
	// for(auto col: table.t)
	// {
	// 	if(strcmp(col.first, column_name)==0)
	// 	{
	// 		column = col.second;
	// 		break;
	// 	}
	// }
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
	Table current_table;
	for(auto tab : tables)
	{
		if(tab.first == table)
		{
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
			break;
		}
	}

}


vector<vector<string>> Context::search_on_filter(function<bool(string)> pred) //Return indices - rows where filter is true. Needs to be passed to sel_row to return the row
{
	vector<string> column = get_column();
	vector<int> indices;
	for (auto it = find_if(column.begin(), column.end(), pred); it != column.end(); it = find_if(++it, column.end(), pred))
	{
	    indices.push_back(it - column.begin());
	}
	Table table = get_table();
	vector<vector<string>> selected_rows;
	for(auto i : indices)
		selected_rows.push_back(table.sel_row(i));
	return selected_rows;
}

// void delete_on_filter(function<bool(string, string)>);

Context::~Context()
{

}
// void table::interpret(char query[])
// {

// 	cout<<query<<endl;
// 	vector<string> tokens;
// 	char *word = strtok(query, " ");

// 	while (word != NULL)
// 	{
// 		tokens.push_back(word);
// 		word = strtok(NULL, " ");
// 	}
// 	string first = *tokens.begin();

// 	if(strcmp("SELECT",first.c_str())==0)
// 	{
// 	  selectfrom(tokens[1]);
// 	}
// 	if(strcmp("DELETE",first.c_str())==0)
// 	{
// 	  deletewhere(tokens[4],tokens[6]);
// 	}

// }

// void table::selectfrom (string colname)
// {
// 	cout<<colname<<" : ";
// 	for(auto col : t)
// 	{
// 		if( strcmp(col.first.c_str(),colname.c_str())==0)
// 		{
// 			for(auto row : col.second)
// 				cout << row << " ";
// 			cout << "\n\n";
// 		}
// 	}
// }

// void table::deletewhere(string colname,string val)
// {
// 		for(auto col: t)
// 		{
// 				if( strcmp(col.first.c_str(),colname.c_str())==0)
// 				{
// 					int x= 0;
// 					for(	vector<string>::iterator ite = col.second.begin(); ite!=col.second.end();++ite,++x)
// 					{
// 					//	cout<<(*ite).c_str();
// 						if(strcmp((*ite).c_str(),val.c_str())==0)
// 						{
// 								del_row(x);
// 						}
// 					}
// 				}
// 		}
// }
