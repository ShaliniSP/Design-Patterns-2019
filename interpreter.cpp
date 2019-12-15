#include "interpreter.h"
#include "language.h"
#include "expression.h"
#include "predicates.h"
#include <string>

// class isEqual
// {
// 	private:
// 		string filter_string;
// 	public:
// 		isEqual(string s) : filter_string(s){}
// 		bool operator()(string check)
// 		{
// 			return filter_string==check;
// 		}
// };
// class isNotEqual
// {
// 	private:
// 		string filter_string;
// 	public:
// 		isNotEqual(string s) : filter_string(s){}
// 		bool operator()(string check)
// 		{
// 			return filter_string!=check;
// 		}
// };
// class startsWith
// {
// 	private:
// 		string filter_string;
// 	public:
// 		startsWith(string s) : filter_string(s){}
// 		bool operator()(string check)
// 		{
// 			return check.find(filter_string)==0;
// 		}
// };


// Table::Table():size(0)
// {}

// Table::Table(string table_name, vector<string> column_names)
// {
// 	name=table_name;
// 	size = 0;
// 	for(auto name : column_names)
// 	{
// 		t.insert(pair<string,vector<string> >(name, vector<string>()));
// 	}
// }

// Table::Table(const Table& rhs)
// {
// 	size = rhs.size;
// 	t = rhs.t;
// }

// Table& Table::operator=(const Table& rhs)
// {
// 	if(this != &rhs)
// 	{
// 		size = rhs.size;
// 		t = rhs.t;
// 	}
// 	return *this;
// }

// void Table::add_row(map<string, string> row)
// {
// 	for(auto r: row)
// 	{
// 		t[r.first].push_back(r.second);
// 	}
// 	++size;
// }

// void Table::display()//change to make it display like a table
// {
// 	cout<<"\n";
// 	cout<<endl<<"--------------------"<<endl;
// 	for(auto col : t)
// 	{
// 	 	cout << col.first <<"\t";
// 	}
// 	cout<<endl<<"-------------------- "<<endl;

// 	int num_rows;
// 	for(auto col : t)
// 	{
// 		num_rows= col.second.size();
// 	}
// 	vector<vector<string>> rows;
// 	for(int i=0;i < num_rows; i++)
// 	{
// 		vector<string> row;
// 		for(auto col: t)
// 		{
// 			vector<string>::iterator ite = col.second.begin()+i;
// 			row.push_back(*ite);
// 		}
// 		rows.push_back(row);

// 	}
// 	for(auto row :rows)
// 	{
// 		for(auto column: row)
// 		{
// 			cout<<column<<"\t";
// 		}
// 		cout<<endl;
// 	}
// 		cout<<endl;

// }

// void Table::del_row(int row_num)//delete does not work. have to use references??
// {

// 	for(auto col: t)
// 	{
// 		vector<string>::iterator ite = col.second.begin()+row_num;

// 		col.second.erase(col.second.begin()+row_num);

// 		t[col.first] = col.second;
// 	}
// 	--size;
// }

// vector<string> Table::sel_row(int row_num)//returns row as vector of strings
// {
// 	vector<string> row;
// 	for(auto col: t)
// 	{
// 		vector<string>::iterator ite = col.second.begin()+row_num;
// 		row.push_back(*ite);
// 	}
// 	return row;
// }

// vector<string> Table::sel_col (string colname)
// {
// 	vector<string> column;
// 	cout<<colname<<" : ";
// 	for(auto col : t)
// 	{
// 		if( col.first == colname)
// 		{
// 			return col.second;
// 		}
// 	}
// }

// Table::~Table()
// {}

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
			//cout << *rit;
			t.del_row(*rit);
		}
		num_of_del_rows = to_string(indices.size());
	}

	db.tables[table] = t;
	return {{"Deleted ", num_of_del_rows, " Rows"}};
}

Context::~Context()
{}


// Database::Database()
// {}

// void Database::add_table(string table_name, Table table)
// {
// 	auto it= tables.find(table_name);
// 	if(it==tables.end())
// 	{
// 		//table is added for the first time
// 		tables.insert({table_name, table});
// 	}
// 	else
// 	{
// 		(it->second) = table;
// 		display_tables();
// 		//cout<<"\nTable changed: ";
// 		//(it->second).display();

// 	}
// }

// void Database::del_table(string table_name)
// {
// 	tables.erase(table_name);
// 	cout<<"Table "<< table_name<< " delted.\n";

// }

// void Database::display_tables()
// {
// 	auto it= tables.begin();
// 	cout<<"\nThe Tables in the database are:\n "<<endl;
// 	for(; it!=tables.end(); ++it)
// 	{

// 		cout<<"Table Name: " << it->first;
// 		(it->second).display();
// 	}
// }

// Database::~Database()
// {}

// Expression::~Expression()
// {}


// Insert::Insert()
// {}

// Insert::Insert(string table_name, Values v): table(table_name), values(v)
// {}

// vector<vector<string>> Insert::interpret(Context &ctx)
// {
// 	ctx.set_table(table);
// 	vector<vector<string>> temp = values.interpret(ctx);

// 	return temp;
// }


// Values::Values()
// {}

// Values::Values(map<string, string> row_to_be_inserted): row(row_to_be_inserted)
// {}

// vector<vector<string>> Values::interpret(Context &ctx)
// {
// 	Table t = ctx.get_table();
// 	t.add_row(row);
// 	//t.display();
// 	ctx.db.tables[ctx.table] = t;
// 	//ctx.db.add_table(ctx.table, t);
// 	return {{"Row added successfully"}};
// }

// From::From()
// {}

// From::From(string table_name, Select s)//: table(table_name), select(s), del(Delwhere(string str(" "), isEqual equal(string str(" "))))
// {
// 	table = table_name;
// 	select = s;
// 	string temp = " ";
// 	isEqual equal(temp);
// 	del = Delwhere(temp, equal);
// }

// From::From(string table_name, Delwhere d)//: table(table_name), select(s),
// {
// 	table = table_name;
// 	del = d;
// 	string temp = " ";
// 	isEqual equal(temp);
// 	select = Select(temp, Where(temp, equal));
// }

// vector<vector<string>> From::interpret(Context &ctx)
// {
// 	ctx.set_table(table);
// 	cout << "From";
// 	if(select.column == " ")
// 		return del.interpret(ctx);
// 	else
// 		return select.interpret(ctx);
// }

// Select::Select()
// {}

// Select::Select(string column_name):column(column), isWhere(false)
// {
// 	cout << "CTOR CALLEd" << column;
// }

// Select::Select(string column_name, Where w): column(column_name), isWhere(where.filter_col == " "? false:true), where(w)
// {}

// vector<vector<string>> Select::interpret(Context &ctx)
// {
// 	ctx.set_column(column);
// 	cout << "COL:" << ctx.column;

// 	vector<vector<string>> result;
// 	if(isWhere)
// 	{
// 		return where.interpret(ctx);
// 	}
// 	else
// 	{
// 		cout << "No where";
// 		if(column == "*")
// 			return ctx.get_column(1);
// 		else
// 		{
// 			result.push_back(ctx.get_column());
// 			return result;
// 		}

// 	}
// }

// Where::Where()
// {}

// Where::Where(string f_col, function<bool(string)> predicate): filter_col(f_col), pred(predicate)
// {}

// vector<vector<string>> Where::interpret(Context &ctx)
// {
// 	return ctx.search_on_filter(filter_col, pred);
// }

// Delwhere::Delwhere()
// {}

// Delwhere::Delwhere(string f_col, function<bool(string)> predicate): filter_col(f_col), pred(predicate)
// {}

// vector<vector<string>> Delwhere::interpret(Context &ctx)
// {
// 	return ctx.delete_on_filter(filter_col, pred);
// }

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

// Language::~Language()
// {

// }

// SQL::SQL(string q) : query(q)
// {

// }

// void SQL::tokenize()
// {

//  	//vector<string> tokens;
// 	char q[query.size() + 1];
// 	query.copy(q, query.size() + 1);
// 	q[query.size()] = '\0';
//  	char *word = strtok(q, " ");

//  	while (word != NULL)
//  	{
//  		tokens.push_back(word);
//  		word = strtok(NULL, " ");
//  	}

// }

// vector<vector<string>> SQL::evaluate_query(Context &ctx)
// {
// 	vector<vector<string>> result;
// 	string first = *tokens.begin();

// 	if(strcmp("SELECT",first.c_str())==0)
// 	{
// 		//SELECT A FROM t WHERE B = b
// 		ctx.column = *(tokens.begin()+1);
// 		if(tokens.size()==8)
// 		{
// 			if(*(tokens.begin()+6) == "=")
// 			{
// 				isEqual equal( *(tokens.begin()+7));
// 			//	result = ctx.search_on_filter(*(tokens.begin()+5), equal);
// 				Expression *q = new From(*(tokens.begin()+3) , Select(*(tokens.begin()+1) , Where(*(tokens.begin()+5), equal) ));
// 				result = q->interpret(ctx);
// 			}
// 			else if(*(tokens.begin()+6) == "!=")
// 			{
// 				isNotEqual not_equal( *(tokens.begin()+7));
// 				Expression *q = new From(*(tokens.begin()+3) , Select(*(tokens.begin()+1) , Where(*(tokens.begin()+5), not_equal) ));
// 				result = q->interpret(ctx);			}
// 			else if(*(tokens.begin()+6) == "startswith")
// 			{
// 				startsWith sw( *(tokens.begin()+7));
// 				Expression *q = new From(*(tokens.begin()+3) , Select(*(tokens.begin()+1) , Where(*(tokens.begin()+5), sw) ));
// 				result = q->interpret(ctx);
// 			}
// 		}
// 		else
// 		{
// 			//cout << *(tokens.begin()+1);
// 			string temp = " ";
// 			isEqual equal(temp);
// 			Expression *q = new From(*(tokens.begin()+3) , Select(*(tokens.begin()+1), Where(temp, equal)));
// 			result = q->interpret(ctx);
// 		}

// 	}
// 	else if (strcmp("INSERT",first.c_str())==0)
// 	{
// 		// "INSERT INTO t values A:d,B:i,C:y";
// 		map<string, string> insert_map;

// 		string values = *(tokens.begin()+4);
// 		char v[values.size() + 1];
// 		values.copy(v, values.size() + 1);
// 		v[values.size()] = '\0';

// 		char *entry = strtok(v, ",");
// 		vector<char*> entries;
// 	 	while (entry != NULL)
// 	 	{
// 			//cout<<entry<<endl;
// 			entries.push_back(entry);
// 	 		entry = strtok(NULL, ",");
// 	 	}
// 		for(auto it= entries.begin(); it!=entries.end(); ++it)
// 		{

// 			char *col = strtok(*it, ":");
// 			char *val = strtok(NULL, ":");
// 			insert_map.insert({col,val});
// 		}


// 		Expression *q = new Insert(*(tokens.begin()+2) , Values(insert_map));
// 		result = q->interpret(ctx);

// 	}
// 	else if (strcmp("DELETE",first.c_str())==0)
// 	{
// 			//DELETE FROM t WHERE B = b
// 			if(tokens.size()==7)
// 			{
// 				if(*(tokens.begin()+5) == "=")
// 				{
// 							isEqual equal(*(tokens.begin()+6));
// 							//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
// 							Expression *q = new From(*(tokens.begin()+2) ,Delwhere(*(tokens.begin()+4), equal ));
// 							result = q->interpret(ctx);
// 				}
// 				if(*(tokens.begin()+5) == "!=")
// 				{
// 							isNotEqual not_equal(*(tokens.begin()+5));
// 							//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
// 							Expression *q = new From(*(tokens.begin()+2) ,Delwhere(*(tokens.begin()+4), not_equal ));
// 							result = q->interpret(ctx);
// 				}
// 				if(*(tokens.begin()+5) == "startswith")
// 				{
// 							startsWith sw(*(tokens.begin()+5));
// 							//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
// 							Expression *q = new From(*(tokens.begin()+2) ,Delwhere(*(tokens.begin()+4), sw ));
// 							result = q->interpret(ctx);
// 				}
// 			}
// 			else
// 			{
// 				string temp= " ";
// 				isEqual equal(temp);
// 				//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
// 				Expression *q = new From(*(tokens.begin()+2) ,Delwhere(temp, equal ));
// 				result = q->interpret(ctx);
// 			}
// 	}
// 	else
// 	{
// 		vector<string> res;
// 		res.push_back("\nThe given query is not found");
// 		result.push_back(res);
// 	}
// 	return result;
// }


// SQL::~SQL()
// {

// }



// REST_methods::REST_methods(string q) : query(q)
// {

// }

// void REST_methods::tokenize()
// {

//  	//vector<string> tokens;
// 	char q[query.size() + 1];
// 	query.copy(q, query.size() + 1);
// 	q[query.size()] = '\0';
//  	char *word = strtok(q, " ");

//  	while (word != NULL)
//  	{
//  		tokens.push_back(word);
//  		word = strtok(NULL, " ");
//  	}

// }

// vector<vector<string>> REST_methods::evaluate_query(Context &ctx)
// {
// 	vector<vector<string>> result;
// 	string first = *(tokens.begin()+2);
// //cout<<*(tokens.begin()+3) ;
// 	if(strcmp("GET",first.c_str())==0)
// 	{
// 		//IN t GET ALL SUCH THAT B = b

// 		 if(*(tokens.begin()+3) == "ALL")
// 		 {
// 		 	ctx.column = "*";
// 		 }
// 		 else{
// 			 	ctx.column = *(tokens.begin()+3);
// 		 }
// 		 cout<<tokens.size();
// 		if(tokens.size()==9)
// 		{
// 			if(*(tokens.begin()+7) == "=")
// 			{

// 				isEqual equal( *(tokens.begin()+8));
// 			//	result = ctx.search_on_filter(*(tokens.begin()+5), equal);
// 				Expression *q = new From(*(tokens.begin()+1) , Select(ctx.column, Where(*(tokens.begin()+6), equal) ));
// 				result = q->interpret(ctx);
// 			}
// 			else if(*(tokens.begin()+7) == "!=")
// 			{
// 				isNotEqual not_equal( *(tokens.begin()+8));
// 					Expression *q = new From(*(tokens.begin()+1) , Select(ctx.column , Where(*(tokens.begin()+6), not_equal) ));
// 					result = q->interpret(ctx);;
// 			}
// 			else if(*(tokens.begin()+7) == "startswith")
// 			{
// 				startsWith sw( *(tokens.begin()+8));
// 				Expression *q = new From(*(tokens.begin()+1) , Select(ctx.column , Where(*(tokens.begin()+6), sw) ));
// 				result = q->interpret(ctx);;
// 			}
// 		}
// 		else
// 		{
// 			Expression *q = new From(*(tokens.begin()+1) , Select(ctx.column));
// 			result = q->interpret(ctx);
// 		}

// 	}
// 	else if (strcmp("POST",first.c_str())==0)
// 	{


// 		map<string, string> insert_map;

// 		string values = *(tokens.begin()+4);
// 		char v[values.size() + 1];
// 		values.copy(v, values.size() + 1);
// 		v[values.size()] = '\0';

// 		char *entry = strtok(v, ",");
// 		vector<char*> entries;
// 	 	while (entry != NULL)
// 	 	{
// 			//cout<<entry<<endl;
// 			entries.push_back(entry);
// 	 		entry = strtok(NULL, ",");
// 	 	}
// 		for(auto it= entries.begin(); it!=entries.end(); ++it)
// 		{

// 			char *col = strtok(*it, ":");
// 			char *val = strtok(NULL, ":");
// 			insert_map.insert({col,val});
// 		}


// 		Expression *q = new Insert(*(tokens.begin()+1) , Values(insert_map));
// 		result = q->interpret(ctx);

// 	}
// 	else if (strcmp("DELETE",first.c_str())==0)
// 	{
// 			//FROM t DELETE ALL
// 			//FROM t DELETE IF B = b
// 		if(tokens.size()==7)
// 		{
// 			if(*(tokens.begin()+5) == "=")
// 			{
// 						isEqual equal(*(tokens.begin()+6));
// 						//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
// 						Expression *q = new From(*(tokens.begin()+1) ,Delwhere(*(tokens.begin()+4), equal ));
// 						result = q->interpret(ctx);
// 			}
// 			if(*(tokens.begin()+5) == "!=")
// 			{
// 						isNotEqual not_equal(*(tokens.begin()+5));
// 						//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
// 						Expression *q = new From(*(tokens.begin()+1) ,Delwhere(*(tokens.begin()+4), not_equal ));
// 						result = q->interpret(ctx);
// 			}
// 			if(*(tokens.begin()+5) == "startswith")
// 			{
// 						startsWith sw(*(tokens.begin()+5));
// 						//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
// 						Expression *q = new From(*(tokens.begin()+1) ,Delwhere(*(tokens.begin()+4), sw ));
// 						result = q->interpret(ctx);
// 			}
// 		}
// 		else
// 		{
// 			string temp= " ";
// 			isEqual equal(temp);
// 			//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
// 			Expression *q = new From(*(tokens.begin()+1) ,Delwhere(temp, equal ));
// 			result = q->interpret(ctx);
// 		}
// 	}
// 	else
// 	{
// 		vector<string> res;
// 		res.push_back("\nThe given query is not found");
// 		result.push_back(res);
// 	}
// 	return result;
// }

// REST_methods::~REST_methods()
// {

// }
