#include "interpreter.h"

int main(int argc, char const *argv[])
{
	Database db= Database();

	//Table 1
	vector<string> col_names{"A", "B", "C"};
	string table_name = "t";
	Table t = Table(table_name,col_names);
	map<string, string> m = {{"A", "a"}, {"B", "b"}, {"C", "c"}};
	map<string, string> o = {{"A", "x"}, {"B", "y"}, {"C", "z"}};
	map<string, string> n = {{"A", "p"}, {"B", "q"}, {"C", "r"}};
	map<string, string> p = {{"A", "h"}, {"B", "b"}, {"C", "j"}};
	t.add_row(m);
	t.add_row(n);
	t.add_row(o);
	t.add_row(p);
	db.add_table(table_name,t);



	//Table 2

	vector<string> col_names2{"srn", "name", "grade"};
	string table_name2 = "school";
	Table school = Table(table_name2,col_names2);
	map<string, string> x = {{"srn", "1"}, {"name", "John"}, {"grade", "B"}};
	map<string, string> y = {{"srn", "2"}, {"name", "Jane"}, {"grade", "A"}};
	school.add_row(x);
	school.add_row(y);
	db.add_table(table_name2,school);
	db.display_tables();



	//Evaluating queries:
	Context ctx = Context(db);

	string lang_name= "SQL";
	string query1 = "INSERT INTO t values A:d,B:i,C:y";
  string query2 = "SELECT * FROM t WHERE B != b";
	vector<vector<string>> result;

	if(lang_name=="SQL")
	{
		Language *l = new SQL(query1);
		l->tokenize();
		result= l->evaluate_query(ctx);
	}

	display_result ( query1, result );
	ctx.db.display_tables();
	ctx.clear();

	if(lang_name=="SQL")
	{
		Language *l = new SQL(query2);
		l->tokenize();
		result= l->evaluate_query(ctx);
	}

	display_result ( query2, result );
	ctx.db.display_tables();
	ctx.clear();

	lang_name= "REST_methods";
	//string query3 = "IN school POST DATA srn:5,name:Jack,grade:C";
	string query3 = "IN t GET A SUCH THAT B = b";

	if(lang_name=="REST_methods")
	{
		Language *l = new REST_methods(query3);
		l->tokenize();
		result= l->evaluate_query(ctx);
	}

	display_result ( query3, result );
	ctx.db.display_tables();
	ctx.clear();

	return 0;
}
