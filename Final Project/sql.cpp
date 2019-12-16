#include "sql.h"

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
				result = q->interpret(ctx);
			}
		}
		else
		{
			//cout << *(tokens.begin()+1);
			string temp = " ";
			isEqual equal(temp);
			Expression *q = new From(*(tokens.begin()+3) , Select("*(tokens.begin()+1)", Where(temp, equal)));
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
	else if (strcmp("DELETE",first.c_str())==0)
	{
			//DELETE FROM t WHERE B = b
			if(tokens.size()==7)
			{
				if(*(tokens.begin()+5) == "=")
				{
							isEqual equal(*(tokens.begin()+6));
							//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
							Expression *q = new From(*(tokens.begin()+2) ,Delwhere(*(tokens.begin()+4), equal ));
							result = q->interpret(ctx);
				}
				if(*(tokens.begin()+5) == "!=")
				{
							isNotEqual not_equal(*(tokens.begin()+5));
							//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
							Expression *q = new From(*(tokens.begin()+2) ,Delwhere(*(tokens.begin()+4), not_equal ));
							result = q->interpret(ctx);
				}
				if(*(tokens.begin()+5) == "startswith")
				{
							startsWith sw(*(tokens.begin()+5));
							//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
							Expression *q = new From(*(tokens.begin()+2) ,Delwhere(*(tokens.begin()+4), sw ));
							result = q->interpret(ctx);
				}
			}
			else
			{
				string temp= " ";
				isEqual equal(temp);
				//result = ctx.delete_on_filter(*(tokens.begin()+4), equal);
				Expression *q = new From(*(tokens.begin()+2) ,Delwhere(temp, equal ));
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


SQL::~SQL()
{

}
