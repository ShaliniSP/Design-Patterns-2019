#include "interpreter.h"

int main(int argc, char const *argv[])
{
	vector<string> col_names{"A", "B", "C"};
	table t = table(col_names);
	map<string, string> m = {{"A", "a"}, {"B", "b"}, {"C", "c"}};
	map<string, string> n = {{"A", "x"}, {"B", "y"}, {"C", "z"}};
	map<string, string> o = {{"A", "p"}, {"B", "q"}, {"C", "r"}};

	t.display();
	t.add_row(m);
	t.display();
	t.del_row(0);
	t.display();
	t.add_row(n);
	t.display();
	t.add_row(o);
	t.display();
	t.add_row(m);
	t.display();
	t.del_row(1);
	t.display();

	return 0;
}
