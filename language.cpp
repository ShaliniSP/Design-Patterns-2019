#include "language.h"

Language::~Language()
{

}

void Language::display_result(string query, vector<vector<string>> result)
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

