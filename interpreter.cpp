#include "interpreter.h"
#include <cstring>

table::table(vector<string> column_names)
{
	for(auto name : column_names)
	{
		t.insert(pair<string,vector<string> >(name, vector<string>()));
	}
}

void table::add_row(map<string, string> row)
{
	for(auto r: row)
	{
		t[r.first].push_back(r.second);
	}
}

void table::display()//change to make it display like a table
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



void table::del_row(int row_num)//delete does not work. have to use references??
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

table::~table()
{

}

void table::interpret(char query[])
{

	cout<<query<<endl;
	vector<string> tokens;
	char *word = strtok(query, " ");

	while (word != NULL)
	{
		tokens.push_back(word);
		word = strtok(NULL, " ");
	}
	string first = *tokens.begin();

	if(strcmp("SELECT",first.c_str())==0)
	{
	  selectfrom(tokens[1]);
	}
	if(strcmp("DELETE",first.c_str())==0)
	{
	  deletewhere(tokens[4],tokens[6]);
	}

}

void table::selectfrom (string colname)
{
	cout<<colname<<" : ";
	for(auto col : t)
	{
		if( strcmp(col.first.c_str(),colname.c_str())==0)
		{
			for(auto row : col.second)
				cout << row << " ";
			cout << "\n\n";
		}
	}
}

void table::deletewhere(string colname,string val)
{
		for(auto col: t)
		{
				if( strcmp(col.first.c_str(),colname.c_str())==0)
				{
					int x= 0;
					for(	vector<string>::iterator ite = col.second.begin(); ite!=col.second.end();++ite,++x)
					{
					//	cout<<(*ite).c_str();
						if(strcmp((*ite).c_str(),val.c_str())==0)
						{
								del_row(x);
						}
					}
				}
		}
}

Context::Context()
{

}

Context::~Context()
{

}
