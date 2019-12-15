#include "expression.h"

Expression::~Expression()
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

From::From(string table_name, Select s)//: table(table_name), select(s), del(Delwhere(string str(" "), isEqual equal(string str(" "))))
{
	table = table_name;
	select = s;
	string temp = " ";
	isEqual equal(temp);
	del = Delwhere(temp, equal);
}

From::From(string table_name, Delwhere d)//: table(table_name), select(s),
{
	table = table_name;
	del = d;
	string temp = " ";
	isEqual equal(temp);
	select = Select(temp, Where(temp, equal));
}

vector<vector<string>> From::interpret(Context &ctx)
{
	ctx.set_table(table);
	cout << "From";
	if(select.column == " ")
		return del.interpret(ctx);
	else
		return select.interpret(ctx);
}

Select::Select()
{}

Select::Select(string column_name):column(column), isWhere(false)
{
	cout << "CTOR CALLEd" << column;
}

Select::Select(string column_name, Where w): column(column_name), isWhere(where.filter_col == " "? false:true), where(w)
{}

vector<vector<string>> Select::interpret(Context &ctx)
{
	ctx.set_column(column);
	cout << "COL:" << ctx.column;

	vector<vector<string>> result;
	if(isWhere)
	{
		return where.interpret(ctx);
	}
	else
	{
		cout << "No where";
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

Delwhere::Delwhere()
{}

Delwhere::Delwhere(string f_col, function<bool(string)> predicate): filter_col(f_col), pred(predicate)
{}

vector<vector<string>> Delwhere::interpret(Context &ctx)
{
	return ctx.delete_on_filter(filter_col, pred);
}
