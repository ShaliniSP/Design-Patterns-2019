#ifndef EXPRESSION
#define Expression
#include "interpreter.h"
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

class Where : public Expression
{
	private:
		function<bool(string)> pred;

	public:
		string filter_col;
		Where();
		Where(string, function<bool(string)>);
		vector<vector<string>> interpret(Context &ctx);
};


class Select : public Expression
{
	private:
		Where where;
		bool isWhere;
	public:
		string column;
		Select();
		Select(string);
		Select(string, Where);
		vector<vector<string>> interpret(Context &ctx);
};

class Delwhere : public Expression
{
	private:
		function<bool(string)> pred;

	public:
		string filter_col;
		Delwhere();
		Delwhere(string, function<bool(string)>);
		vector<vector<string>> interpret(Context &ctx);
};

class From : public Expression
{
	private:
		string table;
		Select select;
		Delwhere del;
	public:
		From();
		From(string, Select);
		From(string, Delwhere);
		vector<vector<string>> interpret(Context &ctx);
};
#endif