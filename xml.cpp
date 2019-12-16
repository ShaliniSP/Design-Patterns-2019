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
    string table_name = (*(tokens.begin()+1)).substr(1,(*(tokens.begin()+2)).find(">")-1);
    vector<string> values;
    map<string, string> insert_map;
    int index = 2 ;
    string col = *(values.begin()+index);
    while(index<values.size())
    {
      string column_name = col.substr(2,col.find(">")-2);
      string val = *(values.begin() + index - 1);
      //cout<<column_name<<" "<<val<<endl;;

      index+=3;
      col = *(values.begin()+index);
      insert_map.insert({column_name,val});

    }

		Expression *q = new Insert(table_name , Values(insert_map));
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


XML::~XML()
{

}
