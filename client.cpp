#include "interpreter.h"

int main(int argc, char const *argv[])
{
	vector<string> col_names{"A", "B", "C"};
	table t = table(col_names);
	map<string, string> m = {{"A", "a"}, {"B", "b"}, {"C", "c"}};
	map<string, string> o = {{"A", "x"}, {"B", "y"}, {"C", "z"}};
	map<string, string> n = {{"A", "p"}, {"B", "q"}, {"C", "r"}};
	map<string, string> p = {{"A", "h"}, {"B", "b"}, {"C", "j"}};


	t.add_row(m);
	t.add_row(n);
	t.add_row(o);
	t.add_row(p);
	t.display();

	char sql_query1[] ="SELECT B FROM t";
	char sql_query2[] ="DELETE FROM t WHERE B = b";
	//Context context= Context();
	//context.interpret(sql_query);

	t.interpret(sql_query1);
	t.interpret(sql_query2);
	t.display();

	return 0;
}
