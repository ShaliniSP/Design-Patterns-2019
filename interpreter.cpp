#include "interpreter.h"
#include <string>

Table::Table()
{}

Table::Table(string table_name, vector<string> column_names)
{
	name=table_name;
	for(auto name : column_names)
	{
		t.insert(pair<string,vector<string> >(name, vector<string>()));
	}
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

		col.second.erase(col.second.begin()+row_num);

		t[col.first] = col.second;
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

vector<string> Table::sel_col (string colname)
{
	vector<string> column;
	cout<<colname<<" : ";
	for(auto col : t)
	{
		if( col.first == colname)
		{
			return col.second;
		}
	}
}

Table::~Table()
{}

Context::Context()
{

}
Context::Context( Database database) : db(database)
{

}

void Context::set_table(string table_name)
{
	// auto it= tables.find(table_name);
	// if(it==tables.end())
	// {
	// 	//table is added for the first time
	// 	tables.insert({table_name, table});
	// 	for(auto t : tables)
	// 	{
	// 		cout<<"The table inserted : "<<t.first;
	// 		t.second.display();
	// 	}
	// }
	// else
	// {
	// 	(it->second) = table;
	// 	cout<<"\nTable changed: ";
	// 	(it->second).display();

	// }
	table = table_name;
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
	for(auto tab : db.tables)
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

vector<vector<string>> Context::get_column(int i)
{
	Table current_table = get_table();
	vector<vector<string>> columns;

	for(auto col: current_table.t)
	{
		columns.push_back(col.second);
	}
	return columns;
}

vector<vector<string>> Context::search_on_filter(string column_name, function<bool(string)> pred) //Return indices - rows where filter is true. Needs to be passed to sel_row to return the row
{
	Table t = get_table();
	vector<string> column = t.sel_col(column_name);
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
{}


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

Expression::~Expression()
{}

Database::~Database()
{}

Insert::Insert()
{}

Insert::Insert(string table_name, Values v): table(table_name), values(v)
{}

vector<vector<string>> Insert::interpret(Context &ctx)
{
	ctx.set_table(table);
	return values.interpret(ctx);
}


Values::Values()
{}

Values::Values(map<string, string> row_to_be_inserted): row(row_to_be_inserted)
{}

vector<vector<string>> Values::interpret(Context &ctx)
{
	Table t = ctx.get_table();
	t.add_row(row);
	//t.display();
	ctx.db.tables[ctx.table] = t;
	//ctx.db.add_table(ctx.table, t);
	return {{"Row added successfully"}};
}

From::From()
{}

From::From(string table_name, Select s): table(table_name), select(s)
{}

vector<vector<string>> From::interpret(Context ctx)
{
	ctx.set_table(table);
	return select.interpret(ctx);
}

Select::Select()
{}

Select::Select(string column_name):column(column)
{}

// Select::Select(string column_name, Where w): column(column_name), where(w)
// {}

vector<vector<string>> Select::interpret(Context ctx)
{
	ctx.set_column(column);
	vector<vector<string>> result;
	// if(where)
	// {
	// 	return where.interpret(ctx);
	// }
	//else
	{
		if(column == "*")
			return ctx.get_column(1);
		else
		{
			result.push_back(ctx.get_column());
			return result;
		}

	}
}

Where::Where()
{}

Where::Where(function<bool(string)> predicate): pred(predicate)
{}

void display_result(string query, vector<vector<string>> result)
{
	cout<<"\n_____________________________________________________\n"<<"Query : "<<query <<endl;
	cout<<"\nResult of the query is : ";

	for (auto i = result.begin(); i != result.end(); ++i)
  		for (auto j = (*i).begin(); j != (*i).end(); ++j)
			   cout << *j << " ";
			cout<<endl;
	cout<<"\n_____________________________________________________\n";

}

Language::~Language()
{

}

SQL::SQL(string q) : query(q)
{

}

void SQL::tokenize()
{

 	//vector<string> tokens;
	char q[query.size() + 1];
	query.copy(q, query.size() + 1);
	q[query.size()] = '\0';
 	char *word = strtok(q, " ");

 	while (word != NULL)
 	{
 		tokens.push_back(word);
 		word = strtok(NULL, " ");
 	}

}

vector<vector<string>> SQL::evaluate_query(Context &ctx)
{
	vector<vector<string>> result;
	string first = *tokens.begin();

	if(strcmp("SELECT",first.c_str())==0)
	{

		  //selectfrom(tokens[1]);
	}
	else if (strcmp("INSERT",first.c_str())==0)
	{
		//cout<<"INSERT";
		// "INSERT INTO t values A:d,B:i,C:y";
		map<string, string> insert_map;

		string values = *(tokens.begin()+4);
		char v[values.size() + 1];
		values.copy(v, values.size() + 1);
		v[values.size()] = '\0';
		//cout<<v<<endl;
		char *entry = strtok(v, ",");
		vector<char*> entries;
	 	while (entry != NULL)
	 	{
			//cout<<entry<<endl;
			entries.push_back(entry);
	 		entry = strtok(NULL, ",");
	 	}
		for(auto it= entries.begin(); it!=entries.end(); ++it)
		{

			char *col = strtok(*it, ":");
			char *val = strtok(NULL, ":");
			insert_map.insert({col,val});
		}


		Expression *q = new Insert(*(tokens.begin()+2) , Values(insert_map));
		result = q->interpret(ctx);

	}
	else if (strcmp("DELETE",first.c_str())==0)
	{

	}
	else
	{
		vector<string> res;
		res.push_back("\nThe given query is not found");
		result.push_back(res);
	}
	return result;
}

SQL::~SQL()
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
