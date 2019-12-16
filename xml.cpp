#include "xml.h"
#include "sql.h"

XML::XML(string q) : query(q)
{

}

void XML::tokenize()
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

vector<vector<string>> XML::evaluate_query(Context &ctx)
{
	vector<vector<string>> result;
	string first = *tokens.begin();

	if(strcmp("DISPLAY",first.c_str())==0)
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
				result = q->interpret(ctx);
			}
		}
		else
		{

			string temp = " ";
			isEqual equal(temp);
			Expression *q = new From(*(tokens.begin()+3) , Select(*(tokens.begin()+1), Where(temp, equal)));
			result = q->interpret(ctx);
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
	else
	{
		vector<string> res;
		res.push_back("\nThe given query is not found");
		result.push_back(res);
	}
	return result;
}

void XML::display_result(Context ctx, string query, vector<vector<string>> result)
{
	ctx.table  = "t";
	Table tab = ctx.get_table();
	vector<string> col = tab.get_col_names();
	cout<<"\n__________________________________________________________________________\n"<<"Query : "<<query <<endl;
	cout<<"\nResult of the query is : \n";
	cout << "<" << ctx.table << ">\n";
	//for (auto i = result.begin(); i != result.end(); ++i)
	for(auto i = 0; i < result.size(); ++i)
	{
		cout << "\t<row" << /*i <<*/ ">\n";
  		//for (auto j = (*i).begin(); j != (*i).end(); ++j)
		for(auto j = 0; j < result[i].size(); ++j)
			   cout << "\t\t<" << col[j] << ">" << result[i][j] << "<" << col[j] << "/>\n";
		cout << "\t</row" << /*i <<*/ ">\n\n";
	}
	cout << "</" << ctx.table << ">\n";
	cout<<"\n__________________________________________________________________________\n";

}

XML::~XML()
{

}
