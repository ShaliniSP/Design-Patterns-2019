#include "interpreter.h"
#include <string>

class isEqual
{
	private:
		string filter_string;
	public:
		isEqual(string s) : filter_string(s){}
		bool operator()(string check)
		{
			return filter_string==check;
		}
};
class isNotEqual
{
	private:
		string filter_string;
	public:
		isNotEqual(string s) : filter_string(s){}
		bool operator()(string check)
		{
			return filter_string!=check;
		}
};
class startsWith
{
	private:
		string filter_string;
	public:
		startsWith(string s) : filter_string(s){}
		bool operator()(string check)
		{
			return check.find(filter_string)==0;
		}
};


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
	cout<<"\n";
	cout<<endl<<"--------------------"<<endl;
	for(auto col : t)
	{
	 	cout << col.first <<"\t";
	}
	cout<<endl<<"-------------------- "<<endl;

	int num_rows;
	for(auto col : t)
	{
		num_rows= col.second.size();
	}
	vector<vector<string>> rows;
	for(int i=0;i < num_rows; i++)
	{
		vector<string> row;
		for(auto col: t)
		{
			vector<string>::iterator ite = col.second.begin()+i;
			row.push_back(*ite);
		}
		rows.push_back(row);

	}
	for(auto row :rows)
	{
		for(auto column: row)
		{
			cout<<column<<"\t";
		}
		cout<<endl;
	}
		cout<<endl;

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
	vector<vector<string>> temp = values.interpret(ctx);

	return temp;
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

vector<vector<string>> From::interpret(Context &ctx)
{
	ctx.set_table(table);
	return select.interpret(ctx);
}

Select::Select()
{}

Select::Select(string column_name):column(column), isWhere(false)
{}

Select::Select(string column_name, Where w): column(column_name), isWhere(true), where(w)
{}

vector<vector<string>> Select::interpret(Context &ctx)
{
	ctx.set_column(column);
	vector<vector<string>> result;
	if(isWhere)
	{
		return where.interpret(ctx);
	}
	else
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

Where::Where(string f_col, function<bool(string)> predicate): filter_col(f_col), pred(predicate)
{}

vector<vector<string>> Where::interpret(Context &ctx)
{
	return ctx.search_on_filter(filter_col, pred);
}

void display_result(string query, vector<vector<string>> result)
{
	cout<<"\n__________________________________________________________________________\n"<<"Query : "<<query <<endl;
	cout<<"\nResult of the query is : \n";

	for (auto i = result.begin(); i != result.end(); ++i)
	{
  		for (auto j = (*i).begin(); j != (*i).end(); ++j)
			   cout << *j << " ";
			cout<<endl;
	}
	cout<<"\n__________________________________________________________________________\n";

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
		//SELECT A FROM t WHERE B = b
		ctx.column = *(tokens.begin()+1);
		if(tokens.size()==8)
		{
			if(*(tokens.begin()+6) == "=")
			{
				isEqual equal( *(tokens.begin()+7));
			//	result = ctx.search_on_filter(*(tokens.begin()+5), equal);
				Expression *q = new From(*(tokens.begin()+3) , Select(*(tokens.begin()+1) , Where(*(tokens.begin()+5), equal) ));
				result = q->interpret(ctx);
			}
			else if(*(tokens.begin()+6) == "!=")
			{
				isNotEqual not_equal( *(tokens.begin()+7));
				Expression *q = new From(*(tokens.begin()+3) , Select(*(tokens.begin()+1) , Where(*(tokens.begin()+5), not_equal) ));
				result = q->interpret(ctx);			}
			else if(*(tokens.begin()+6) == "startswith")
			{
				startsWith sw( *(tokens.begin()+7));
				Expression *q = new From(*(tokens.begin()+3) , Select(*(tokens.begin()+1) , Where(*(tokens.begin()+5), sw) ));
				result = q->interpret(ctx);			}
		}
		else
		{
		// 	Expression *q = new From(*(tokens.begin()+3) , Select(*(tokens.begin()+1) ));
		// 	result = q->interpret(ctx);
		}

	}
	else if (strcmp("INSERT",first.c_str())==0)
	{
		// "INSERT INTO t values A:d,B:i,C:y";
		map<string, string> insert_map;

		string values = *(tokens.begin()+4);
		char v[values.size() + 1];
		values.copy(v, values.size() + 1);
		v[values.size()] = '\0';

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



REST_methods::REST_methods(string q) : query(q)
{

}

void REST_methods::tokenize()
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

vector<vector<string>> REST_methods::evaluate_query(Context &ctx)
{
	vector<vector<string>> result;
	string first = *(tokens.begin()+2);
//cout<<*(tokens.begin()+3) ;
	if(strcmp("GET",first.c_str())==0)
	{
		//IN t GET ALL SUCH THAT B = b

		 if(*(tokens.begin()+3) == "ALL")
		 {
		 	ctx.column = "*";
		 }
		 else{
			 	ctx.column = *(tokens.begin()+3);
		 }
		 cout<<tokens.size();
		if(tokens.size()==9)
		{
			if(*(tokens.begin()+7) == "=")
			{

				isEqual equal( *(tokens.begin()+8));
			//	result = ctx.search_on_filter(*(tokens.begin()+5), equal);
				Expression *q = new From(*(tokens.begin()+1) , Select(ctx.column, Where(*(tokens.begin()+6), equal) ));
				result = q->interpret(ctx);
			}
			else if(*(tokens.begin()+7) == "!=")
			{
				isNotEqual not_equal( *(tokens.begin()+8));
					Expression *q = new From(*(tokens.begin()+1) , Select(ctx.column , Where(*(tokens.begin()+6), not_equal) ));
					result = q->interpret(ctx);;
			}
			else if(*(tokens.begin()+7) == "startswith")
			{
				startsWith sw( *(tokens.begin()+8));
				Expression *q = new From(*(tokens.begin()+1) , Select(ctx.column , Where(*(tokens.begin()+6), sw) ));
				result = q->interpret(ctx);;
			}
		}
		else
		{
			Expression *q = new From(*(tokens.begin()+1) , Select(ctx.column));
			result = q->interpret(ctx);
		}

	}
	else if (strcmp("POST",first.c_str())==0)
	{

		// "INSERT INTO t values A:d,B:i,C:y";
		map<string, string> insert_map;

		string values = *(tokens.begin()+4);
		char v[values.size() + 1];
		values.copy(v, values.size() + 1);
		v[values.size()] = '\0';

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


		Expression *q = new Insert(*(tokens.begin()+1) , Values(insert_map));
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

REST_methods::~REST_methods()
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
