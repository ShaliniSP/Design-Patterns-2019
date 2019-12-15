#include "table.h"

Table::Table():size(0)
{}

Table::Table(string table_name, vector<string> column_names)
{
	name=table_name;
	size = 0;
	for(auto name : column_names)
	{
		t.insert(pair<string,vector<string> >(name, vector<string>()));
	}
}

Table::Table(const Table& rhs)
{
	size = rhs.size;
	t = rhs.t;
}

Table& Table::operator=(const Table& rhs)
{
	if(this != &rhs)
	{
		size = rhs.size;
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
	++size;
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
	--size;
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