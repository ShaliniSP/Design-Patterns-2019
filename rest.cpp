#include "rest.h"

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
		 //cout<<tokens.size();
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
			string temp = " ";
			isEqual equal(temp);
			Expression *q = new From(*(tokens.begin()+1) , Select(ctx.column, Where(temp, equal)));
			result = q->interpret(ctx);
		}

	}
	else if (strcmp("POST",first.c_str())==0)
	{


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
			//FROM t DELETE ALL
			//FROM t DELETE IF B = b
		if(tokens.size()==7)
		{
			if(*(tokens.begin()+5) == "=")
			{
						isEqual equal(*(tokens.begin()+6));
						//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
						Expression *q = new From(*(tokens.begin()+1) ,Delwhere(*(tokens.begin()+4), equal ));
						result = q->interpret(ctx);
			}
			if(*(tokens.begin()+5) == "!=")
			{
						isNotEqual not_equal(*(tokens.begin()+5));
						//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
						Expression *q = new From(*(tokens.begin()+1) ,Delwhere(*(tokens.begin()+4), not_equal ));
						result = q->interpret(ctx);
			}
			if(*(tokens.begin()+5) == "startswith")
			{
						startsWith sw(*(tokens.begin()+5));
						//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
						Expression *q = new From(*(tokens.begin()+1) ,Delwhere(*(tokens.begin()+4), sw ));
						result = q->interpret(ctx);
			}
		}
		else
		{
			string temp= " ";
			isEqual equal(temp);
			//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
			Expression *q = new From(*(tokens.begin()+1) ,Delwhere(temp, equal ));
			result = q->interpret(ctx);
		}
	}
	else
	{
		vector<string> res;
		res.push_back("\nThe given query is not found");
		result.push_back(res);
	}
	return result;
}

void REST_methods::display_result(Context ctx, string query, vector<vector<string>> result)
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

REST_methods::~REST_methods()
{

}
