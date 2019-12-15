#include "interpreter.h"

int main(int argc, char const *argv[])
{
	Context context= Context();


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
	context.set_table(table_name,t);


	char sql_query1[] ="SELECT B FROM t";
	char sql_query2[] ="DELETE FROM t WHERE B = b";


	//context.interpret(sql_query);
	//
	// t.interpret(sql_query1);
	// t.interpret(sql_query2);
	// t.display();

	return 0;
}
