#include "interpreter.h"

int main(int argc, char const *argv[])
{
	Database db= Database();


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
	t.display();
	db.add_table(table_name,t);


	char sql_query1[] ="SELECT B FROM t";
	char sql_query2[] ="DELETE FROM t WHERE B = b";


	//Another table

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
	string lang_name= "SQL";
	string query1 = "INSERT INTO t values A:d,B:i,C:y";
	string query2 = "SELECT A FROM t WHERE B=b";
	vector<vector<string>> result;
	if(lang_name=="SQL")
	{
		Language *l = new SQL(query1);
		l->tokenize();
		result= l->evaluate_query();
	}

	display_result ( query1, result );
	//context.interpret(sql_query);
	//
	// t.interpret(sql_query1);
	// t.interpret(sql_query2);
	// t.display();

	return 0;
}
